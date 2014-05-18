#pragma once

#include <vector>
#include <iostream>
#include <map>
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
    static Apperino *get() {
        assert(theApperino != nullptr);
        return theApperino;
    }
    std::shared_ptr<Windowrino> openWindow(
        const char *title,
        int x,
        int y,
        int w,
        int h,
        Uint32 flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );
    void run();
    void quit();
    void on(Uint32 type, std::function<void (const SDL_Event &)> &&cb);
private:
    bool running = true;
    std::vector<std::shared_ptr<Windowrino>> windows;
    std::map<Uint32, std::function<void (const SDL_Event &)>> callbacks;
    static Apperino *theApperino;
};
