#include "apperino.h"

/*
* Wrapper for SDL_Init  *
* * * * * * * * * * * * */
Apperino::Apperino(Uint32 flags) {
    assert(theApperino == nullptr);
    if (SDL_Init(flags) < 0) {
        // TODO: Exception
        // SDL failed to initiate
    }
    theApperino = this;
}

Apperino *Apperino::theApperino = nullptr;

std::shared_ptr<Windowrino> Apperino::openWindow(
    const char *title,
    int x,
    int y,
    int w,
    int h,
    Uint32 flags
) {
    auto win = std::make_shared<Windowrino>(
        title,
        x,
        y,
        w,
        h,
        flags
    );
    windows.push_back(win);

    return win;
}

void Apperino::on(Uint32 type, std::function<void (const SDL_Event &)> &&cb) {
    callbacks[type] = std::move(cb);
}

void Apperino::run() {
    SDL_Event event;
    while(running) {
        SDL_Delay(1);
        while(SDL_PollEvent(&event)){
            auto iter = callbacks.find(event.type);
            if (iter != callbacks.end()) {
                (iter->second)(event);
            }
        }
    }
}

void Apperino::quit() {
    running = false;
}

Apperino::~Apperino() {
    assert(theApperino == this);
    SDL_Quit();
    theApperino = nullptr;
}

/*
* Wrapper for SDL_Window  *
* * * * * * * * * * * * * */
Windowrino::Windowrino(
    const char *title,
    int x,
    int y,
    int w,
    int h,
    Uint32 flags
) {
    win = SDL_CreateWindow(
        title,
        x,
        y,
        w,
        h,
        flags
    );
    if(!win) {
        // TODO: exception
        // window creation failed
    }
}

void Windowrino::setPosition(int x, int y) {
    SDL_SetWindowPosition(win, x, y);
}

void Windowrino::setSize(int w, int h) {
    SDL_SetWindowSize(win, w, h);
}

void Windowrino::setFullscreen(Uint32 flags) {
    SDL_SetWindowFullscreen(win, flags);
}

void Windowrino::setMaxSize(int w, int h) {
    SDL_SetWindowMaximumSize(win, w, h);
}

void Windowrino::setMinSize(int w, int h) {
    SDL_SetWindowMinimumSize(win, w, h);
}

void Windowrino::minimize() {
    SDL_MinimizeWindow(win);
}

void Windowrino::maximize() {
    SDL_MaximizeWindow(win);
}

Windowrino::~Windowrino() {
    SDL_DestroyWindow(win);
}
