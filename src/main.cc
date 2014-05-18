#include "apperino.h"

int main(int argc, char *argv[]) {

    Apperino app;

    auto win1 = app.openWindow(
        "Window 1",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600
    );

    auto win2 = app.openWindow(
        "Window 2",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600
    );

    auto win3 = app.openWindow(
        "Window 3",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600
    );

    win1->on(SDL_KEYDOWN, [](std::shared_ptr<Windowrino> win, const SDL_Event &event) {
        win->makeCurrentCtx();
        glClearColor(0.0, 1.0, 0.0, 1.0);
        glClear (GL_COLOR_BUFFER_BIT);
        win->swap();
    });

    win2->on(SDL_KEYDOWN, [](std::shared_ptr<Windowrino> win, const SDL_Event &event) {
        win->makeCurrentCtx();
        glClearColor ( 1.0, 0.0, 0.0, 1.0 );
        glClear ( GL_COLOR_BUFFER_BIT );
        win->swap();
    });

    win3->on(SDL_KEYDOWN, [&app](std::shared_ptr<Windowrino> win, const SDL_Event &event) {
        app.quit();
    });

    app.run();

    return 0;
}
