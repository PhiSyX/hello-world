mod copy;
mod read;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    read::run()?;
    copy::run()?;
    Ok(())
}
