fn foo(x: &mut i32, y: &mut i32) -> i32 {
    *x = 42;
    *y = 7;
    *x
}

unsafe fn unsafe_bar(x: *mut i32, y: *mut i32) -> i32 {
    *x = 42;
    *y = 7;
    *x
}

fn main() {
    let mut n = 0;

    // println!("Output: {}", foo(&mut n, &mut n)); // <- ERROR.

    let n_ptr = &mut n as *mut i32;
    let n1 = unsafe { &mut *n_ptr };
    let n2 = unsafe { &mut *n_ptr };
    println!("Output 1: {}", foo(n1, n2));

    let mut n = 0;
    println!("Output 2: {}", unsafe { unsafe_bar(&mut n, &mut n) });
}

/*
$ cargo run

Output 1: 7
Output 2: 7
*/

/*
$ cargo run --release

Output 1: 42
Output 2: 7
*/
