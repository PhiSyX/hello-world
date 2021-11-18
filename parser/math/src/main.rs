mod ast;
mod parser;
mod token;
mod tokenizer;

use std::io;

use parser::ParseError;

use crate::parser::Parser;

// ---- //
// Main //
// ---- //

fn main() {
    println!("Entrez une expression arithmétique :");

    loop {
        let mut input = String::new();

        io::stdin()
            .read_line(&mut input)
            .map(|_| match eval(input) {
                Ok(v) => {
                    println!("\t L'expression calculée retourne: {}", v);
                }
                Err(err) => {
                    eprintln!("\t Erreur lors de l'évaluation de l'expression: '{}'", err);
                    eprintln!("\t Entrez une expression arithmétique valide.");
                }
            })
            .unwrap();
        println!();
    }
}

fn eval(expression: String) -> Result<f64, ParseError> {
    let expression: String = expression.split_whitespace().collect();

    let mut parser = Parser::new(&expression)?;
    let ast = parser.parse()?;

    println!(
        "\t L'AST généré pour l'expression '{}' est: {:?}",
        expression, ast
    );

    Ok(ast::eval(ast)?)
}
