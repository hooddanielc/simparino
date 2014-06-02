release: src/main.cc src/apperino.cc
	g++ -Ilib -I/opt/local/include/GraphicsMagick -std=c++11 -o simparino src/main.cc src/apperino.cc src/worldowrino.cc src/bufferino.cc lib/tinyobjloader/tiny_obj_loader.cc \
		-F /Library/Frameworks \
		-framework SDL2 \
		-framework OpenGL \
		-framework BulletDynamics \
		-framework BulletCollision \
		-framework LinearMath \
		-L/opt/local/lib -lGLEW -lGraphicsMagick++ \