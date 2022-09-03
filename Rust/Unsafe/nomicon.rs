#![allow(dead_code)]

mod example;

use self::example::*;

fn main() {
    dbg!(index_lt(4, &[1, 2, 3, 4, 5])); //? Some(5)
    dbg!(index_le(5, &[1, 2, 3, 4, 5])); //? Some(91)

    exotic_sizes();
    repr();
}
