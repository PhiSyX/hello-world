use SDLms::Window;

export struct Window {
	handle?: cxx!(SDL::SDL_Window),
}

export enum WindowError {
	FailToCreateWindowWithSDL(string) = "failed to create window: %s"
	FailToCreateRendererWithSDL(string) = "failed to create renderer: %s"
}

impl Window {
	export fn new(
		title: &string,
		width: u16, height: u16
	) -> Result<Window, WindowError> {
		return Window::new(
			title,
			SDL::SDL_WINDOWPOS_CENTERED, SDL::SDL_WINDOWPOS_CENTERED,
			width, height
		);
	}

	export fn new(
		title: &string,
		x: i32, y: i32,
		width: u16, height: u16
	) -> Result<Window, WindowError> {
		return Window::new(
			title,
			x, y,
			width, height,
			0
		);
	}

	export fn new(
		title: &string,
		x: i32, y: i32,
		width: u16, height: u16,
		flags: u32
	) -> Result<Window, WindowError> {
		let handle = cxx! {
			SDL::SDL_CreateWindow(
				c_str!(title),
				x, y,
				width, height,
				flags
			)
		};

		if handle.is_none() {
			let err = cxx! { SDL::SDL_GetError() };
			return Err(WindowError::FailToCreateWindowWithSDL(err));
		}

		return Ok(Window { handle });
	}

	export fn from(window: &mut Window) -> Window {
		let new_window = Window { handle: owned!(window.handle) };
		window.handle = None;
		return new_window;
	}
}

export fn create_renderer(&mut Window) -> Result<Renderer, WindowError> {
	assert!(self.handle);

	let renderer = cxx! { SDL::SDL_CreateRenderer(unwrap!(self.handle), -1, 0) };

	if renderer.is_none() {
		let err = cxx! { SDL::SDL_GetError() };
		return Err(WindowError::FailToCreateRendererWithSDL(err));
	}

	return Ok(Renderer::new(renderer));
}

@implicit
fn drop(&mut Window) {
	cxx!(SDL::SDL_DestroyWindow(unwrap!(self.handle)));
}
