mod multi_chan;
mod prime_number;

#[tokio::main]
async fn main() {
    prime_number::run().await;
    println!();
    multi_chan::run().await;
}
