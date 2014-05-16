release: src/main.cc src/apperino.cc
	g++ -o simparino src/main.cc src/apperino.cc -F /Library/Frameworks -framework SDL2 -framework OpenGL