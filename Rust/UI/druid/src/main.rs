use druid::{
    theme::{BACKGROUND_DARK, PLACEHOLDER_COLOR, WIDE_WIDGET_WIDTH},
    widget::{
        prelude::*, Button, Controller, CrossAxisAlignment, Flex, Label,
        LineBreaking, TextBox,
    },
    AppLauncher, Data, Lens, LensExt, WidgetExt, WindowDesc,
};

pub const GRID: f64 = 8.0;

#[derive(Clone, Debug, Data, Lens)]
pub struct Login {
    pub auth: Authentication,
}

#[derive(Clone, Debug, Data, Lens)]
pub struct Authentication {
    pub username: String,
    pub password: String,
}

struct Authenticate {
    _tab: Tab,
}

impl Authenticate {
    fn new(tab: Tab) -> Self {
        Self { _tab: tab }
    }
}

impl<W: Widget<State>> Controller<State, W> for Authenticate {}

type SubmitHandler = fn(&mut EventCtx, &mut String, &Env);

#[derive(Default)]
pub struct InputController {
    _on_submit: Option<SubmitHandler>,
}

impl Controller<String, TextBox<String>> for InputController {}

#[derive(Clone, Data, Lens)]
struct State {
    pub login: Login,
}

pub fn main() {
    let main_window = WindowDesc::new(build_root_widget())
        .title("my app")
        .window_size((500.0, 500.0));

    let initial_state: State = State {
        login: Login {
            auth: Authentication {
                username: String::new(),
                password: String::new(),
            },
        },
    };

    AppLauncher::with_window(main_window)
        .launch(initial_state)
        .expect("Failed to launch application");
}

fn build_root_widget() -> impl Widget<State> {
    login_setup_widget().background(BACKGROUND_DARK).expand()
}

fn login_setup_widget() -> impl Widget<State> {
    Flex::column()
        .must_fill_main_axis(true)
        .cross_axis_alignment(CrossAxisAlignment::Start)
        .with_spacer(grid(2.0))
        .with_child(
            Label::new("Veuillez vous connecter.")
                .with_line_break_mode(LineBreaking::WordWrap),
        )
        .with_spacer(grid(2.0))
        .with_child(
            Label::new("Bla bla bla bla")
                .with_text_color(PLACEHOLDER_COLOR)
                .with_line_break_mode(LineBreaking::WordWrap),
        )
        .with_spacer(grid(6.0))
        .with_child(login_tab_widget(Tab::FirstStep).expand_width())
        .padding(grid(4.0))
}

pub fn grid(m: f64) -> f64 {
    GRID * m
}

#[derive(Copy, Clone)]
enum Tab {
    FirstStep,
}

fn login_tab_widget(tab: Tab) -> impl Widget<State> {
    let mut col = Flex::column().cross_axis_alignment(match tab {
        | Tab::FirstStep => CrossAxisAlignment::Center,
    });

    col = col
        .with_child(
            TextBox::new()
                .with_placeholder("Nom d'utilisateur")
                .controller(InputController::default())
                .env_scope(|env, _| env.set(WIDE_WIDGET_WIDTH, grid(16.0)))
                .lens(
                    State::login
                        .then(Login::auth)
                        .then(Authentication::username),
                ),
        )
        .with_spacer(grid(1.0));

    col = col
        .with_child(
            TextBox::new()
                .with_placeholder("Mot de passe")
                .controller(InputController::default())
                .env_scope(|env, _| env.set(WIDE_WIDGET_WIDTH, grid(16.0)))
                .lens(
                    State::login
                        .then(Login::auth)
                        .then(Authentication::password),
                ),
        )
        .with_spacer(grid(1.0));

    col = col
        .with_child(
            Button::new(match &tab {
                | Tab::FirstStep => "Se connecter",
            })
            .on_click(|ctx, state: &mut State, _| {
                println!(
                    "Nom d'utilisateur: '{}' - Mot de passe: '{}'",
                    state.login.auth.username, state.login.auth.password,
                );
            }),
        )
        .with_spacer(grid(1.0));

    col = col.with_spacer(grid(3.0));
    col.controller(Authenticate::new(tab))
}
