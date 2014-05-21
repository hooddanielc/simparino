#pragma once

#define GL_GLEXT_PROTOTYPES

#include <vector>
#include <iostream>
#include <map>
#include <memory>
#include <cassert>
#include <functional>
#include <fstream>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include "SDL.h"
#include "SDL_opengl.h"
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#endif

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

class Shaderino {
public:
    Shaderino();
    ~Shaderino();
    void compile(const char* filename, GLenum shaderType);
    void link();
    void use();
    std::vector<GLuint> shaders;
    GLuint id;
};
