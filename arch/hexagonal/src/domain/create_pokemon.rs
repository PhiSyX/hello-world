use std::{convert::TryFrom, sync::Arc};

use crate::{
    domain::entities::{
        Pokemon, PokemonName, PokemonNumber, PokemonTypes,
    },
    repositories::pokemon::{InsertError, Repository},
};

pub struct Request {
    pub number: u16,
    pub name: String,
    pub types: Vec<String>,
}

pub struct Response {
    pub number: u16,
    pub name: String,
    pub types: Vec<String>,
}

pub enum Error {
    BadRequest,
    Conflict,
    Unknown,
}

pub fn execute(
    repository: Arc<dyn Repository>,
    request: Request,
) -> Result<Response, Error> {
    match (
        PokemonNumber::try_from(request.number),
        PokemonName::try_from(request.name),
        PokemonTypes::try_from(request.types),
    ) {
        | (Ok(number), Ok(name), Ok(types)) => {
            match repository.insert(number, name, types) {
                | Ok(Pokemon {
                    number,
                    name,
                    types,
                }) => Ok(Response {
                    number: u16::from(number),
                    name: String::from(name),
                    types: Vec::<String>::from(types),
                }),
                | Err(InsertError::Conflict) => Err(Error::Conflict),
                | Err(InsertError::Unknown) => Err(Error::Unknown),
            }
        }
        | _ => Err(Error::BadRequest),
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::repositories::pokemon::InMemoryRepository;

    #[test]
    fn it_should_return_a_bad_request_error_when_request_is_invalid() {
        let repository = Arc::new(InMemoryRepository::new());
        let request = Request::new(
            PokemonNumber::pikachu(),
            PokemonName::bad(),
            PokemonTypes::pikachu(),
        );

        let response = execute(repository, request);

        match response {
            | Err(Error::BadRequest) => {}
            | _ => unreachable!(),
        };
    }

    #[test]
    fn it_should_return_a_conflict_error_when_pokemon_number_already_exists(
    ) {
        let repository = Arc::new(InMemoryRepository::new());
        repository
            .insert(
                PokemonNumber::pikachu(),
                PokemonName::pikachu(),
                PokemonTypes::pikachu(),
            )
            .ok();
        let request = Request::new(
            PokemonNumber::pikachu(),
            PokemonName::charmander(),
            PokemonTypes::charmander(),
        );

        let response = execute(repository, request);

        match response {
            | Err(Error::Conflict) => {}
            | _ => unreachable!(),
        }
    }

    #[test]
    fn it_should_return_an_unknown_error_when_an_unexpected_error_happens()
    {
        let repository = Arc::new(InMemoryRepository::new().with_error());
        let request = Request::new(
            PokemonNumber::pikachu(),
            PokemonName::pikachu(),
            PokemonTypes::pikachu(),
        );

        let response = execute(repository, request);

        match response {
            | Err(Error::Unknown) => {}
            | _ => unreachable!(),
        };
    }

    #[test]
    fn it_should_return_the_pokemon_otherwise() {
        let repository = Arc::new(InMemoryRepository::new());
        let request = Request::new(
            PokemonNumber::pikachu(),
            PokemonName::pikachu(),
            PokemonTypes::pikachu(),
        );

        let response = execute(repository, request);

        match response {
            | Ok(response) => {
                assert_eq!(
                    response.number,
                    u16::from(PokemonNumber::pikachu())
                );
                assert_eq!(
                    response.name,
                    String::from(PokemonName::pikachu())
                );
                assert_eq!(
                    response.types,
                    Vec::<String>::from(PokemonTypes::pikachu())
                );
            }
            | _ => unreachable!(),
        };
    }

    impl Request {
        fn new(
            number: PokemonNumber,
            name: PokemonName,
            types: PokemonTypes,
        ) -> Self {
            Self {
                number: u16::from(number),
                name: String::from(name),
                types: Vec::<String>::from(types),
            }
        }
    }
}
