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
#include <string>

#include "bufferino.h"

class Shapodino {
friend class ShapodinoBuilder;
friend class Worldowrino;
public:
    Shapodino();
    void pushBufferSequence(std::shared_ptr<BufferSequerino> bufferinoSequence);
    void draw();
    void setModel(glm::mat4 modelMatrix);
    glm::mat4 getModel();
    void setRigidBody(btRigidBody *body);
    void updateMvpFromRigidBody();
private:
    void setMesh(btTriangleMesh *mesh);
    void setMotionState(btDefaultMotionState *bulletMotionState);
    void setCollisionShape(btCollisionShape *theCollisionShape);
    btCollisionShape* getCollisionShape();
    btCollisionShape* getStaticCollisionShape();
    btRigidBody *rigidBody;
    btCollisionShape *collisionShape;
    btDefaultMotionState *motionState;
    btTriangleMesh *bt_mesh;
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
    GLuint programid;
private:
    std::string readfile(const char *path);
    std::vector<GLuint> shaders;
};

class Camerino {
public:
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;
    glm::mat4 mvp;
    Camerino();
    glm::mat4 getMVP(const glm::mat4 &projection, const glm::mat4 &model);
};

class Worldowrino {
public:
    Worldowrino();
    ~Worldowrino();
    void draw(std::shared_ptr<Shaderino> shader);
    void resize(float width, float height);
    void addShapodino(std::shared_ptr<Shapodino> shape, bool staticShape, float mass);
    void stepSimulation(btScalar timeStep, int maxSubSteps = 1, btScalar fixedTimeStep = btScalar(1.)/btScalar(60.));
private:
    glm::mat4 projection;
    Camerino camerino;
    std::vector<std::shared_ptr<Shapodino>> shapodinos;
    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *dynamicsWorld;
};
