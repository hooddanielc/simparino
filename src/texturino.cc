#include "texturino.h"

using namespace Magick;

/*
* Helper class making opengl texturino's
* * * * * * * * * * * * * * * * * * * * */
Texturino::Texturino(const char *fname) : img(fname) {
    img.write(&blob, "RGBA");
    //glTexImage2D(m_textureTarget, 0, GL_RGBA, img.columns(), img.rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
}