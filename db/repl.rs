use std::{
    env,
    io::{self, Write},
};

mod connection;
mod string_buffer;

use connection::Connection;
use string_buffer::StringBuffer;

#[allow(clippy::upper_case_acronyms)]
struct REPL;

impl REPL {
    fn print_prompt() -> io::Result<()> {
        print!("Rust: db > ");
        io::stdout().flush()
    }
}

// ---- //
// Main //
// ---- //

fn main() {
    let database_filename = env::args().nth(1).expect(
        "Erreur: nom de fichier invalide pour la base de données.",
    );

    let connection = Connection::open(&database_filename);

    let exit_code: u8 = 'exit: loop {
        let mut input = StringBuffer::default();

        if REPL::print_prompt()
            .and_then(|_| REPL::read_line(&mut input))
            .is_err()
        {
            break 'exit 1; // EXIT_FAILURE
        }

        if input == ".exit" {
            break 'exit 0; // EXIT_SUCCESS
        } else {
            println!("La commande '{}' est invalide.", input);
        }
    };

    std::process::exit(exit_code as i32);
}
