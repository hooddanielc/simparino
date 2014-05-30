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

    win1->on(SDL_KEYDOWN, [](std::shared_ptr<Windowrino> win, const SDL_Event &event) {
        win->makeCurrentCtx();
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        win->swap();
    });

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // create a "camera"
    Camerino camera;

    // Load a teapot shape
    Shapodino teapot(
        "res/test_objs/textured_cube.obj",
        "res/test_objs/"
    );

    // get some coordinates from our .obj file
    auto positions = teapot.getMesh();
    auto uvs = teapot.getUvs();
    // Generate random colors
    std::vector<float> colors;
    for(auto i = 0; i < positions.size(); ++i) {
        colors.push_back((float) rand() / RAND_MAX);
    }

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
    GLuint textureunit  = glGetUniformLocation(shader.id, "myTextureSampler");
    glUniform1i(textureunit, 0);

    // Create a bufferino to feed data to shaderino
    Bufferino buffer;
    // in vec3 vertexPosition_modelspace; = posisitions.data()
    buffer.addBuffer(0, positions.data(), positions.size() * 4);
    // in vec3 vertexColor
    buffer.addBuffer(1, colors.data(), colors.size() * 4);

    // manually generate a texture
    Texturino tex("res/img/test-texture.png");
    GLuint texture = tex.getTexture();
    GLuint uvbo;
    glGenBuffers(1, &uvbo);
    glBindBuffer(GL_ARRAY_BUFFER, uvbo);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * 4, uvs.data(), GL_STATIC_DRAW);

    // enable our bufferino as well
    buffer.enable();

    // bind uv coords to layout=0
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, uvbo);
    glVertexAttribPointer(
       2,                  // number must match the layout in the shader
       2,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
       (void*)0            // array buffer offset
    );

    // enable our texture
    glBindTexture(GL_TEXTURE_2D, texture);

    // clear background color
    // light purple
    glClearColor(0.5, 0.5, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // draw vao
    glDrawArrays(GL_TRIANGLES, 0, positions.size()); // Starting from vertex 0; 3 vertices total -> 1 triangle
    
    buffer.disable();
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
