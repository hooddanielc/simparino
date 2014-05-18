#include "apperino.h"

int main(int argc, char *argv[]) {

    Apperino app;

    auto win = app.openWindow(
        "Window 1",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600
    );

    app.on(SDL_KEYDOWN, [&app, &win](const SDL_Event &event) {
        glClearColor ( 0.0, 1.0, 0.0, 1.0 );
        glClear ( GL_COLOR_BUFFER_BIT );
        printf("Version=%s\n", glGetString(GL_VERSION));
        printf("Version=%s\n", glGetString(GL_VENDOR));
        printf("Version=%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
        win->swap();
        SDL_Delay(5000);
        app.quit();
    });

    app.run();

    return 0;
}
