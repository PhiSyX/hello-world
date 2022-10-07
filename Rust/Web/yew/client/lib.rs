use std::collections::HashMap;

use yew::{function_component, html, prelude::*};
use yew_router::{
    history::{AnyHistory, History, MemoryHistory},
    prelude::*,
};

// --------- //
// Structure //
// --------- //

#[derive(Debug, Default)]
#[derive(PartialEq, Eq)]
#[derive(Properties)]
pub struct ServerAppProps {
    pub path: AttrValue,
    pub queries: HashMap<String, String>,
}

// ----------- //
// Énumération //
// ----------- //

#[derive(Clone)]
#[derive(PartialEq, Eq)]
#[derive(Routable)]
pub enum Route {
    #[at("/")]
    Home,
    #[at("/first-page")]
    FirstPage,
    #[at("/second-page")]
    SecondPage,
}

// --------- //
// Composant //
// --------- //

#[function_component]
pub fn ServerApp(props: &ServerAppProps) -> Html {
    let history = AnyHistory::from(MemoryHistory::new());

    history
        .push_with_query(&*props.path, &props.queries)
        .unwrap();

    html! {
        <Router history={history}>
            <Switch<Route> render={render} />
        </Router>
    }
}

#[function_component]
pub fn App() -> Html {
    html! {
        <BrowserRouter>
            <Switch<Route> render={render} />
        </BrowserRouter>
    }
}

// ----- //
// Rendu //
// ----- //

fn render(route: Route) -> Html {
    let children = match route {
        | Route::Home => html! {
            <div>
                <p> { "Hello World"} </p>
                <a href="/first-page"> { "Aller à la page /first-page" } </a>
            </div>
        },
        | Route::FirstPage => first_page(),
        | Route::SecondPage => second_page(),
    };

    html! {
        <main>{children}</main>
    }
}

fn first_page() -> Html {
    html! {
        <section>
            <p> { "First page" } </p>

            <a href="/second-page"> { "Aller à la page /second-page" } </a>
        </section>
    }
}

fn second_page() -> Html {
    html! {
        <>
            <p> { "Second page"} </p>

            <a href="/"> { "Aller à l'accueil" } </a>

            <hr />

            <Counter />
        </>
    }
}

#[function_component]
fn Counter() -> Html {
    let s_counter = use_state(|| 0isize);

    let increment = {
        let s_counter = s_counter.clone();
        Callback::from(move |_| s_counter.set(*s_counter + 1))
    };

    let decrement = {
        let s_counter = s_counter.clone();
        Callback::from(move |_| s_counter.set(*s_counter - 1))
    };

    html! {
        <>
            <p> {"Compteur: "} {*s_counter} </p>

            <button onclick={increment}> {"+1"} </button>
            <button onclick={decrement}> {"-1"} </button>
        </>
    }
}
