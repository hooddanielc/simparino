#include "apperino.h"

class MyWindow : public Windowrino {
public:
    std::shared_ptr<Shaderino> shader;
    MyWindow(const char *title,
        int x,
        int y,
        int w,
        int h,
        Uint32 flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    ) : Windowrino(
        title,
        x,
        y,
        w,
        h,
        flags
    ){
        // Enable opengl stuff
        glEnable(GL_DEPTH_TEST);

        // Compile a shader
        shader = std::make_shared<Shaderino>();
        shader->compile("shaders/test.vs", GL_VERTEX_SHADER);
        shader->compile("shaders/test.fs", GL_FRAGMENT_SHADER);
        shader->link();
        shader->use();

        // create a builder that can build a shape
        ShapodinoBuilder texture_cube_builder(
            "res/test_objs/textured_cube.obj",
            "res/test_objs/"
        );

        // create a shape
        auto shape = texture_cube_builder.makeShapodino();

        // make a shared shape and add it to the world
        auto shared_shape = std::make_shared<Shapodino>(shape);
        world.addShapodino(shared_shape);

        // make another shape
        auto shared_shape_2 = std::make_shared<Shapodino>(shape);
        world.addShapodino(shared_shape_2);

        // create a hedge builder
        ShapodinoBuilder hedge_plane_builder(
            "res/test_objs/hedge-plane.obj",
            "res/test_objs/"
        );

        hedge_plane_builder.printToConsole();

        // create shape
        auto shape_hedge = hedge_plane_builder.makeShapodino();

        // get shared version
        auto shared_shape_hedge = std::make_shared<Shapodino>(shape_hedge);
        world.addShapodino(shared_shape_hedge);

        // translate shape down a couple units
        shared_shape_hedge->setModel(glm::translate(shared_shape_hedge->getModel(), glm::vec3(0.0, -1.0, 0.0)));

        // control the shape with arrow keys
        on(SDL_KEYDOWN, [shared_shape](std::shared_ptr<Windowrino> win, const SDL_Event &event) {
            if(event.key.keysym.sym == SDLK_LEFT) {
                shared_shape->setModel(glm::rotate(shared_shape->getModel(), .2f, glm::vec3(0.0, 1.0, 0.0)));
            } else if(event.key.keysym.sym == SDLK_RIGHT) {
                shared_shape->setModel(glm::rotate(shared_shape->getModel(), -.2f, glm::vec3(0.0, 1.0, 0.0)));
            } else if(event.key.keysym.sym == SDLK_UP) {
                shared_shape->setModel(glm::translate(shared_shape->getModel(), glm::vec3(0.0, 0.0, 1.0)));
            }else if(event.key.keysym.sym == SDLK_DOWN) {
                shared_shape->setModel(glm::translate(shared_shape->getModel(), glm::vec3(0.0, 0.0, -1.0)));
            }
        });
    }
    void loop(double time) {
        // make background light purple
        glClearColor(.5, .5, .5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw using our shader
        world.draw(shader);

        // swap buffers
        swap();
    }
};

int main(int argc, char *argv[]) {

    Apperino app;

    app.addWindow(std::make_shared<MyWindow>(
        "Window 1",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600
    ));

    app.on(SDL_KEYDOWN, [](const SDL_Event &event) {
        if(event.key.keysym.sym == SDLK_ESCAPE) {
            Apperino::get()->quit();
        }
    });

    app.on(SDL_QUIT, [](const SDL_Event &event) {
        Apperino::get()->quit();
    });

    app.run();

    return 0;
}
