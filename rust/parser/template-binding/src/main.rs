use std::{
    collections::HashMap,
    io::{self, BufRead},
};

use template_binding::expr::Template;

fn main() {
    let mut data = HashMap::new();
    data.insert(String::from("nom"), String::from("Mike"));
    data.insert(String::from("age"), String::from("24"));
    data.insert(String::from("tag_paragraph"), String::from("p"));

    println!(
        "Binding template: ajouter des données de variables à un texte"
    );
    println!("Les données disponibles: {:?}", &data);
    println!(
        "Par exemple: '<{{{{tag_paragraph}}}}>{{{{nom}}}} me donne {{{{age}}}} ans.</{{{{tag_paragraph}}}}>'"
    );
    println!();

    io::stdin()
        .lock()
        .lines()
        .filter_map(|line| line.ok())
        .for_each(|line| match Template::new(&line, data.clone()) {
            | Template::Variable(content) => println!("\t -> {}", content),
            | Template::Literal(text) => println!("\t -> {}", text),
        });
}
