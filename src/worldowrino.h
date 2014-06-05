#pragma once

#define GLM_FORCE_RADIANS

#include <iostream>
#include <map>
#include <vector>
#include <tinyobjloader/tiny_obj_loader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <BulletDynamics/btBulletDynamicsCommon.h>
#include <GL/glew.h>
#include <fstream>

#include "bufferino.h"

class Shapodino {
public:
    Shapodino() : modelMatrix(glm::mat4(1.0)) {}
    void pushBufferSequence(std::shared_ptr<BufferSequerino> bufferinoSequence);
    void draw();
private:
    glm::mat4 modelMatrix;
    std::vector<std::shared_ptr<BufferSequerino>> bufferinos;
};

class ShapodinoBuilder {
public:
    ShapodinoBuilder(const char *objfile, const char *mtlfile);
    void printToConsole();
    std::vector<tinyobj::shape_t> shapes;
    std::string mtldir;
    Shapodino makeShapodino();
};

class Shaderino {
public:
    Shaderino();
    ~Shaderino();
    void compile(const char* filename, GLenum shaderType);
    void link();
    void use();
    GLuint id;
private:
    std::string readfile(const char *path);
    std::vector<GLuint> shaders;
};

class Worldowrino {
public:
    Worldowrino();
    ~Worldowrino();
    void draw(const char *shaderName);
    void resize(float width, float height);
    std::map<std::string, std::shared_ptr<Shaderino>> shaderinos;
private:
    glm::mat4 projection;
    std::vector<Shapodino> shapodinos;
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
};