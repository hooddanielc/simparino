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

void BufferSequerino::pushTexture(GLenum textureUnit, std::shared_ptr<TextureBufferino> texture) {
    tbos[textureUnit] = texture;
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
        glBindTexture(GL_TEXTURE_2D, it->second->getId());
    }
    glBindVertexArray(0);
}

void BufferSequerino::bind() {
    glBindVertexArray(vao);
    for(auto it = tbos.begin(); it != tbos.end(); ++it) {
        glActiveTexture(it->first);
        glBindTexture(GL_TEXTURE_2D, it->second->getId());
    }
}

BufferSequerino::~BufferSequerino() {
    // delete vertex array object
    glDeleteVertexArrays(1, &vao);
}

/*
* Helper class making opengl texturino's
* * * * * * * * * * * * * * * * * * * * */
TextureBufferino::TextureBufferino(const char *fname) : relativepath(fname) {
    Magick::Image image(fname);
    image.write(&blob, "RGBA");
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.columns(), image.rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
}

GLuint TextureBufferino::getId() {
    return textureId;
}

TextureBufferino::~TextureBufferino() {
    glDeleteTextures(1, &textureId);
}

std::shared_ptr<TextureBufferino> MakeTextureBufferino(const char *fname) {
    return std::make_shared<TextureBufferino>(fname);
}