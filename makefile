test: example
	@./example

example: example.cpp gl_video.cpp
	@g++ -O2 example.cpp -o example -lsfml-system -lsfml-window -lsfml-graphics -lGL