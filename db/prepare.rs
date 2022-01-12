use crate::statement::StatementType;

// ----------- //
// Énumération //
// ----------- //

pub(crate) enum Prepare {
    Insert,
    Select,
}

// -------------- //
// Implémentation // -> Interface
// -------------- //

impl From<StatementType> for Prepare {
    fn from(stmt: StatementType) -> Self {
        match stmt {
            | StatementType::Insert => Self::Insert,
            | StatementType::Select => Self::Select,
        }
    }
}
