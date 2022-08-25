use std::str::FromStr;

use crate::{prepare::Prepare, string_buffer::StringBuffer};

// ----------- //
// Énumération //
// ----------- //

#[derive(Copy, Clone)]
pub(crate) enum StatementType {
    Insert,
    Select,
}

// --------- //
// Structure //
// --------- //

#[derive(Default)]
pub(crate) struct Statement {
    statement_type: Option<StatementType>,
}

// -------------- //
// Implémentation //
// -------------- //

impl Statement {
    pub(crate) fn execute(&self, prepare: &Prepare) {
        match prepare {
            | Prepare::Insert => {
                println!("Instruction: INSERT")
            }
            | Prepare::Select => {
                println!("Instruction: SELECT")
            }
        }
    }
}

impl Statement {
    pub(crate) fn prepare(
        &mut self,
        input: &StringBuffer,
    ) -> Result<Prepare, &'static str> {
        let maybe_statement_type = input.parse();
        let maybe_prepare = maybe_statement_type.map(Prepare::from);
        self.statement_type = maybe_statement_type.ok();
        maybe_prepare
    }
}

// -------------- //
// Implémentation // -> Interface
// -------------- //

impl FromStr for StatementType {
    type Err = &'static str;

    fn from_str(input: &str) -> Result<Self, Self::Err> {
        Ok(match input {
            | "insert" => StatementType::Insert,
            | "select" => StatementType::Select,
            | _ => return Err("L'instruction est invalide."),
        })
    }
}
