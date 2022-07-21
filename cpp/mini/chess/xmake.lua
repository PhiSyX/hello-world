add_requires("libsdl")
add_rules("mode.release", "mode.debug")

set_languages("c++17")

add_includedirs("src")

target("ChessGame")
    add_headerfiles("src/**.hxx")
    add_files("src/**.cxx")
    add_packages("libsdl")
