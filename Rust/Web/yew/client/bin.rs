fn main() {
    type WebApp = web_yew_client::App;
    yew::Renderer::<WebApp>::new().hydrate();
}
