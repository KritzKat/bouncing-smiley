all: src/main.cpp
	g++ src/main.cpp -g -pedantic -o build/program -lSDL2 -lSDL2_image