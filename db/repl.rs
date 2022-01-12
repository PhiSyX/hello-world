mod connection;
mod meta_command;
mod prepare;
mod statement;
mod string_buffer;

use std::{
    env,
    io::{self, Write},
};

use connection::Connection;
use meta_command::MetaCommand;
use prepare::Prepare;
use statement::Statement;
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

        if let Some('.') = input.chars().next() {
            match input.parse() {
                | Ok(MetaCommand::Exit) => break 'exit 0,
                | Err(err) => {
                    eprintln!("{}", err);
                    continue;
                }
            }
        }

        let mut statement = Statement::default();

        match statement.prepare(&input) {
            | Ok(prepare) => {
                statement.execute(&prepare);
                println!("Exécutée.");
            }
            | Err(err) => eprintln!("{}", err),
        }
    };

    std::process::exit(exit_code as i32);
}
