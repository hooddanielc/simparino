#include "apperino.h"

const char *lol = "OMG";

/* Our program's entry point */
int main(int argc, char *argv[]) {

    Apperino app;

    auto win = app.openWindow(
        "Window 1",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        512,
        512
    );

    win->setMinSize(800, 600);

    app.on(SDL_KEYDOWN, [&app](const SDL_Event &event) {
        std::cout << "keydown" << std::endl;
        app.quit();
    });

    app.run();

    return 0;
}
