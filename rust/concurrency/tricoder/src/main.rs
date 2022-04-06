mod error;
mod port;
mod subdomain;

use std::{env, time::Duration};

pub use error::Error;
use rayon::iter::{IntoParallelIterator, ParallelIterator};
use reqwest::{blocking::Client, redirect};
use subdomain::Subdomain;

fn main() -> Result<(), anyhow::Error> {
    let args: Vec<String> = env::args().collect();

    if args.len() != 2 {
        return Err(Error::CliUsage.into());
    }

    let http_timeout = Duration::from_secs(5);
    let http_client = Client::builder()
        .redirect(redirect::Policy::limited(4))
        .timeout(http_timeout)
        .build()?;

    let pool = rayon::ThreadPoolBuilder::new()
        .num_threads(256)
        .build()
        .unwrap();

    pool.install(|| {
        let scan_result: Vec<Subdomain> =
            Subdomain::enumerate(&http_client, &args[1])
                .unwrap()
                .into_par_iter()
                .map(port::scan)
                .collect();

        scan_result.iter().for_each(|subdomain| {
            println!("{}:", subdomain.domain);

            subdomain.open_ports.iter().for_each(|port| {
                println!("\t{}", port.port);
            });

            println!();
        });
    });

    Ok(())
}
