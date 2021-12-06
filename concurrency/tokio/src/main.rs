mod prime_number;

#[tokio::main]
async fn main() {
    prime_number::run().await;
}
