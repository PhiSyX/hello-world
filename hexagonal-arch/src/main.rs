mod api;
mod domain;
mod repositories;

use std::sync::Arc;

use repositories::pokemon::InMemoryRepository;

fn main() {
    let repository = Arc::new(InMemoryRepository::new());
    api::serve("localhost:8000", repository);
}
