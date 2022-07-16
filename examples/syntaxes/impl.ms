/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

struct MyStruct {
	a: string,
	b?: i32, // équivaut à Option<i32>. Les valeurs attendues sont None or Some(i32)
	c: Option<i32>, // équivaut à ce qui est écrit ci-haut
}

enum MyEnum {
	A,
	B(string),
	C { a: i32, b: i32 },
}

// Fonctions liées à ces structures de données

fn my_struct_fn(&MyStruct, a: i32, b: i32) -> i32 {
	return (a + b) + (self.a + self.b.unwrap_or_default());
}

fn my_enum_fn(&MyEnum, a: i32, b: i32) -> i32 {
	return match self {
		MyEnum::A => a,
		MyEnum::B(s) => (a + b) + (s.len() as i32),
		MyEnum::C { a, b } => a + b,
	};
}

// équivalence :

impl MyStruct {
	fn my_struct_fn2(&self, a: i32, b: i32) -> i32 {
		return (a + b) + (self.a + self.b.unwrap_or_default());
	}
}

impl MyEnum {
	fn my_enum_fn2(&self, a: i32, b: i32) -> i32 {
		return match self {
			MyEnum::A => a,
			MyEnum::B(s) => (a + b) + (s.len() as i32),
			MyEnum::C { a, b } => a + b,
		};
	}
}

fn main() {
	let my_struct = MyStruct { a: "Hello", b: Some(42), c: None };
	let n1 = my_struct.my_struct_fn(1, 2);
	let n2 = my_struct.my_struct_fn2(1, 2);
	println!("{n1} {n2}");

	let my_enum = MyEnum::B("Hello"); // "Hello" = 5
	let n3 = my_enum.my_enum_fn(1, 2); // (1 + 2 [= 3]) + 5 [= 8]
	assert!(n3 == 8);
	let n4 = my_enum.my_enum_fn2(1, 2);
	println!("{n4}");
}
