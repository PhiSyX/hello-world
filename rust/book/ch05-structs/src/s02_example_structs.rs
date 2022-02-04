pub fn section_2() {
  let width1 = 30;
  let height1 = 50;

  println!(
    "The area of the rectangle is {} square pixels.",
    area(width1, height1)
  );

  let rect1 = (30, 50);

  println!(
    "The area of the rectangle is {} square pixels.",
    area_tuple(rect1)
  );

  let rect2 = Rectangle {
    width: 30,
    height: 50,
  };

  println!(
    "The area of the rectangle is {} square pixels.",
    area_rect(&rect2)
  );

  dbg!(&rect2);
}

fn area(width: u32, height: u32) -> u32 {
  width * height
}

fn area_tuple((w, h): (u32, u32)) -> u32 {
  w * h
}

#[derive(Debug)]
struct Rectangle {
  width: u32,
  height: u32,
}

fn area_rect(rect: &Rectangle) -> u32 {
  rect.width * rect.height
}
