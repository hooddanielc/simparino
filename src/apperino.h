#pragma once

#include "worldowrino.h"

#include <vector>
#include <iostream>
#include <map>
#include <memory>
#include <cassert>
#include <functional>
#include <fstream>
#include <string>

#define GL_GLEXT_PROTOTYPES
#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "Magick++.h"

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
    Uint32 id;
    void setPosition(int x, int y);
    void setSize(int w, int h);
    void setMaxSize(int w, int h);
    void setMinSize(int w, int h);
    void setFullscreen(Uint32 flags = SDL_WINDOW_FULLSCREEN_DESKTOP);
    void minimize();
    void maximize();
    void swap();
    void makeCurrentCtx();
    void on(Uint32 type, std::function<void (std::shared_ptr<Windowrino>, const SDL_Event &)> &&cb);
    virtual void loop(double time){}
    Worldowrino world;
private:
    SDL_Window *win;
    SDL_GLContext ctx;
};

class Apperino {
friend void Windowrino::on(Uint32 type, std::function<void(std::shared_ptr<Windowrino>, const SDL_Event &)> &&cb);
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
    void addWindow(std::shared_ptr<Windowrino>);
    void run();
    void quit();
    void on(Uint32 type, std::function<void (const SDL_Event &)> &&cb);
    std::string readfile(const char *path);
private:
    bool running = true;
    std::map<Uint32, std::shared_ptr<Windowrino>> windowrinos;
    std::map<Uint32, std::vector<std::function<void (const SDL_Event &)>>> callbacks;
    std::map<Uint32, std::map<Uint32, std::vector<std::function<void (std::shared_ptr<Windowrino>, const SDL_Event &)>>>> windowCallbacks;
    void on(Uint32 windowId, Uint32 type, std::function<void (std::shared_ptr<Windowrino>, const SDL_Event &)> &&cb);
    static Apperino *theApperino;
};
