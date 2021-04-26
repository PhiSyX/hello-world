use std::collections::HashMap;

pub fn section_3() {
  let mut scores = HashMap::new();

  scores.insert(String::from("Blue"), 10);
  scores.insert(String::from("Yellow"), 50);

  dbg!(scores);

  let teams = vec![
    String::from("Blue"),
    String::from("Yellow")
  ];
  let initial_scores = vec![10, 50];

  let scores: HashMap<_, _> = teams
    .into_iter()
    .zip(initial_scores.into_iter())
    .collect();

  dbg!(scores);

  // Que se passe-t-il si un tableau est plus grand que l'autre ?

  let test_1 = vec![
    String::from("#1"),
    String::from("#2"),
  ];
  let test_1_1 = vec![
    String::from("Italie"),
    String::from("Belgique"),
    String::from("France"),
  ];
  let test: HashMap<_, _> = test_1
    .into_iter()
    .zip(test_1_1.into_iter())
    .collect();

  dbg!(test); //? intéressant

  let field_name = String::from("Favorite color");
  let field_value = String::from("Blue");

  let mut map = HashMap::new();
  map.insert(field_name, field_value);
  // field_name and field_value are invalid at this point, try using them and
  // see what compiler error you get!
  dbg!(map);

  let mut scores = HashMap::new();

  scores.insert(String::from("Blue"), 10);
  scores.insert(String::from("Yellow"), 50);

  let team_name = String::from("Blue");
  let score = scores.get(&team_name);

  dbg!(score);

  let mut scores = HashMap::new();

  scores.insert(String::from("Blue"), 10);
  scores.insert(String::from("Yellow"), 50);

  for (key, value) in &scores {
    println!("{}: {}", key, value);
  }

  let mut scores = HashMap::new();
  scores.insert(String::from("Blue"), 10);

  scores.entry(String::from("Yellow")).or_insert(50);
  scores.entry(String::from("Blue")).or_insert(50);

  println!("{:?}", scores);

  let text = "hello world wonderful world";

  let mut map = HashMap::new();

  for word in text.split_whitespace() {
    let count = map.entry(word).or_insert(0);
    *count += 1;
  }

  println!("{:?}", map);

}
