use std::error;

// ----------- //
// Énumération //
// ----------- //

#[derive(Debug)]
#[derive(PartialEq)]
pub enum Node {
    Add(Box<Node>, Box<Node>),
    Sub(Box<Node>, Box<Node>),
    Mul(Box<Node>, Box<Node>),
    Div(Box<Node>, Box<Node>),
    Caret(Box<Node>, Box<Node>),
    Mod(Box<Node>, Box<Node>),
    Neg(Box<Node>),
    Num(f64),
}

// -------- //
// Fonction //
// -------- //

pub fn eval(expr: Node) -> Result<f64, Box<dyn error::Error>> {
    Ok(match expr {
        Node::Num(i) => i,
        Node::Add(e1, e2) => eval(*e1)? + eval(*e2)?,
        Node::Sub(e1, e2) => eval(*e1)? - eval(*e2)?,
        Node::Mul(e1, e2) => eval(*e1)? * eval(*e2)?,
        Node::Div(e1, e2) => eval(*e1)? / eval(*e2)?,
        Node::Mod(e1, e2) => eval(*e1)? % eval(*e2)?,
        Node::Neg(e1) => -(eval(*e1)?),
        Node::Caret(e1, e2) => eval(*e1)?.powf(eval(*e2)?),
    })
}
