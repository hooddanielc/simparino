#include "apperino.h"

class MyWindow : public Windowrino {
public:
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
        // Compile a shader
        auto shader = std::make_shared<Shaderino>();
        shader->compile("shaders/test.vs", GL_VERTEX_SHADER);
        shader->compile("shaders/test.fs", GL_FRAGMENT_SHADER);
        shader->link();
        worldowrino.shaderinos["texture_shader"] = shader;

        shader->use();

        // Enable depth test
        glEnable(GL_DEPTH_TEST);

        Camerino camera;

        ShapodinoBuilder texture_cube_builder(
            "res/test_objs/textured_cube.obj",
            "res/test_objs/"
        );

        // set "MVP" uniform
        GLuint MatrixID = glGetUniformLocation(shader->id, "MVP");
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(camera.mvp));

        // Get "myTextureSampler" uniform
        GLuint textureunit  = glGetUniformLocation(shader->id, "texture0");
        glUniform1i(textureunit, 0);

        // make a shape and draw
        auto shape = texture_cube_builder.makeShapodino();
        glClearColor(0.5, 0.5, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shape.draw();

        // swap buffers
        swap();
    }
    void loop(double time) {
        // this is where we want to draw out world
        //std::cout << "the time is: " << time << std::endl;
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
