#include "texturino.h"

using namespace Magick;

/*
* Helper class making opengl texturino's
* * * * * * * * * * * * * * * * * * * * */
Texturino::Texturino(const char *fname) : img(fname) {
    img.write(&blob, "RGBA");
}

GLuint Texturino::getTexture() {
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.columns(), img.rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
    glGenTextures(0, &tex);
    return tex;
}
