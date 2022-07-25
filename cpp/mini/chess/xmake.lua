add_requires("libsdl", "libsdl_image")
add_rules("mode.release", "mode.debug")

set_languages("c++17")

add_includedirs("src")

set_rundir("bin")
set_targetdir("bin/$(plat)_$(arch)_$(mode)")

target("ChessGame")
	add_headerfiles("src/**.hxx")
	add_files("src/**.cxx")
	add_packages("libsdl", "libsdl_image")
