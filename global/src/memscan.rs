static STATIC: i32 = 1000;

fn noop() -> *const i32 {
    let noop_local = 12345;
    &noop_local as *const i32
}

pub fn memscan() {
    let local_str = "a";
    let local_int = 42;
    let boxed_str = Box::new('b');
    let boxed_int = Box::new(42);
    let fn_int = noop();

    println!("STATIC:    {:p}", &STATIC as *const i32);
    println!("local str: {:p}", local_str as *const str);
    println!("local int: {:p}", &local_int as *const i32);
    println!("boxed int: {:p}", Box::into_raw(boxed_int));
    println!("boxed str: {:p}", Box::into_raw(boxed_str));
    println!("fn_int:    {:p}", fn_int);
}
