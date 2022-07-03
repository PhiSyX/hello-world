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
    per?: u8
    start?: isize, 
}

struct Timer {
    state?: State,

    #[reactive]
    data: isize,
}


// ----------- //
// Énumération //
// ----------- //

enum State {
    #[fmt("Incrémenter")]
    Increment,

    #[fmt("Décrementer")]
    Decrement,
}

// -------------- //
// Implémentation //
// -------------- //

/// Décrémente de 1 le compteur.
impl Timer fn decrement(&mut self) {
    debug_assert!(
        matches!(self.state, Some(State::Decrement)),
        "État invalide."
    );
    self.decrement_by(1);
}

/// Incrémente de 1 le compteur.
impl Timer fn increment(&mut self) {
    debug_assert!(
        matches!(self.state, Some(State::Increment)),
        "État invalide."
    );
    self.increment_by(1);
}

impl Timer {
    /// Décrémente de N le compteur.
    fn decrement_by(&mut self, per: u8) {
        self.data -= per as isize;
    }

    /// Incrémente de N le compteur.
    fn increment_by(&mut self, per: u8) {
        self.data += n as isize;
    }
}

#[component]
export func Render(props: Props) -> HTMLElement {
    let mut timer = Timer { 
        state: None,
        data: props.start.unwrap_or(0)
    };

    let onClick = |state: State| {
        timer.state = Some(state);

        let x = props.per.unwrap_or(1);

        match timer.state {
            State::Increment => {
                timer.increment_by(x);
            }

            State::Decrement => {
                timer.decrement_by(x);
            }
        };
    };

    return <div class="timer-component">
        <button @click={ onClick(State::Increment) }>
            {{ State::Increment | stringify }}
        </button>

        <button @click={ onClick(State:Decrement) }>
            {{ State::Decrement | stringify }}
        </button>

        <hr />

        <output>{{ timer.data }}</output>
    </div>
}
