import SDLms;

use SDLms::Lib;
use SDLms::Window;

fn main() -> Result<()> {
	let sdl = Lib::new();

	let window = Window::new("ChessGame", 1280, 720)?;
	let mut renderer = window.create_renderer()?;

	let mut opened = true;

	while (opened) {
		let event = cxx! { SDL::SDL_Event };

		while (sdl.poll_event(&event)) {
			match (event.type) {
				| SDL::SDL_QUIT => {
					opened = false;
				}
			}
		}

		renderer.set_draw_color(128, 128, 255, 255);
		renderer.clear();
		renderer.present();
	}

	return Ok(());
}
