#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <memory>

class Windowrino {
public:
    Windowrino(
        const char *title,
        int x,
        int y,
        int w,
        int h,
        Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );
    ~Windowrino();
    Windowrino(const Windowrino &) = delete;
    Windowrino &operator=(const Windowrino &) = delete;
    friend class Apperino;
private:
    SDL_Window *win;
};

class Apperino {
public:
    Apperino(Uint32 flags = SDL_INIT_VIDEO);
    ~Apperino();
    Apperino(const Apperino &) = delete;
    Apperino &operator=(const Apperino &) = delete;
    std::shared_ptr<Windowrino> addWindow(
        const char *title,
        int x,
        int y,
        int w,
        int h,
        Uint32 flags
    );
private:
    std::vector<std::shared_ptr<Windowrino>> windows;
};