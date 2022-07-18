/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

/// Mot-clé function
function myFunction() {}

/// Mot-clé func
func MyFunc() {}

/// Mot-clé fn
fn my_fn() {}

// Il n'y a pas de différence avec ces trois mots-clés.

/// Fonction avec des paramètres
fn my_fn_with_params(a: i32, b: i32) {
	println!("{a} + {b} = {}", a + b);
}

func MyFnWithParamsAndReturn(a: i32, b: i32) -> i32 {
	return a + b;
}

/// Fonction constante
const fn my_const_fn() {}

/// fonction exportable
export function myExportableFunction() {}
