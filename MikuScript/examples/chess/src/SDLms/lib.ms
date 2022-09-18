export struct Lib;

export fn new() -> Lib {
	cxx!(SDL::SDL_Init(0));
	return Lib;
}

export fn poll_event(&Lib, event: &SDL::SDL_Event) -> bool {
	return cxx!(SDL::SDL_PollEvent(event)) > 0;
}

@implicit
fn drop(&mut Lib) {
	cxx!(SDL::SDL_Quit());
}
