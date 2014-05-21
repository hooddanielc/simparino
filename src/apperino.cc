#include "apperino.h"

/*
* Wrapper for SDL_Init  *
* * * * * * * * * * * * */
Apperino::Apperino(Uint32 flags) {
    assert(theApperino == nullptr);
    if (SDL_Init(flags) < 0) {
        // TODO: Exception
        // SDL failed to initiate
    }
    theApperino = this;
}

Apperino *Apperino::theApperino = nullptr;

std::shared_ptr<Windowrino> Apperino::openWindow(
    const char *title,
    int x,
    int y,
    int w,
    int h,
    Uint32 flags
) {
    auto win = std::make_shared<Windowrino>(
        title,
        x,
        y,
        w,
        h,
        flags
    );
    windowrinos[win->id] = win;
    return win;
}

void Apperino::on(Uint32 type, std::function<void (const SDL_Event &)> &&cb) {
    callbacks[type].push_back(std::move(cb));
}

void Apperino::on(Uint32 windowId, Uint32 type, std::function<void (std::shared_ptr<Windowrino>, const SDL_Event &)> &&cb) {
    windowCallbacks[windowId][type].push_back(std::move(cb));
}

void Apperino::run() {
    SDL_Event event;
    while(running) {
        SDL_Delay(1);
        while(SDL_PollEvent(&event)){
            auto iterCbs = callbacks.find(event.type);
            if(iterCbs != callbacks.end()) {
                for(auto it = iterCbs->second.begin(); it != iterCbs->second.end(); ++it) {
                    (*it)(event);
                }
            }
            auto iterWindowCbGroups = windowCallbacks.find(event.window.windowID);
            if(iterWindowCbGroups != windowCallbacks.end()) {
                auto iterWindowCbs = iterWindowCbGroups->second.find(event.type);
                if(iterWindowCbs != iterWindowCbGroups->second.end()) {
                    for(auto it = iterWindowCbs->second.begin(); it != iterWindowCbs->second.end(); ++it) {
                        (*it)(windowrinos[event.window.windowID], event);
                    }
                }
            }
        }
    }
}

std::string Apperino::readfile(const char *path) {
    std::string thepath = "res/";
    thepath += path;
    std::string contents;
    std::fstream fs;
    fs.open(thepath);
    if(fs.is_open()) {
        std::string line;
        while(std::getline(fs, line)) {
            contents += line + "\n";
        }
        fs.close();
    }
    return contents;
}

void Apperino::quit() {
    running = false;
}

Apperino::~Apperino() {
    assert(theApperino == this);
    SDL_Quit();
    theApperino = nullptr;
}

/*
* Wrapper for SDL_Window  *
* * * * * * * * * * * * * */
Windowrino::Windowrino(
    const char *title,
    int x,
    int y,
    int w,
    int h,
    Uint32 flags
) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    win = SDL_CreateWindow(
        title,
        x,
        y,
        w,
        h,
        flags
    );
    if(!win) {
        // TODO: exception
        // window creation failed
        return;
    }
    id = SDL_GetWindowID(win);
    ctx = SDL_GL_CreateContext(win);
}

void Windowrino::setPosition(int x, int y) {
    SDL_SetWindowPosition(win, x, y);
}

void Windowrino::setSize(int w, int h) {
    SDL_SetWindowSize(win, w, h);
}

void Windowrino::setFullscreen(Uint32 flags) {
    SDL_SetWindowFullscreen(win, flags);
}

void Windowrino::setMaxSize(int w, int h) {
    SDL_SetWindowMaximumSize(win, w, h);
}

void Windowrino::setMinSize(int w, int h) {
    SDL_SetWindowMinimumSize(win, w, h);
}

void Windowrino::minimize() {
    SDL_MinimizeWindow(win);
}

void Windowrino::maximize() {
    SDL_MaximizeWindow(win);
}

void Windowrino::swap() {
    SDL_GL_SwapWindow(win);
}

void Windowrino::on(Uint32 type, std::function<void (std::shared_ptr<Windowrino>, const SDL_Event &)> &&cb) {
    Apperino::get()->on(id, type, std::move(cb));
}

void Windowrino::makeCurrentCtx() {
    SDL_GL_MakeCurrent(win, ctx);
}

Windowrino::~Windowrino() {
    SDL_GL_DeleteContext(ctx);
    SDL_DestroyWindow(win);
}

ShaderProgram::ShaderProgram() {
    id = glCreateProgram();
}

void ShaderProgram::compile(const char *filename, GLenum shaderType) {
    GLuint shaderid = glCreateShader(GL_FRAGMENT_SHADER);
    // compile shader
    const char *source = Apperino::get()->readfile(filename).c_str();
    glShaderSource(shaderid, 1, &source, NULL);
    glCompileShader(shaderid);

    // check if it compiled
    GLint Result = GL_FALSE;
    int InfoLogLength;

    glGetShaderiv(shaderid, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(shaderid, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(shaderid, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
    shaders.push_back(shaderid);
}

void ShaderProgram::link() {
    for(auto itr = shaders.begin(); itr < shaders.end(); ++itr) {
        glAttachShader(id, *itr);
    }
    glLinkProgram(id);
    for(auto itr = shaders.begin(); itr < shaders.end(); ++itr) {
        glDeleteShader(*itr);
    }

    // check program
    GLint Result = GL_FALSE;
    int InfoLogLength;

    glGetProgramiv(id, GL_LINK_STATUS, &Result);
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage( fmax(InfoLogLength, int(1)) );
    glGetProgramInfoLog(id, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(id);
}
