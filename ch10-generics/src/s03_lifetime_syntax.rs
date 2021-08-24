pub fn section_3() {
    lifetime();
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
