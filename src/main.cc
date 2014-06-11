#include "apperino.h"

class MyWindow : public Windowrino {
public:
    double lastTime;
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
    ) {
        lastTime = 0;
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

        texture_cube_builder.printToConsole();

        // create a shape
        auto shape = texture_cube_builder.makeShapodino();

        // make a shared shape and add it to the world
        auto shared_shape = std::make_shared<Shapodino>(shape);
        shared_shape->setModel(glm::translate(shared_shape->getModel(), glm::vec3(0.0, 1.0, 0.0)));
        world.addShapodino(shared_shape, false, 0.1);

        // make another shape
        auto shared_shape_2 = std::make_shared<Shapodino>(shape);
        shared_shape_2->setModel(glm::translate(shared_shape_2->getModel(), glm::vec3(1.0, 10.0, 0.0)));
        world.addShapodino(shared_shape_2, false, 0.1);

        // create a plane with hedge texture
        ShapodinoBuilder hedge_plane_builder(
            "res/test_objs/hedge-plane.obj",
            "res/test_objs/"
        );
        auto shape_hedge = hedge_plane_builder.makeShapodino();
        auto shared_shape_hedge = std::make_shared<Shapodino>(shape_hedge);
        world.addShapodino(shared_shape_hedge, true, 0);

        // translate shape down a couple units
        shared_shape_hedge->setModel(glm::translate(shared_shape_hedge->getModel(), glm::vec3(0.0, -1.0, 0.0)));

        // create a psychedelic monkey
        ShapodinoBuilder psychedelic_monkey_builder(
            "res/test_objs/psychedelicmonkey.obj",
            "res/test_objs/"
        ); 
        auto shape_monkey = psychedelic_monkey_builder.makeShapodino();
        auto my_monkey_shape = std::make_shared<Shapodino>(shape_monkey);
        my_monkey_shape->setModel(glm::translate(my_monkey_shape->getModel(), glm::vec3(0.0, 4.0, 0.0)));
        world.addShapodino(my_monkey_shape, false, 0.1);

        // step world with keypress
        on(SDL_KEYDOWN, [shared_shape](std::shared_ptr<Windowrino> win, const SDL_Event &event) {
            //win->world.stepSimulation(1, 1);
        });
    }
    void loop(double time) {
        // make background light purple
        glClearColor(.5, .5, .5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double timeSinceLast = time - lastTime;
        lastTime = time;
        world.stepSimulation(time, 1);
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
