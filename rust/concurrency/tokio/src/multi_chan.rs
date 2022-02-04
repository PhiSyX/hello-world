use tokio::{
    sync::{mpsc, oneshot},
    time::{sleep, Duration},
};

async fn multi_chan(
    chan_u64: mpsc::Sender<i64>,
    chan_str: mpsc::Sender<String>,
    chan_end: oneshot::Sender<()>,
) {
    for i in 0..10 {
        if i % 2 == 0 {
            println!("send rx1");
            chan_u64.send(i).await.unwrap();
        } else {
            println!("send rx2");
            chan_str.send(format!("send_{}", i)).await.unwrap();
        }

        sleep(Duration::from_secs(1)).await;
    }

    chan_end.send(()).unwrap();
}

pub(crate) async fn run() {
    println!("Start");

    let (tx1, mut rx1) = mpsc::channel(100);
    let (tx2, mut rx2) = mpsc::channel(100);
    let (tx_exit, mut rx_exit) = oneshot::channel();

    tokio::spawn(multi_chan(tx1, tx2, tx_exit));

    loop {
        tokio::select! {
            Some(i) = rx1.recv() => {
                println!("recv rx1: {}", i);
            }

            Some(s) = rx2.recv() => {
                println!("recv rx2: {}", s);
            }

            _ = &mut rx_exit => {
                println!("Exit");
                break;
            }
        }
    }
}
