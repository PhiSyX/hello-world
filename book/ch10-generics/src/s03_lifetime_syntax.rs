pub fn section_3() {
    lifetime();
    lifetime_2();
    lifetime_3();
    lifetime_4();
}

// fn _lifetime() {
//     {
//         let r: u32; // ---------+-- 'a
//                     //          |
//         {
//             //          |
//             let x: u32 = 5; // -+-- 'b  |
//             r = &x; //  |       |
//         } // -+       |
//           //          |
//         println!("r: {}", r); //          |
//     }

//     {
//         let x: u8 = 5; // ----------+-- 'b
//                        //           |
//         let r: &u8 = &x; // --+-- 'a  |
//                          //   |       |
//         println!("r: {}", r); //   |       |
//                               // --+       |
//     }
// }

fn lifetime() {
    let my_string = String::from("hello");
    let my_string_2 = String::from("world");

    let chosen = choice(&my_string, &my_string_2);
    println!("chosen: {}", chosen);
}

fn choice<'a>(a: &'a str, b: &'a str) -> &'a str {
    let c = "pwet";

    if a.len() == b.len() {
        c
    } else if a.len() > b.len() {
        a
    } else {
        b
    }
}

fn announce<'a, T>(value: &'a T)
where
    T: core::fmt::Display,
{
    println!("Yey {}!", value);
}

fn lifetime_2() {
    'x: {
        let num: u8 = 42;
        'y: {
            let num_ref /*: &'y u8*/ = &num;
            'z: {
                announce(num_ref);
            }
        }
    }
}

struct Editor {
    text: String,
}

impl Editor {
    fn new() -> Self {
        Self {
            text: String::new(),
        }
    }

    fn insert(&mut self, ch: char) {
        self.text.push(ch);
    }

    fn reset(&mut self) {
        self.text = String::new();
    }

    // bad
    fn get_text_string(&self) -> String {
        self.text.clone()
    }

    // bad
    // fn get_text_copy(&self) -> String {
    //   self.text // error
    // }

    // ok (but, we can do better)
    fn get_text_ref(&self) -> &String {
        &self.text
    }

    fn get_text<'a>(&'a self) -> &'a String {
        &self.text
    }
}

fn lifetime_3() {
    let mut editor = Editor::new();
    editor.insert('a');
    editor.insert('b');
    editor.insert('c');

    // let text = editor.get_text();
    // editor.insert('d'); // error (borrow checker)
    // editor.reset(); // error (borrow checker)
    // println!("editor.text: {}", text); // use after free. Not possible.

    let text = editor.get_text();
    println!("editor.text: {}", text);
    editor.insert('d'); // allowed
}

struct Car {
    model: String,
}

struct Person<'a> {
    car: Option<&'a mut Car>,
}

impl<'a> Person<'a> {
    fn new() -> Self {
        Self { car: None }
    }

    fn buy_car(&mut self, c: &'a mut Car) {
        self.car = Some(c);
    }

    fn trade_with<'b>(&mut self, other: &'b mut Person<'a>) {
        let tmp = other.car.take();

        other.car = self.car.take();
        self.car = tmp;
    }
}

fn lifetime_4() {
    let mut tesla = Car {
        model: String::from("Tesla"),
    };
    let mut audi = Car {
        model: String::from("Audi"),
    };

    let mut person_1 = Person::new();
    let mut person_2 = Person::new();

    {
        // let bmw = Car {
        //     model: String::from("BMW"),
        // };
        // person.buy_car(&bmw); // error

        person_1.buy_car(&mut tesla); // ok
    }

    person_2.buy_car(&mut audi);

    person_1.trade_with(&mut person_2);

    println!("Person 1: {}", person_1.car.unwrap().model);
    println!("Person 2: {}", person_2.car.unwrap().model);
}
