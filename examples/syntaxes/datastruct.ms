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
