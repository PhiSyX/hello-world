#[allow(clippy::upper_case_acronyms)]
#[derive(Clone, PartialEq, Debug)]
pub enum Token {
    Add,
    Sub,
    Mul,
    Div,
    Caret,
    Mod,
    LParen,
    RParen,
    Num(f64),
    EOF,
}

#[derive(PartialEq, PartialOrd, Debug)]
pub enum OperatorPrecedence {
    DefaultZero,
    AddSub,
    MulDiv,
    Mod,
    Neg,
    Power,
}

// -------------- //
// Implémentation //
// -------------- //

impl Token {
    pub fn get_operator_precedence(&self) -> OperatorPrecedence {
        match *self {
            Self::Add | Self::Sub => OperatorPrecedence::AddSub,
            Self::Mul | Self::Div => OperatorPrecedence::MulDiv,
            Self::Caret => OperatorPrecedence::Power,
            Self::Mod => OperatorPrecedence::Mod,
            _ => OperatorPrecedence::DefaultZero,
        }
    }
}
