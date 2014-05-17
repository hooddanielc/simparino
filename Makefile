release: src/main.cc src/apperino.cc
	g++ -std=c++11 -o simparino src/main.cc src/apperino.cc -F /Library/Frameworks -framework SDL2 -framework OpenGL