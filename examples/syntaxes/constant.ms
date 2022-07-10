/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

// --------- //
// Constante //
// --------- //

const FOO: string = "foo";

const FOOBAR: string = FOO + "bar";

const fn foobar() -> string {
	return "foobar";
}

fn main() {
	const foobar_1: string = foobar();

	// On peut également utiliser `let`.
	let foobar_2 = foobar();
}

// ---- //
// Test //
// ---- //

#[test] Scenario {
	@input ```
		const FOO: string = "foo";
	```

	Lorsque l'entrée est analysée lexicalement
	Alors le prochain jeton DOIT être un jeton de type "<keyword-const-token>"
	Et le prochain jeton DOIT être un jeton de type "<ident-token>"
	Et le prochain jeton DOIT être un jeton de type "<colon-token>"
	Et le prochain jeton DOIT être un jeton de type "<ident-token>"
	Et le prochain jeton DOIT être un jeton de type "<equal-token>"
	Et le prochain jeton DOIT être un jeton de type "<literal-string-token>"
	Et le prochain jeton DOIT être un jeton de type "<semicolon-token>"
}

#[test] Scenario {
	@input ```
		const FOO;
	```

	Lorsque l'entrée est analysée lexicalement
	Alors il s'agit d'une erreur d'analyse avec comme raison
		"Un jeton <colon-token> est attendu après l'identifiant (<ident-token>) d'une constante."
}

#[test] Scenario {
	@input ```
		const FOO: string = "foo";
	```

	Lorsque l'entrée est analysée syntaxiquement
	Alors une déclaration de constante est attendue
	Et l'identifiant DOIT être "FOO"
	Et le type DOIT être "string"
	Et l'expression DOIT être une chaine de caractères litérale avec comme donnée "foo"
}

#[test] Scenario {
	@input ```
		const FOO: string = "foo";
		const FOOBAR: string = FOO + "bar";
	```

	Lorsque l'entrée est interprétée
	Alors la constante {{ FOO }} DOIT avoir comme valeur "foo"
	Et la constante {{ FOOBAR }} DOIT avoir comme valeur "foobar"
}

#[test]
func test_is_foo() {
	assert!(FOO == "foo");

	let foo = "foo";
	assert!(FOO == foo);

	let bar = "bar";
	assert!(FOO != bar);
}

#[test]: the FOOBAR constant should be "foobar"
func test_is_foobar() {
	assert!(FOOBAR == "foobar");

	let foobar = "foobar";
	assert!(FOOBAR == foobar);

	let foobar_1 = foobar();
	const FOOBAR_2: string = foobar();
	assert!(foobar_1 == FOOBAR_2);
}
