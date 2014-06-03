#include "apperino.h"
#include "worldowrino.h"
#include "bufferino.h"

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
        
    }
    void loop(double time) {
        //std::cout << "the time is: " << time << std::endl;
    }
};

int main(int argc, char *argv[]) {

    Apperino app;

    auto world = std::make_shared<Worldowrino>();

    auto win1 = std::make_shared<MyWindow>(
        "Window 1",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600
    );

    app.addWindow(win1);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // create a "camera"
    Camerino camera;

    // Compile a shader
    Shaderino shader;
    shader.compile("shaders/test.vs", GL_VERTEX_SHADER);
    shader.compile("shaders/test.fs", GL_FRAGMENT_SHADER);
    shader.link();
    shader.use();

    ShapodinoBuilder texture_cube_builder(
        "res/test_objs/textured_cube.obj",
        "res/test_objs/"
    );

    // set "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(shader.id, "MVP");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(camera.mvp));

    // Get "myTextureSampler" uniform
    GLuint textureunit  = glGetUniformLocation(shader.id, "texture0");
    glUniform1i(textureunit, 0);

    // make a shape and draw
    auto shape = texture_cube_builder.makeShapodino();
    glClearColor(0.5, 0.5, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shape.draw();

    // swap buffers
    win1->swap();

    app.on(SDL_KEYDOWN, [](const SDL_Event &event) {
        if(event.key.keysym.sym == SDLK_ESCAPE) {
            Apperino::get()->quit();
        }
        if(event.key.keysym.sym == SDLK_0) {
            auto f = Apperino::get()->readfile("teapot.obj");
            std::cout << f << std::endl;
        }
    });

    app.on(SDL_QUIT, [](const SDL_Event &event) {
        Apperino::get()->quit();
    });

    app.run();

    return 0;
}
