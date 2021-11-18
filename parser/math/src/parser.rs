use core::fmt;
use std::error;

use crate::{
    ast::Node,
    token::{OperatorPrecedence, Token},
    tokenizer::Tokenizer,
};

// --------- //
// Structure //
// --------- //

pub struct Parser<'a> {
    tokenizer: Tokenizer<'a>,
    current_token: Token,
}

// ----------- //
// Énumération //
// ----------- //

#[derive(Debug)]
pub enum ParseError {
    UnableToParse(String),
    InvalidOperator(String),
}

// -------------- //
// Implémentation //
// -------------- //

impl<'a> Parser<'a> {
    pub fn new(expression: &'a str) -> Result<Self, ParseError> {
        let mut lexer = Tokenizer::new(expression);

        let current_token = match lexer.next() {
            Some(token) => token,
            None => {
                return Err(ParseError::InvalidOperator(String::from(
                    "caractère invalide",
                )))
            }
        };

        Ok(Self {
            tokenizer: lexer,
            current_token,
        })
    }
}

impl Parser<'_> {
    pub fn parse(&mut self) -> Result<Node, ParseError> {
        self.generate_ast(OperatorPrecedence::DefaultZero)
    }

    fn check_paren(&mut self, expected: Token) -> Result<(), ParseError> {
        if expected == self.current_token {
            self.try_next_token()
        } else {
            Err(ParseError::InvalidOperator(format!(
                "attendue `{:?}`, reçue `{:?}`",
                expected, self.current_token
            )))
        }
    }

    fn generate_ast(
        &mut self,
        operator_precedence: OperatorPrecedence,
    ) -> Result<Node, ParseError> {
        let mut left_expression = self.parse_number()?;

        while operator_precedence < self.current_token.get_operator_precedence() {
            if self.current_token == Token::EOF {
                break;
            }

            let right_expression = self.try_convert_token_to_node(left_expression.clone())?;

            left_expression = right_expression;
        }

        Ok(left_expression)
    }

    fn parse_number(&mut self) -> Result<Node, ParseError> {
        match self.current_token {
            Token::Sub => {
                self.try_next_token()?;
                let expr = self.generate_ast(OperatorPrecedence::Neg)?;
                Ok(Node::Neg(Box::new(expr)))
            }

            Token::Num(i) => {
                self.try_next_token()?;
                Ok(Node::Num(i))
            }

            Token::LParen => {
                self.try_next_token()?;

                let expression = self.generate_ast(OperatorPrecedence::DefaultZero)?;

                self.check_paren(Token::RParen)?;

                if self.current_token == Token::LParen {
                    let right = self.generate_ast(OperatorPrecedence::MulDiv)?;
                    return Ok(Node::Mul(Box::new(expression), Box::new(right)));
                }

                Ok(expression)
            }

            _ => Err(ParseError::UnableToParse(String::from(
                "analyse impossible",
            ))),
        }
    }

    fn try_convert_token_to_node(&mut self, left_expression: Node) -> Result<Node, ParseError> {
        match self.current_token {
            Token::Add => {
                self.try_next_token()?;
                let right_expression = self.generate_ast(OperatorPrecedence::AddSub)?;
                Ok(Node::Add(
                    Box::new(left_expression),
                    Box::new(right_expression),
                ))
            }
            Token::Sub => {
                self.try_next_token()?;
                let right_expression = self.generate_ast(OperatorPrecedence::AddSub)?;
                Ok(Node::Sub(
                    Box::new(left_expression),
                    Box::new(right_expression),
                ))
            }
            Token::Mul => {
                self.try_next_token()?;
                let right_expression = self.generate_ast(OperatorPrecedence::MulDiv)?;
                Ok(Node::Mul(
                    Box::new(left_expression),
                    Box::new(right_expression),
                ))
            }
            Token::Div => {
                self.try_next_token()?;
                let right_expression = self.generate_ast(OperatorPrecedence::MulDiv)?;
                Ok(Node::Div(
                    Box::new(left_expression),
                    Box::new(right_expression),
                ))
            }
            Token::Caret => {
                self.try_next_token()?;
                let right_expression = self.generate_ast(OperatorPrecedence::Power)?;
                Ok(Node::Caret(
                    Box::new(left_expression),
                    Box::new(right_expression),
                ))
            }
            Token::Mod => {
                self.try_next_token()?;
                let right_expression = self.generate_ast(OperatorPrecedence::Mod)?;
                Ok(Node::Mod(
                    Box::new(left_expression),
                    Box::new(right_expression),
                ))
            }
            _ => Err(ParseError::InvalidOperator(format!(
                "entrez un opérateur valide: {:?}",
                self.current_token
            ))),
        }
    }

    fn try_next_token(&mut self) -> Result<(), ParseError> {
        let next_token = match self.tokenizer.next() {
            Some(token) => token,
            None => {
                return Err(ParseError::InvalidOperator(String::from(
                    "caractère invalide",
                )))
            }
        };

        self.current_token = next_token;

        Ok(())
    }
}

// -------------- //
// Implémentation //
// -------------- //

impl fmt::Display for ParseError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(
            f,
            "{}",
            match self {
                Self::UnableToParse(msg) | Self::InvalidOperator(msg) => msg,
            }
        )
    }
}

impl From<Box<dyn error::Error>> for ParseError {
    fn from(_err: Box<dyn error::Error>) -> Self {
        Self::UnableToParse(String::from("Analyse impossible"))
    }
}
