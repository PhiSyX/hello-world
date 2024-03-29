use std::{collections::HashSet, time::Duration};

use futures::{stream, StreamExt};
use reqwest::Client;
use serde::Deserialize;
use trust_dns_resolver::{
    config::{ResolverConfig, ResolverOpts},
    name_server::{
        GenericConnection, GenericConnectionProvider, TokioRuntime,
    },
    AsyncResolver,
};

use crate::{port::Port, Error};

type DnsResolver = AsyncResolver<
    GenericConnection,
    GenericConnectionProvider<TokioRuntime>,
>;

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

    pub async fn enumerate(
        http_client: &Client,
        target: &str,
    ) -> Result<Vec<Self>, Error> {
        let entries: Vec<CrtShEntry> = http_client
            .get(&format!("https://crt.sh/?q=%25.{}&output=json", target))
            .send()
            .await?
            .json()
            .await?;

        let dns_config = ResolverConfig::default();
        let mut dns_options = ResolverOpts::default();
        dns_options.timeout = Duration::from_secs(4);

        let dns_resolver = AsyncResolver::tokio(dns_config, dns_options)
            .expect("Un client DNS");

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

        subdomains.insert(target.into());

        let subdomains = stream::iter(subdomains.into_iter())
            .map(Subdomain::new)
            .filter_map(|subdomain| {
                let dns_resolver = dns_resolver.clone();
                async move {
                    if Self::resolves(&dns_resolver, &subdomain).await {
                        Some(subdomain)
                    } else {
                        None
                    }
                }
            })
            .collect()
            .await;

        Ok(subdomains)
    }

    async fn resolves(
        dns_resolver: &DnsResolver,
        domain: &Subdomain,
    ) -> bool {
        dns_resolver.lookup_ip(&domain.domain).await.is_ok()
    }
}
