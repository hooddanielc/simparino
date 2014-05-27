#pragma once

#define GLM_FORCE_RADIANS

#include <tinyobjloader/tiny_obj_loader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <BulletDynamics/btBulletDynamicsCommon.h>
#include <iostream>
#include <GL/glew.h>

#include "apperino.h"

class Worldowrino {

public:
    Worldowrino();
    ~Worldowrino();
private:
    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *dynamicsWorld;
};

class Camerino {
public:
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;
    glm::mat4 mvp;
    Camerino();
    ~Camerino();
};

class Shapodino {
public:
    Shapodino(const char *fname);
    ~Shapodino();
    void printToConsole();
    std::vector<tinyobj::shape_t> shapes;
    std::vector<float> getMesh();
};

class Bufferino {
public:
    GLuint vao;
    std::map<int, GLuint> vbos;
    void addBuffer(int idx, GLfloat *data, int size);
    void bind();
    void disable();
    Bufferino();
    ~Bufferino();
};

class Shaderino {
public:
    Shaderino();
    ~Shaderino();
    void compile(const char* filename, GLenum shaderType);
    void link();
    void use();
    std::vector<GLuint> shaders;
    GLuint id;
};
