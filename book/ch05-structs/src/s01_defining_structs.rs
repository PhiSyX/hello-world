#[derive(Debug)]
struct User {
  username: String,
  email: String,
  sign_in_count: u64,
  active: bool,
}

fn build_user(email: String, username: String) -> User {
  User {
    email,
    username,
    active: true,
    sign_in_count: 1,
  }
}

#[derive(Debug)]
struct Color(i32, i32, i32);

#[derive(Debug)]
struct Point(i32, i32, i32);


pub fn section_1() {
  let mut user1 = User {
    email: String::from("someone@example.com"),
    username: String::from("someusername123"),
    active: true,
    sign_in_count: 1,
  };

  user1.email = String::from("anotheremail@example.com");
  dbg!(user1);

  let user2 = build_user(
    String::from("someone@example.com"),
    String::from("someusername123"),
  );

  dbg!(&user2);

  let user3 = User {
    email: String::from("foo@bar"),
    username: String::from("foobar"),
    ..user2
  };

  dbg!(user3);

  let black = Color(0, 0, 0);
  let origin = Point(0, 0, 0);

  dbg!(black, origin);
}
