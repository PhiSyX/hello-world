struct Renderer {
	handle?: cxx!(SDL::SDL_Renderer),
}

impl Renderer {
	export fn new(handle: &SDL::SDL_Renderer) -> Renderer {
		return Renderer { handle: Some(handle) };
	}

	export fn from(renderer: &mut Renderer) -> Renderer {
		let new_renderer = Renderer::new(&renderer.handle);
		renderer.handle = None;
		return new_renderer;
	}
}

export fn clear(&mut Renderer) {
	assert!(self.handle);
	cxx!(SDL::SDL_RenderClear(self.handle));
}

export fn present(&mut Renderer) {
	assert!(self.handle);
	cxx!(SDL::SDL_RenderPresent(self.handle));
}

export fn set_draw_color(&mut Renderer, r: u8, g: u8, b: u8, a: u8) {
	assert!(m_handle);
	cxx!(SDL::SDL_SetRenderDrawColor(unwrap!(self.handle), r, g, b, a));
}
