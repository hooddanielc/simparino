#include "bufferino.h"

/*
* For managing a Vertex Array Object
* with many Vertex Buffer Objects
* * * * * * * * * * * * * * * * * */
BufferSequerino::BufferSequerino() {
    // generate a vertex array object
    glGenVertexArrays(1, &vao);
}

void BufferSequerino::pushBuffer(std::shared_ptr<AnyBufferino> &buff) {
    vbos.push_back(buff);
}

void BufferSequerino::pushTexture(GLenum textureUnit, GLuint textureId) {
    tbos[textureUnit] = textureId;
}

void BufferSequerino::build() {
    glBindVertexArray(vao);
    int idx = 0;
    for(auto it = vbos.begin(); it != vbos.end(); ++it, ++idx) {
        glEnableVertexAttribArray(idx);
        glBindBuffer(GL_ARRAY_BUFFER, (*it)->getId());
        glVertexAttribPointer(
           idx,
           (*it)->getColumns(),
           GL_FLOAT,
           GL_FALSE,
           0,
           (void*)0
        );
    }
    for(auto it = tbos.begin(); it != tbos.end(); ++it) {
        glActiveTexture(it->first);
        glBindTexture(GL_TEXTURE_2D, it->second);
    }
    glBindVertexArray(0);
}

void BufferSequerino::bind() {
    glBindVertexArray(vao);
}



void BufferSequerino::addBuffer(int idx, GLfloat *data, int size) {
    glBindVertexArray(vao);
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    somevbos[idx] = vbo;
}

void BufferSequerino::enable() {
    glBindVertexArray(vao);
    for(auto it = somevbos.begin(); it != somevbos.end(); ++it) {
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

BufferSequerino::~BufferSequerino() {
    // delete vertex array object
    glDeleteVertexArrays(1, &vao);
    // delete buffers
    for(auto it = somevbos.begin(); it != somevbos.end(); ++it) {
        glDeleteBuffers(1, &it->second);
    }
}
