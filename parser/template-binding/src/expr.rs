use core::fmt;
use std::collections::HashMap;

// ---- //
// Type //
// ---- //

type ContentData = HashMap<String, String>;

// --------- //
// Structure //
// --------- //

#[derive(Debug)]
#[derive(PartialEq)]
pub struct ExpressionData<'a> {
    input: &'a str,
    output: String,
}

#[derive(Debug)]
#[derive(PartialEq)]
pub struct Expression {
    var: Option<String>,
    output: String,
}

// ----------- //
// Énumération //
// ----------- //

#[derive(Debug)]
#[derive(PartialEq)]
pub enum Template<'a> {
    Literal(String),
    Variable(ExpressionData<'a>),
}

// -------------- //
// Implémentation //
// -------------- //

impl<'a> Template<'a> {
    pub fn new(input: &'a str, data: ContentData) -> Self {
        // let is_template_variable = check_matching_pair(input, "{{", "}}");
        // if is_template_variable {
        let content = ExpressionData::new(input, &data);
        Self::Variable(content)
        // } else {
        //     Self::Literal(input.to_owned())
        // }
    }
}

impl<'a> ExpressionData<'a> {
    pub fn new(input: &'a str, data: &ContentData) -> Self {
        let output = input
            .split_whitespace()
            .map(|w| Expression::new(w, data).to_string())
            .collect::<Vec<_>>()
            .join(" ");

        Self { input, output }
    }
}

impl Expression {
    fn new(input: &str, data: &ContentData) -> Self {
        let is_template_variable = check_matching_pair(input, "{{", "}}");

        if !is_template_variable {
            return Self {
                var: None,
                output: input.to_owned(),
            };
        }

        let (_, i) = get_index_symbol(input, '{');
        let (_, j) = get_index_symbol(input, '}');

        let var = &input[i + 1 + 1..j];

        let output = data
            .get(var)
            .map(|val| {
                let head = ExpressionData::new(&input[0..i], data);
                let tail = ExpressionData::new(&input[j + 1 + 1..], data);
                format!("{}{}{}", head, val, tail)
            })
            .unwrap_or_else(|| input.to_owned());

        Self {
            var: Some(var.to_owned()),
            output,
        }
    }
}

// -------------- //
// Implémentation // -> Interface
// -------------- //

impl fmt::Display for ExpressionData<'_> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.output)
    }
}

impl fmt::Display for Expression {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.output)
    }
}

// -------- //
// Fonction //
// -------- //

fn check_matching_pair(input: &str, s1: &str, s2: &str) -> bool {
    input.contains(s1) && input.contains(s2)
}

fn get_index_symbol(input: &str, symbol: char) -> (bool, usize) {
    let mut does_exist = false;
    let mut index = 0;

    for (idx, ch) in input.char_indices() {
        if ch == symbol {
            does_exist = true;
            index = idx;
            break;
        }
    }

    (does_exist, index)
}
