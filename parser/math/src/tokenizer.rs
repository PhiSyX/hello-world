use std::{iter::Peekable, str::Chars};

use crate::token::Token;

// --------- //
// Structure //
// --------- //

pub struct Tokenizer<'a> {
    expression: Peekable<Chars<'a>>,
}

// -------------- //
// Implémentation //
// -------------- //

impl<'a> Tokenizer<'a> {
    /// Construit une nouvelle instance de [Tokenizer]
    pub fn new(new_expr: &'a str) -> Self {
        Self {
            expression: new_expr.chars().peekable(),
        }
    }
}

// -------------- //
// Implémentation // -> Interface
// -------------- //

impl<'a> Iterator for Tokenizer<'a> {
    type Item = Token;

    fn next(&mut self) -> Option<Token> {
        let next_ch = self.expression.next();

        match next_ch {
            | Some('0'..='9') => {
                let mut numeric = next_ch?.to_string();

                while let Some(next_ch) = self.expression.peek() {
                    if next_ch.is_numeric() || '.'.eq(next_ch) {
                        numeric.push(self.expression.next()?);
                    } else if '('.eq(next_ch) {
                        return None;
                    } else {
                        break;
                    }
                }

                Some(Self::Item::Num(numeric.parse::<f64>().unwrap()))
            }
            | Some('+') => Some(Self::Item::Add),
            | Some('-') => Some(Self::Item::Sub),
            | Some('*') => Some(Self::Item::Mul),
            | Some('/') => Some(Self::Item::Div),
            | Some('^') => Some(Self::Item::Caret),
            | Some('%') => Some(Self::Item::Mod),
            | Some('(') => Some(Self::Item::LParen),
            | Some(')') => Some(Self::Item::RParen),
            | None => Some(Self::Item::EOF),
            | Some(_) => None,
        }
    }
}
