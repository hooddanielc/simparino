#pragma once

#define GLM_FORCE_RADIANS

#include <iostream>
#include <map>
#include <tinyobjloader/tiny_obj_loader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <BulletDynamics/btBulletDynamicsCommon.h>
#include <GL/glew.h>

#include "apperino.h"
#include "bufferino.h"

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
    std::map<std::string, std::shared_ptr<TextureBufferino>> texturinos;
};

class Camerino {
public:
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;
    glm::mat4 mvp;
    Camerino();
};

class Shapodino {
    void pushBufferSequence(size_t arrayLength, BufferSequerino bufferinoSequence);
    void draw();
    std::map<size_t, BufferSequerino> bufferinos;
};

class ShapodinoBuilder {
public:
    ShapodinoBuilder(const char *objfile, const char *mtlfile);
    void printToConsole();
    std::vector<tinyobj::shape_t> shapes;
    std::vector<float> getMesh();
    std::vector<float> getUvs();
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
