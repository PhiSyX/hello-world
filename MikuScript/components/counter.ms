/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

use mikuscript::event::ui::MouseEvent;
use mikuscript::dom::element::HTMLElement;

// --------- //
// Structure //
// --------- //

/// Propriétés du composant.
struct Props {
	per?: u8, // `?` avant le `:` signifie que la valeur de la propriété est
	          // optionnelle.
	start: Option<isize>, // <- Cela équivaut à faire ceci.
}

struct Counter {
	state?: State,

	#[reactive] // <- Dès que la propriété `data` change, le composant est
	            // re-rendu.
	data: isize,
}


// ----------- //
// Énumération //
// ----------- //

enum State {
	Increment = "Incrémenter",
	Decrement = "Décrementer",
}

// -------------- //
// Implémentation //
// -------------- //

/// Incrémente de 1 le compteur.
fn increment(&mut Counter) {
	assert!(
		matches!(self.state, Some(State::Increment)),
		"État invalide."
	);
	self.increment_by(1);
}

// Les deux façons de déclarer des méthodes liées à une structure de données
// ci-dessous sont valides dans MikuScript.
//
// @example ```ms
// let mut counter = Counter {...};
// counter.decrement();       // <--------------------------.
// counter.decrement_by(10);  // <-----------------------.  |
// ```                                                   |  |
/// Décrémente de 1 le compteur.                         |  |
fn decrement(&mut Counter) { // <------------------------+--'
	assert!(                                          /* | */
		matches!(self.state, Some(State::Decrement)), /* | */
		"État invalide."                              /* | */
	);                                                /* | */
	self.decrement_by(1);                             /* | */
}                                                     /* | */
													  /* | */
impl Counter {                                        /* | */
	/// Décrémente de N le compteur.                     |
	fn decrement_by(&mut self, per: u8) { // <-----------'
		self.data -= per as isize;
	}

	/// Incrémente de N le compteur.
	fn increment_by(&mut self, per: u8) {
		self.data += n as isize;
	}
}

#[component<Counter>]
export func Render(props: Props) -> HTMLElement {
	let mut counter = Counter {
		state: None,
		data: unwrap_or! { props.start | 0 }
	};

	// Callback
	let onClick = |state: State| {
		counter.state = Some(state);

		let x = unwrap_or { props.per | 1 };

		match counter.state {
			State::Increment => {
				counter.increment_by(x);
			}

			State::Decrement => {
				counter.decrement_by(x);
			}
		};
	};

	return <div class="counter-component">
		<button @click={ onClick(State::Increment) }>
			{{ State::Increment | stringify }}
		</button>

		<button @click={ onClick(State:Decrement) }>
			{{ State::Decrement | stringify }}
		</button>

		<hr />

		<output>{{ counter.data }}</output>
	</div>
}
