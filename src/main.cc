#include <SDL2/SDL.h>
#include "apperino.h"

/* Our program's entry point */
int main(int argc, char *argv[]) {

    Apperino app;

    app.addWindow(
        "Window 1",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        512,
        512,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    app.addWindow(
        "Window 2",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        512,
        512,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    // event handling
    SDL_Event event;
    
    int keep_goin = 1;
    while(keep_goin) {
        while( SDL_PollEvent( &event ) ){
            /* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
            switch(event.type){
              case SDL_KEYDOWN:
                if(event.key.keysym.sym != SDLK_0) {
                    keep_goin = 0;
                }
                break;
              case SDL_KEYUP:
                if(event.key.keysym.sym == SDLK_0) {
                    std::cout << "I pressed zero :p" << std::endl;
                    
                }
                break;
              default:
                break;
            }
        }
    }

    return 0;
}