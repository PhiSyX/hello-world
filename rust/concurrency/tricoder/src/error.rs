use thiserror::Error;

#[derive(Error, Debug, Clone)]
pub enum Error {
    #[error("Utilisation: cargo r -- <host.org>")]
    CliUsage,

    #[error("Erreur IO reqwest: {0}")]
    Reqwest(String),
}

impl From<reqwest::Error> for Error {
    fn from(err: reqwest::Error) -> Self {
        Error::Reqwest(err.to_string())
    }
}
