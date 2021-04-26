pub fn section_1() {
  let four = IpAddrKind::V4;
  let six = IpAddrKind::V6;

  route(four);
  route(six);

  let home = IpAddr {
    kind: IpAddrKind::V4,
    address: String::from("127.0.0.1"),
  };
  dbg!(home);

  let loopback = IpAddr {
    kind: IpAddrKind::V6,
    address: String::from("::1"),
  };
  dbg!(loopback);

  let home = IpAddrTuple::V4(127, 0, 0, 1);
  dbg!(home);

  let loopback = IpAddrTuple::V6(String::from("::1"));
  dbg!(loopback);

  let m = Message::Write(String::from("hello"));
  m.call();

}
#[derive(Debug)]
enum IpAddrKind {
  V4,
  V6,
}

#[derive(Debug)]
struct IpAddr {
  kind: IpAddrKind,
  address: String,
}

#[derive(Debug)]
enum IpAddrTuple {
  V4(u8, u8, u8, u8),
  V6(String),
}

fn route(ip_kind: IpAddrKind) {
  dbg!(ip_kind);
}


enum Message {
  Quit,
  Move { x: i32, y: i32 },
  Write(String),
  ChangeColor(i32, i32, i32),
}

struct QuitMessage; // unit struct
struct MoveMessage {
    x: i32,
    y: i32,
}
struct WriteMessage(String); // tuple struct
struct ChangeColorMessage(i32, i32, i32); // tuple struct

impl Message {
  fn call(&self) {
      // method body would be defined here
  }
}

