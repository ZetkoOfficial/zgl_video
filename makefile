test: example
	@./example

example: example.cpp zgl_video.hpp
	@g++ -O2 example.cpp -o example -lsfml-system -lsfml-window -lsfml-graphics -lGL -lpng -lpthread
