#pragma once

#include <iostream>
#include <string>
#include <GL/glew.h>

#include "Magick++.h"

class Texturino {
public:
    Magick::Blob blob;
    Magick::Image img;
    Texturino(const char *fname);
    GLuint getTexture();
};