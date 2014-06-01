#include "bufferino.h"

/*
* For managing a Vertex Array Object
* with many Vertex Buffer Objects
* * * * * * * * * * * * * * * * * */
BufferSequerino::BufferSequerino() {
    // generate a vertex array object
    glGenVertexArrays(1, &vao);
}

void BufferSequerino::pushBuffer(std::shared_ptr<AnyBufferino> buff) {
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
    for(auto it = tbos.begin(); it != tbos.end(); ++it) {
        glActiveTexture(it->first);
        glBindTexture(GL_TEXTURE_2D, it->second);
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
