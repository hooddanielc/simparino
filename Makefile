release: src/main.cc src/apperino.cc
	g++ -Ilib -std=c++11 -o simparino src/main.cc src/apperino.cc src/worldowrino.cc lib/tinyobjloader/tiny_obj_loader.cc \
		-F /Library/Frameworks \
		-framework SDL2 \
		-framework OpenGL \
		-framework BulletDynamics \
		-framework BulletCollision \
		-framework LinearMath \
		-L/opt/local/lib -lGLEW