mod error;
mod port;
mod subdomain;

use std::{env, sync::Arc, time::Duration};

pub use error::Error;
use futures::{stream, StreamExt};
use reqwest::Client;
use subdomain::Subdomain;
use tokio::{sync::Mutex, time::Instant};

#[tokio::main]
async fn main() -> Result<(), anyhow::Error> {
    let args: Vec<String> = env::args().collect();

    if args.len() != 2 {
        return Err(Error::CliUsage.into());
    }

    let http_timeout = Duration::from_secs(10);
    let http_client = Client::builder().timeout(http_timeout).build()?;

    let total_ports = 200;
    let total_subdomains = 100;
    let scan_start = Instant::now();

    let subdomains = Subdomain::enumerate(&http_client, &args[1]).await?;

    let scan_result: Vec<Subdomain> = stream::iter(subdomains.into_iter())
        .map(|subdomain| port::scan(total_ports, subdomain))
        .buffer_unordered(total_subdomains)
        .collect()
        .await;

    let scan_duration = scan_start.elapsed();
    println!("Scan effectué en {scan_duration:?}");

    scan_result.iter().for_each(|subdomain| {
        println!("{}:", subdomain.domain);

        subdomain.open_ports.iter().for_each(|port| {
            println!("\t{}: ouvert", port.port);
        });

        println!();
    });

    Ok(())
}
