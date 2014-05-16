#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "apperino.h"

/* A simple function that prints a message, the error code returned by SDL,
 * and quits the application */
void sdldie(const char *msg) {
    printf("%s: %s\n", msg, SDL_GetError());
    SDL_Quit();
    exit(1);
}
 
/* Our program's entry point */
int main(int argc, char *argv[]) {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        sdldie("Unable to initialize SDL");
    }

    Windowrino win("So Bored",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        512,
        512,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );


    // event handling
    SDL_Event event;
    /* Poll for events. SDL_PollEvent() returns 0 when there are no  */
    /* more events on the event queue, our while loop will exit when */
    /* that occurs.                                                  */
    int keep_goin = 1;
    while(keep_goin) {
        while( SDL_PollEvent( &event ) ){
            /* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
            switch( event.type ){
              case SDL_KEYDOWN:
                keep_goin = 0;
                break;
              case SDL_KEYUP:
                keep_goin = 0;
                break;
              default:
                break;
            }
        }
    }

    // quit SDL
    SDL_Quit();

    return 0;
}