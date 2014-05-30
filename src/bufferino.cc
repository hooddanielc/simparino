#include "bufferino.h"

/*
* For managing a Vertex Array Object
* with many Vertex Buffer Objects
* * * * * * * * * * * * * * * * * */
Bufferino::Bufferino() {
    // generate a vertex array object
    glGenVertexArrays(1, &vao);
}

void Bufferino::addBuffer(int idx, GLfloat *data, int size) {
    glBindVertexArray(vao);
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    vbos[idx] = vbo;
}

void Bufferino::enable() {
    for(auto it = vbos.begin(); it != vbos.end(); ++it) {
        glEnableVertexAttribArray(it->first);
        glBindBuffer(GL_ARRAY_BUFFER, it->second);
        glVertexAttribPointer(
           it->first,          // number must match the layout in the shader
           3,                  // size
           GL_FLOAT,           // type
           GL_FALSE,           // normalized?
           0,                  // stride
           (void*)0            // array buffer offset
        );
    }
}

void Bufferino::disable() {
    for(auto it = vbos.begin(); it != vbos.end(); ++it) {
        glDisableVertexAttribArray(it->first);
    }
}

Bufferino::~Bufferino() {
    // delete vertex array object
    glDeleteVertexArrays(1, &vao);
    // delete shaders
    for(auto it = vbos.begin(); it != vbos.end(); ++it) {
        glDeleteBuffers(1, &it->second);
    }
}