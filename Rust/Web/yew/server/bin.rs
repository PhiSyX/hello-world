use std::{
    collections::HashMap, convert::Infallible, marker::PhantomData,
};

use anyhow::Result;
use axum::{
    body::{Body, BoxBody},
    extract::Query,
    http::{Request, Response, StatusCode},
    response::{Html, IntoResponse},
    routing::{get, get_service, MethodRouter},
    Extension, Router,
};
use futures::{future::BoxFuture, ready};
use once_cell::sync::Lazy;
use tokio_util::task::LocalPoolHandle;
use tower::Service;
use tower_http::services::ServeDir;
use web_yew_client::{ServerApp, ServerAppProps};
use yew::AttrValue;
use yew_router::Routable;

lazy_static::lazy_static!(
    static ref INDEX_HTML_CONTENT: &'static str = {
        include_str!("../public/index.html")
    };
    static ref WASM_APP_URL: &'static str = {
        "/js/client_bg.wasm"
    };
    static ref JS_APP_URL: &'static str = {
        "/js/client.js"
    };
);

static LOCAL_POOL: Lazy<LocalPoolHandle> =
    Lazy::new(|| LocalPoolHandle::new(num_cpus::get()));

fn init_wasm_before_end_head_tag() -> String {
    format!(
        r#"
    <script src="{js_path}"></script>
    <script>
      window.addEventListener("load", () => wasm_bindgen("{wasm_path}"));
    </script>
"#,
        js_path = *JS_APP_URL,
        wasm_path = *WASM_APP_URL,
    )
}

async fn index_route(
    Extension(content): Extension<String>,
    req: Request<Body>,
    Query(queries): Query<HashMap<String, String>>,
) -> Html<String> {
    let output_ssr = LOCAL_POOL
        .spawn_pinned(|| async move {
            fn cb(
                url: AttrValue,
                queries: HashMap<String, String>,
            ) -> ServerAppProps {
                ServerAppProps { path: url, queries }
            }
            let mut out = String::new();
            yew::ServerRenderer::<ServerApp>::with_props(move || {
                cb(req.uri().path().to_owned().into(), queries)
            })
            .render_to_string(&mut out)
            .await;
            out
        })
        .await
        .unwrap();

    let output_html = content
        .replace(r#"<body>"#, &format!(r#"<body id="🆔">{}"#, output_ssr))
        .replace(
            "</head>",
            &format!("{}</head>", init_wasm_before_end_head_tag()),
        );

    Html(output_html)
}

async fn handle_error(e: impl std::fmt::Debug) -> impl IntoResponse {
    eprintln!("Erreur = {e:?}");
    StatusCode::BAD_REQUEST
}

#[tokio::main]
async fn main() -> Result<()> {
    let wasm_serve =
        get_service(ServeDir::new(".")).handle_error(handle_error);

    let public_dir_serve =
        get_service(ServeDir::new("public")).handle_error(handle_error);

    let route_service = RouteService::<web_yew_client::Route, _, _>::new(
        get(index_route),
        route(&format!("/public/{}", *JS_APP_URL), wasm_serve.clone())
            .route(&format!("/public/{}", *WASM_APP_URL), wasm_serve)
            .fallback(public_dir_serve),
    );

    let route_service = get_service(route_service)
        .layer(Extension(INDEX_HTML_CONTENT.to_owned()));

    let addr = "127.0.0.1:8080";
    println!("http://{}", addr);

    axum::Server::bind(&addr.parse()?)
        .serve(get_service(route_service).into_make_service())
        .await?;

    Ok(())
}

#[derive(Clone)]
struct RouteService<Route, Service, Future> {
    route: PhantomData<Route>,
    service_ready: bool,
    service: Service,
    future_ready: bool,
    future: Future,
}

impl<R, S: Clone, F: Clone> RouteService<R, S, F> {
    pub fn new(s: S, f: F) -> Self {
        Self {
            service: s,
            future: f,
            service_ready: false,
            future_ready: false,
            route: PhantomData,
        }
    }
}

impl<R, S, F> Service<Request<Body>> for RouteService<R, S, F>
where
    R: Routable,
    S: Service<Request<Body>, Error = Infallible> + Clone,
    S::Response: IntoResponse,
    S::Future: Send + 'static,
    F: Service<Request<Body>, Error = Infallible> + Clone,
    F::Response: IntoResponse,
    F::Future: Send + 'static,
{
    type Error = Infallible;
    type Future = BoxFuture<'static, Result<Self::Response, Self::Error>>;
    type Response = Response<BoxBody>;

    fn poll_ready(
        &mut self,
        cx: &mut std::task::Context<'_>,
    ) -> std::task::Poll<Result<(), Self::Error>> {
        loop {
            match (self.service_ready, self.future_ready) {
                | (true, true) => {
                    return Ok(()).into();
                }
                | (false, _) => {
                    ready!(self.service.poll_ready(cx))?;
                    self.service_ready = true;
                }
                | (_, false) => {
                    ready!(self.future.poll_ready(cx))?;
                    self.future_ready = true;
                }
            }
        }
    }

    fn call(&mut self, req: Request<Body>) -> Self::Future {
        if <R as Routable>::recognize(req.uri().path()).is_some() {
            self.service_ready = false;
            let maybe_req = self.service.call(req);
            return Box::pin(async move {
                let res = maybe_req.await?;
                Ok(res.into_response())
            });
        }

        self.future_ready = false;
        let maybe_req = self.future.call(req);
        Box::pin(async move {
            let res = maybe_req.await?;
            Ok(res.into_response())
        })
    }
}

fn route(path: &str, mr: MethodRouter) -> Router {
    Router::new().route(path, mr)
}
