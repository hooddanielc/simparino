#include "apperino.h"

/*
* Wrapper for SDL_Init  *
* * * * * * * * * * * * */
Apperino::Apperino(Uint32 flags) {
    if (SDL_Init(flags) < 0) {
        // TODO: Exception
        // SDL failed to initiate
    }
}

Windowrino Apperino::addWindow(
    const char *title,
    int x,
    int y,
    int w,
    int h,
    Uint32 flags
) {
    Windowrino win(
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

Apperino::~Apperino() {
    SDL_Quit();
}

/*
* Wrapper for SDL_Window  *
* * * * * * * * * * * * * */
Windowrino::Windowrino(){};
Windowrino::Windowrino(
    const char *title,
    int x,
    int y,
    int w,
    int h,
    Uint32 flags
) {
    // initialize window
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
        std::cout << "Could not create window" << std::endl;
    }

    std::cout << "created" << std::endl;
}

Windowrino::~Windowrino() {
    SDL_DestroyWindow(win);
    std::cout << "DESTROYED" << std::endl;
}
