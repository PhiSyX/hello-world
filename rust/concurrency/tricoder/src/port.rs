// Récupéré depuis la commande: (nmap requis)
// awk '$2~/tcp$/' /usr/share/nmap/nmap-services | sort -r -k3 | head -n 100 | tr -s ' ' | cut -d '/' -f1 | sed 's/\S*\s*\(\S*\).*/\1,/'
pub const PORTS_100: [u16; 100] = [
    80, 23, 443, 21, 22, 25, 3389, 110, 445, 139, 143, 53, 135, 3306,
    8080, 1723, 111, 995, 993, 5900, 1025, 587, 8888, 199, 1720, 465, 548,
    113, 81, 6001, 10000, 514, 5060, 179, 1026, 2000, 8443, 8000, 32768,
    554, 26, 1433, 49152, 2001, 515, 8008, 49154, 1027, 5666, 646, 5000,
    5631, 631, 49153, 8081, 2049, 88, 79, 5800, 106, 2121, 1110, 49155,
    6000, 513, 990, 5357, 427, 49156, 543, 544, 5101, 144, 7, 389, 8009,
    3128, 444, 9999, 5009, 7070, 5190, 3000, 5432, 1900, 3986, 13, 1029,
    9, 5051, 6646, 49157, 1028, 873, 1755, 2717, 4899, 9100, 119, 37,
];

use std::{
    net::{SocketAddr, ToSocketAddrs},
    time::Duration,
};

use futures::StreamExt;
use tokio::{net::TcpStream, sync::mpsc};
use tokio_stream::wrappers::ReceiverStream;

use crate::subdomain::Subdomain;

#[derive(Debug, Clone)]
pub struct Port {
    pub port: u16,
    pub is_open: bool,
}

impl Port {
    async fn scan(mut socket_address: SocketAddr, port: u16) -> Self {
        let timeout = Duration::from_secs(3);
        socket_address.set_port(port);

        let is_open = matches!(
            tokio::time::timeout(
                timeout,
                TcpStream::connect(&socket_address)
            )
            .await,
            Ok(Ok(_)),
        );

        Self { port, is_open }
    }
}

pub async fn scan(concurrency: usize, subdomain: Subdomain) -> Subdomain {
    let mut ret = subdomain.clone();

    let socket_addresses: Vec<SocketAddr> =
        format!("{}:1024", subdomain.domain)
            .to_socket_addrs()
            .expect("Des addresses sockets")
            .collect();

    if socket_addresses.is_empty() {
        return subdomain;
    }

    let socket_address = socket_addresses[0];

    // Concurrent stream method 3: using channels
    let (input_tx, input_rx) = mpsc::channel(concurrency);
    let (output_tx, output_rx) = mpsc::channel(concurrency);

    tokio::spawn(async move {
        for port in PORTS_100 {
            let _ = input_tx.send(port).await;
        }
    });

    let input_rx_stream = ReceiverStream::new(input_rx);
    input_rx_stream
        .for_each_concurrent(concurrency, |port| {
            let output_tx = output_tx.clone();
            async move {
                let port = Port::scan(socket_address, port).await;
                if port.is_open {
                    let _ = output_tx.send(port).await;
                }
            }
        })
        .await;
    drop(output_tx);

    let output_rx_stream =
        tokio_stream::wrappers::ReceiverStream::new(output_rx);
    ret.open_ports = output_rx_stream.collect().await;

    ret
}
