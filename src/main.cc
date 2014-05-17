#include "apperino.h"

/* Our program's entry point */
int main(int argc, char *argv[]) {

    Apperino app;

    auto win = app.addWindow(
        "Window 1",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        512,
        512
    );

    win->setMinSize(800, 600);

    // event handling
    SDL_Event event;
    int keep_goin = 1;
    while(keep_goin) {
        SDL_Delay(5);
        while(SDL_PollEvent(&event)){
            /* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
            switch(event.type){
              case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_0) {
                    keep_goin = 0;
                }
                break;
              case SDL_KEYUP:
                if(event.key.keysym.sym == SDLK_5) {
                    win->maximize();
                }
                if(event.key.keysym.sym == SDLK_6) {
                    win->minimize();
                }
                break;
              default:
                break;
            }
        }
    }

    return 0;
}
