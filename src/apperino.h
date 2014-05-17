#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include <cassert>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

class Windowrino {
public:
    Windowrino(
        const char *title,
        int x,
        int y,
        int w,
        int h,
        Uint32 flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );
    ~Windowrino();
    Windowrino(const Windowrino &) = delete;
    Windowrino &operator=(const Windowrino &) = delete;
    void setPosition(int x, int y);
    void setSize(int w, int h);
    void setMaxSize(int w, int h);
    void setMinSize(int w, int h);
    void setFullscreen(Uint32 flags = SDL_WINDOW_FULLSCREEN_DESKTOP);
    void minimize();
    void maximize();
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
        Uint32 flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );
    static Apperino *get() {
        assert(theApperino != nullptr);
        return theApperino;
    }
private:
    std::vector<std::shared_ptr<Windowrino>> windows;
    static Apperino *theApperino;
};
