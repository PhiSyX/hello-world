pub fn section_1() {
  let mut v: Vec<i32> = Vec::new();
  v.push(5);
  v.push(6);
  v.push(7);
  v.push(8);

  dbg!(v);

  let v = vec![1, 2, 3];

  {
    let v = vec![1, 2, 3, 4];
    dbg!(v);
  }

  dbg!(v);

  let mut v = vec![1, 2, 3, 4, 5];

  let third: &i32 = &v[2];
  println!("The third element is {}", third);

  match v.get(2) {
    Some(third/* &i32 */) => println!("The third element is {}", third),
    None => println!("There is no third element."),
  }

  for i in &v {
    dbg!(i);
  }

  for i in &mut v {
    *i += 50;

    dbg!(i);
  }

  let row = vec![
    SpreadsheetCell::Int(3),
    SpreadsheetCell::Text(String::from("blue")),
    SpreadsheetCell::Float(10.12),
  ];

  dbg!(row);
}

#[derive(Debug)]
enum SpreadsheetCell {
  Int(i32),
  Float(f64),
  Text(String),
}
