#pragma once

#include <vector>
#include <map>
#include <GL/glew.h>

class Bufferino {
public:
    GLuint vao;
    std::map<int, GLuint> vbos;
    void addBuffer(int idx, GLfloat *data, int size);
    void enable();
    void disable();
    Bufferino();
    ~Bufferino();
};
