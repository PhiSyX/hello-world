use std::sync::Arc;

use rouille;
use serde::{Deserialize, Serialize};

use crate::{
    api::Status, domain::create_pokemon, repositories::pokemon::Repository,
};

#[derive(Deserialize)]
struct Request {
    number: u16,
    name: String,
    types: Vec<String>,
}

#[derive(Serialize)]
struct Response {
    number: u16,
    name: String,
    types: Vec<String>,
}

pub fn serve(
    repository: Arc<dyn Repository>,
    request: &rouille::Request,
) -> rouille::Response {
    let request = match rouille::input::json_input::<Request>(request) {
        | Ok(request) => create_pokemon::Request {
            number: request.number,
            name: request.name,
            types: request.types,
        },
        | _ => return rouille::Response::from(Status::BadRequest),
    };

    match create_pokemon::execute(repository, request) {
        | Ok(create_pokemon::Response {
            number,
            name,
            types,
        }) => rouille::Response::json(&Response {
            number,
            name,
            types,
        }),
        | Err(create_pokemon::Error::BadRequest) => {
            rouille::Response::from(Status::BadRequest)
        }
        | Err(create_pokemon::Error::Conflict) => {
            rouille::Response::from(Status::Conflict)
        }
        | Err(create_pokemon::Error::Unknown) => {
            rouille::Response::from(Status::InternalServerError)
        }
    }
}
