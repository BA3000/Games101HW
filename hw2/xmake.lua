add_rules("mode.debug", "mode.release")
add_requires("opencv", "eigen")
set_languages("cxx17")

target("hw2")
	set_kind("binary")
	add_packages("opencv", "eigen")
	add_files("src/*.cpp")
