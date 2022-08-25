// ----------- //
// Énumération //
// ----------- //

#[allow(clippy::upper_case_acronyms)]
#[derive(Debug)]
#[derive(PartialEq)]
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

#[derive(Debug)]
#[derive(PartialEq, PartialOrd)]
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
