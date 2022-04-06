use std::{collections::HashSet, time::Duration};

use reqwest::blocking::Client;
use serde::Deserialize;
use trust_dns_resolver::{
    config::{ResolverConfig, ResolverOpts},
    Resolver,
};

use crate::{port::Port, Error};

#[derive(Debug, Deserialize, Clone)]
pub struct CrtShEntry {
    pub name_value: String,
}

#[derive(Debug, Clone)]
pub struct Subdomain {
    pub domain: String,
    pub open_ports: Vec<Port>,
}

impl Subdomain {
    fn new(domain: String) -> Self {
        Self {
            domain,
            open_ports: vec![],
        }
    }

    fn resolves(&self) -> bool {
        let dns_config = ResolverConfig::default();
        let mut dns_options = ResolverOpts::default();
        dns_options.timeout = Duration::from_secs(4);
        let dns_resolver =
            Resolver::new(dns_config, dns_options).expect("un client DNS");
        dns_resolver.lookup_ip(&self.domain).is_ok()
    }

    pub fn enumerate(
        http_client: &Client,
        target: &str,
    ) -> Result<Vec<Self>, Error> {
        let entries: Vec<CrtShEntry> = http_client
            .get(&format!("https://crt.sh/?q=%25.{}&output=json", target))
            .send()?
            .json()?;

        let mut subdomains: HashSet<String> = entries
            .iter()
            .flat_map(|entry| {
                entry
                    .name_value
                    .lines()
                    .map(|subdomain| subdomain.trim().to_string())
            })
            .filter(|subdomain| {
                subdomain != target || !subdomain.contains('*')
            })
            .collect();

        subdomains.insert(target.to_string());

        let subdomains = subdomains
            .into_iter()
            .map(Subdomain::new)
            .filter(|subdomain| subdomain.resolves())
            .collect();

        Ok(subdomains)
    }
}
