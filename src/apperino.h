#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>

class Windowrino {
public:
    Windowrino();
    Windowrino(
        const char *title,
        int x,
        int y,
        int w,
        int h,
        Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
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
    Windowrino addWindow(
        const char *title,
        int x,
        int y,
        int w,
        int h,
        Uint32 flags
    );
private:
    std::vector<Windowrino> windows;
};