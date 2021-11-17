use std::sync::Arc;

use rouille;
use serde::Serialize;

use crate::{
    api::Status, domain::fetch_all_pokemons,
    repositories::pokemon::Repository,
};

#[derive(Serialize)]
struct Response {
    number: u16,
    name: String,
    types: Vec<String>,
}

pub fn serve(repository: Arc<dyn Repository>) -> rouille::Response {
    match fetch_all_pokemons::execute(repository) {
        | Ok(responses) => rouille::Response::json(
            &responses
                .into_iter()
                .map(|response| Response {
                    number: response.number,
                    name: response.name,
                    types: response.types,
                })
                .collect::<Vec<Response>>(),
        ),
        | Err(fetch_all_pokemons::Error::Unknown) => {
            rouille::Response::from(Status::InternalServerError)
        }
    }
}
