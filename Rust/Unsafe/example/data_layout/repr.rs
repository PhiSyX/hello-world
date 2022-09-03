use std::mem::size_of;

struct A {
    a: u8,
    b: u32,
    c: u16,
}

struct B {
    a: u8,
    _pad1: [u8; 3], // to align `b`
    b: u32,
    c: u16,
    _pad2: [u8; 2], // to make overall size multiple of 4
}

struct C {
    b: u32,
    c: u16,
    a: u8,
    _pad: u8,
}

struct Foo<T, U> {
    count: u16,
    data1: T,
    data2: U,
}

/*
struct Foo<u16, u32> {
    count: u16,
    data1: u16,
    data2: u32,
}

struct Foo<u32, u16> {
    count: u16,
    _pad1: u16,
    data1: u32,
    data2: u16,
    _pad2: u16,
}
*/

enum MyOption<T> {
    Some(T),
    None,
}

#[repr(u8)]
enum MyReprOption<T> {
    Some(T),
    None,
}

pub fn repr() {
    assert_eq!(8, dbg!(size_of::<MyOption<&u16>>()));
    assert_eq!(16, dbg!(size_of::<MyReprOption<&u16>>()));
}
