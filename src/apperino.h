#pragma once
#include <SDL2/SDL.h>
#include <vector>

class Windowrino {
public:
    Windowrino(
        const char *title,
        int x,
        int y,
        int w,
        int h,
        Uint32 flags
    );
    ~Windowrino();
    friend class Apperino;
private:
    SDL_Window *win;
};

class Apperino {
public:
    Apperino(Uint32 flags = SDL_INIT_VIDEO);
    ~Apperino();
private:
    std::vector<Windowrino> windows;
};