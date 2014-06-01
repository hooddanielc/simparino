#include "apperino.h"
#include "worldowrino.h"
#include "texturino.h"
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
    ){}
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

    // set "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(shader.id, "MVP");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(camera.mvp));

    // Get "myTextureSampler" uniform
    GLuint textureunit  = glGetUniformLocation(shader.id, "texture0");
    glUniform1i(textureunit, 0);

    // create textured cube buffers
    Shapodino textured_cube(
        "res/test_objs/textured_cube.obj",
        "res/test_objs/"
    );
    auto texture_cub_size = textured_cube.getMesh().size();
    Texturino test_texture_debug("res/img/test-texture.png");

    // create textured psychedelic plane buffers
    Shapodino textured_plane(
        "res/test_objs/psychedelic_plane.obj",
        "res/test_objs/"
    );
    auto textured_plane_size = textured_plane.getMesh().size();
    Texturino test_texture_psychedelic("res/img/psychedelic.png");

    // create a psychedelic cube sequence
    BufferSequerino buffer_sequence_cube;
    buffer_sequence_cube.pushBuffer(MakeBufferino(
        textured_cube.getMesh(),
        3
    ));
    buffer_sequence_cube.pushBuffer(MakeBufferino(
        textured_cube.getUvs(),
        2
    ));
    buffer_sequence_cube.pushTexture(GL_TEXTURE0, test_texture_debug.getTexture());
    buffer_sequence_cube.build();

    // create psychedelic plane sequence
    BufferSequerino buffer_sequence_plane;
    buffer_sequence_plane.pushBuffer(MakeBufferino(
        textured_plane.getMesh(),
        3
    ));
    buffer_sequence_plane.pushBuffer(MakeBufferino(
        textured_plane.getUvs(),
        2
    ));
    buffer_sequence_plane.pushTexture(GL_TEXTURE0, test_texture_psychedelic.getTexture());
    buffer_sequence_plane.build();

    glClearColor(0.5, 0.5, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw our psychedelic_cube buffer sequence
    buffer_sequence_cube.bind();
    glDrawArrays(GL_TRIANGLES, 0, texture_cub_size);

    // draw our psychedelic_plane buffer sequence
    buffer_sequence_plane.bind();
    glDrawArrays(GL_TRIANGLES, 0, textured_plane_size);

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
