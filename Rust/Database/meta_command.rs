use std::str::FromStr;

// ----------- //
// Énumération //
// ----------- //

pub(crate) enum MetaCommand {
    Exit,
}

// -------------- //
// Implémentation // -> Interface
// -------------- //

impl FromStr for MetaCommand {
    type Err = &'static str;

    fn from_str(input: &str) -> Result<Self, Self::Err> {
        Ok(match input {
            | ".exit" => MetaCommand::Exit,
            | _ => return Err("La commande est invalide."),
        })
    }
}
