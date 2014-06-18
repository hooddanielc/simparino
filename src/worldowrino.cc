#include "worldowrino.h"

// Our world so beautifuldino
Worldowrino::Worldowrino() {
    // Build the broadphase
    broadphase = new btDbvtBroadphase();
    // Set up the collision configuration and dispatcher
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    // The actual physics solver
    solver = new btSequentialImpulseConstraintSolver;
    // The world.
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0,-10,0));
}

void Worldowrino::resize(float width, float height) {
    glViewport(0, 0, width, height);
    projection = glm::perspective(45.0f, width / height, 0.1f, 100.0f);
}

void Worldowrino::stepSimulation(btScalar timeStep, int maxSubSteps, btScalar fixedTimeStep) {
    dynamicsWorld->stepSimulation(timeStep, maxSubSteps, fixedTimeStep);
    // update all shapes models
    for(auto it = shapodinos.begin(); it < shapodinos.end(); ++it) {
        // update mvp from rigid body
        (*it)->updateMvpFromRigidBody();
    }
}

void Worldowrino::addShapodino(std::shared_ptr<Shapodino> shape, bool staticShape, float mass) {
    shapodinos.push_back(shape);

    // build shapodino motion state
    glm::mat4 m = shape->getModel();
    btTransform mytransform(
        btMatrix3x3(
            m[0][0], m[0][1], m[0][2],
            m[1][0], m[1][1], m[1][2],
            m[2][0], m[2][1], m[2][2]
        ),
        btVector3(m[3][0], m[3][1], m[3][2])
    );
    btDefaultMotionState *state = new btDefaultMotionState(mytransform);
    shape->setMotionState(state);
    if(staticShape) {
        btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(
            0,
            state,
            shape->getStaticCollisionShape(),
            btVector3(0,0,0)
        );
        groundRigidBodyCI.m_restitution = 0.05;
        groundRigidBodyCI.m_friction = 0.0;
        btRigidBody *groundRigidBody = new btRigidBody(groundRigidBodyCI);
        shape->setRigidBody(groundRigidBody);
        dynamicsWorld->addRigidBody(groundRigidBody);
    } else {
        btVector3 fallInertia(0,0,0);
        shape->getCollisionShape()->calculateLocalInertia(mass, fallInertia);
        btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, state, shape->getCollisionShape(), fallInertia);
        fallRigidBodyCI.m_restitution = 0.05;
        fallRigidBodyCI.m_friction = 20.0;
        fallRigidBodyCI.m_linearDamping = 0.5;
        fallRigidBodyCI.m_additionalAngularDampingThresholdSqr = 0.5;
        btRigidBody *fallRigidBody = new btRigidBody(fallRigidBodyCI);
        shape->setRigidBody(fallRigidBody);
        dynamicsWorld->addRigidBody(fallRigidBody);
    }
}

Worldowrino::~Worldowrino() {
    // Clean up the Bullet physicserino
    delete dynamicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
    delete broadphase;
}

void Worldowrino::draw(std::shared_ptr<Shaderino> shader) {
    // enable opengl stuff
    glEnable(GL_DEPTH_TEST);
    // use shader
    shader->use();
    // can only support one texture for now
    GLuint textureunit  = glGetUniformLocation(shader->programid, "texture0");
    glUniform1i(textureunit, 0);
    // get mvp uniform - we set this
    // for every shape
    GLuint MatrixID = glGetUniformLocation(shader->programid, "MVP");
    GLuint ModelMatrixID = glGetUniformLocation(shader->programid, "M");
    GLuint ViewMatrixID = glGetUniformLocation(shader->programid, "V");
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, glm::value_ptr(camerino.getView()));
    for(auto it = shapodinos.begin(); it < shapodinos.end(); ++it) {
        glm::mat4 mvp = camerino.getMVP(projection, (*it)->getModel());
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(mvp));
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, glm::value_ptr(camerino.getModel()));
        (*it)->draw();
    }
}

/*
* Represents a shape in our
* 3D world. Responsible for
* actually drawing a shape
* at a specific position
* * * * * * * * * * * * * */
Shapodino::Shapodino() : modelMatrix(glm::mat4(1.0)),
    collisionShape(nullptr),
    rigidBody(nullptr) {}

void Shapodino::updateMvpFromRigidBody() {
    btTransform trans;
    rigidBody->getMotionState()->getWorldTransform(trans);
    btScalar m[16];
    trans.getOpenGLMatrix(m);

    modelMatrix[0][0] = m[0];
    modelMatrix[0][1] = m[1];
    modelMatrix[0][2] = m[2];
    modelMatrix[0][3] = m[3];

    modelMatrix[1][0] = m[4];
    modelMatrix[1][1] = m[5];
    modelMatrix[1][2] = m[6];
    modelMatrix[1][3] = m[7];

    modelMatrix[2][0] = m[8];
    modelMatrix[2][1] = m[9];
    modelMatrix[2][2] = m[10];
    modelMatrix[2][3] = m[11];

    modelMatrix[3][0] = m[12];
    modelMatrix[3][1] = m[13];
    modelMatrix[3][2] = m[14];
    modelMatrix[3][3] = m[15];
}

void Shapodino::setRigidBody(btRigidBody *body) {
    rigidBody = body;
}

void Shapodino::setCollisionShape(btCollisionShape *theCollisionShape) {
    collisionShape = theCollisionShape;
}

void Shapodino::setMotionState(btDefaultMotionState *bulletMotionState) {
    motionState = bulletMotionState;
}

void Shapodino::pushBufferSequence(std::shared_ptr<BufferSequerino> bufferSequence) {
    bufferinos.push_back(bufferSequence);
}

void Shapodino::setModel(glm::mat4 model) {
    modelMatrix = model;
}

glm::mat4 Shapodino::getModel() {
    return modelMatrix;
}

void Shapodino::draw() {
    for(auto it = bufferinos.begin(); it != bufferinos.end(); ++it) {
        (*it)->bind();
        glDrawArrays(GL_TRIANGLES, 0, (*it)->getIndices());
    }
}

void Shapodino::setMesh(btTriangleMesh *mesh) {
    bt_mesh = mesh;
}

btCollisionShape* Shapodino::getCollisionShape() {
    if(collisionShape == nullptr) {
        collisionShape = new btConvexTriangleMeshShape(bt_mesh, true);
    }
    return collisionShape;
}

btCollisionShape* Shapodino::getStaticCollisionShape() {
    //if(collisionShape == nullptr) {
        collisionShape = new btBvhTriangleMeshShape(bt_mesh, false);
    //}
    return collisionShape;
}

/*
* Responsible for parsing
* an obj file and creating
* a Shapodino for drawing
* * * * * * * * * * * * * */
ShapodinoBuilder::ShapodinoBuilder(const char *objfile, const char *mtlfile) : mtldir(mtlfile) {
    std::string err = tinyobj::LoadObj(shapes, objfile, mtlfile);
    if(err.length()) {
        std::cout << "Shapodino Error: " << err << std::endl;
        assert(err.length() == 0);
    }
}

void ShapodinoBuilder::printToConsole() {
    std::cout << "# of shapes : " << shapes.size() << std::endl;
    for (size_t i = 0; i < shapes.size(); i++) {
        printf("shape[%ld].name = %s\n", i, shapes[i].name.c_str());
        printf("shape[%ld].indices: %ld\n", i, shapes[i].mesh.indices.size());
        assert((shapes[i].mesh.indices.size() % 3) == 0);
        for (size_t f = 0; f < shapes[i].mesh.indices.size(); f++) {
            printf("  idx[%ld] = %d\n", f, shapes[i].mesh.indices[f]);
        }

        printf("shape[%ld].vertices: %ld\n", i, shapes[i].mesh.positions.size());
        assert((shapes[i].mesh.positions.size() % 3) == 0);
        for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; ++v) {
            printf("  v[%ld] = (%f, %f, %f)\n", v,
            shapes[i].mesh.positions[3*v+0],
            shapes[i].mesh.positions[3*v+1],
            shapes[i].mesh.positions[3*v+2]);
        }
        for (size_t v = 0; v < shapes[i].mesh.texcoords.size() / 2; ++v) {
            printf("  vt[%ld] = (%f, %f)\n", v,
            shapes[i].mesh.texcoords[2*v+0],
            shapes[i].mesh.texcoords[2*v+1]
            );
        }
        for (size_t vn = 0; vn < shapes[i].mesh.normals.size() / 3; ++vn) {
            printf("  vn[%ld] = (%f, %f, %f)\n", vn,
            shapes[i].mesh.normals[3*vn+0],
            shapes[i].mesh.normals[3*vn+1],
            shapes[i].mesh.normals[3*vn+2]
            );
        }
        printf("shape[%ld].material.name = %s\n", i, shapes[i].material.name.c_str());
        printf("  material.Ka = (%f, %f ,%f)\n", shapes[i].material.ambient[0], shapes[i].material.ambient[1], shapes[i].material.ambient[2]);
        printf("  material.Kd = (%f, %f ,%f)\n", shapes[i].material.diffuse[0], shapes[i].material.diffuse[1], shapes[i].material.diffuse[2]);
        printf("  material.Ks = (%f, %f ,%f)\n", shapes[i].material.specular[0], shapes[i].material.specular[1], shapes[i].material.specular[2]);
        printf("  material.Tr = (%f, %f ,%f)\n", shapes[i].material.transmittance[0], shapes[i].material.transmittance[1], shapes[i].material.transmittance[2]);
        printf("  material.Ke = (%f, %f ,%f)\n", shapes[i].material.emission[0], shapes[i].material.emission[1], shapes[i].material.emission[2]);
        printf("  material.Ns = %f\n", shapes[i].material.shininess);
        printf("  material.map_Ka = %s\n", shapes[i].material.ambient_texname.c_str());
        printf("  material.map_Kd = %s\n", shapes[i].material.diffuse_texname.c_str());
        printf("  material.map_Ks = %s\n", shapes[i].material.specular_texname.c_str());
        printf("  material.map_Ns = %s\n", shapes[i].material.normal_texname.c_str());
        std::map<std::string, std::string>::iterator it(shapes[i].material.unknown_parameter.begin());
        std::map<std::string, std::string>::iterator itEnd(shapes[i].material.unknown_parameter.end());
        for (; it != itEnd; it++) {
            printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
        }
        printf("\n");
    }
}

Shapodino ShapodinoBuilder::makeShapodino() {
    Shapodino shape;
    for(auto iterShape = shapes.begin(); iterShape < shapes.end(); ++iterShape) {
        std::vector<float> vtxPositions;
        std::vector<float> uvCoords;
        std::vector<float> vnPositions;
        for(auto iterIndices = (*iterShape).mesh.indices.begin(); iterIndices < (*iterShape).mesh.indices.end(); ++iterIndices) {
            // gather vertex positions
            vtxPositions.push_back((*iterShape).mesh.positions[
                ((*iterIndices) * 3)
            ]);
            vtxPositions.push_back((*iterShape).mesh.positions[
                ((*iterIndices) * 3) + 1
            ]);
            vtxPositions.push_back((*iterShape).mesh.positions[
                ((*iterIndices) * 3) + 2
            ]);

            // gather vertex normals
            vnPositions.push_back((*iterShape).mesh.normals[
                ((*iterIndices) * 3)
            ]);
            vnPositions.push_back((*iterShape).mesh.normals[
                ((*iterIndices) * 3) + 1
            ]);
            vnPositions.push_back((*iterShape).mesh.normals[
                ((*iterIndices) * 3) + 2
            ]);

            // gather uv coordinates
            uvCoords.push_back((*iterShape).mesh.texcoords[
                (*iterIndices) * 2
            ]);
            uvCoords.push_back((*iterShape).mesh.texcoords[
                ((*iterIndices) * 2) + 1
            ]);
        }
        // create a bufferino for the vertex positions
        std::shared_ptr<BufferSequerino> buffer_sequence = std::make_shared<BufferSequerino>();
        // -> layout(location = 0)
        buffer_sequence->pushBuffer(MakeBufferino(
            std::move(vtxPositions),
            3
        ));
        // -> layout(location = 1)
        buffer_sequence->pushBuffer(MakeBufferino(
            std::move(uvCoords),
            2
        ));
        // -> layout(location = 2)
        buffer_sequence->pushBuffer(MakeBufferino(
            std::move(vnPositions),
            3
        ));
        buffer_sequence->pushTexture(GL_TEXTURE0, MakeTextureBufferino(
            (mtldir + (*iterShape).material.diffuse_texname).c_str()
        ));
        buffer_sequence->build();
        shape.pushBufferSequence(buffer_sequence);
    }

    // build triangle mesh shape for bullet physics
    btTriangleMesh *mesh = new btTriangleMesh();
    btConvexHullShape *colshape = new btConvexHullShape();
    for(auto iterShape = shapes.begin(); iterShape < shapes.end(); ++iterShape) {
        for(size_t i = 0; i < (*iterShape).mesh.indices.size() / 3; ++i) {
            // index 1
            auto idx1 = (*iterShape).mesh.indices[i * 3];
            btVector3 v0(
                (*iterShape).mesh.positions[(idx1 * 3)],
                (*iterShape).mesh.positions[(idx1 * 3) + 1],
                (*iterShape).mesh.positions[(idx1 * 3) + 2]
            );
            // index 2
            auto idx2 = (*iterShape).mesh.indices[(i * 3) + 1];
            btVector3 v1(
                (*iterShape).mesh.positions[(idx2 * 3)],
                (*iterShape).mesh.positions[(idx2 * 3) + 1],
                (*iterShape).mesh.positions[(idx2 * 3) + 2]
            );
            // index 3
            auto idx3 = (*iterShape).mesh.indices[(i * 3) + 2];
            btVector3 v2(
                (*iterShape).mesh.positions[(idx3 * 3)],
                (*iterShape).mesh.positions[(idx3 * 3) + 1],
                (*iterShape).mesh.positions[(idx3 * 3) + 2]
            );
            mesh->addTriangle(v0,v1,v2);


            colshape->addPoint(v0);
            colshape->addPoint(v1);
            colshape->addPoint(v2);
        }
    }
    shape.setCollisionShape(colshape);
    shape.setMesh(mesh);
    return shape;
}

/*
* Used for calculating the View matrix
* * * * * * * * * * * * * * * * * * */
Camerino::Camerino() {
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    // Camera matrix
    view = glm::lookAt(
        glm::vec3(3,3,3), // Camera is at (3,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    // Model matrix : an identity matrix (model will be at the origin)
    model = glm::mat4(1.0f); // Changes for each model !
    // Our ModelViewProjection : multiplication of our 3 matrices
    mvp = projection * view * model; // Remember, matrix multiplication is the other way around
}

glm::mat4 Camerino::getMVP(const glm::mat4 &myprojection, const glm::mat4 &mymodel) {
    projection = myprojection;
    model = mymodel;
    return myprojection * view * mymodel;
}

glm::mat4 Camerino::getView() {
    return view;
}

glm::mat4 Camerino::getModel() {
    return model;
}

/*
* Helperino for compiling and
* using shader programinos  *
* * * * * * * * * * * * * * */
Shaderino::Shaderino() : linked(false) {
    programid = glCreateProgram();
}

std::string Shaderino::readfile(const char *path) {
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

void Shaderino::compile(const char *filename, GLenum shaderType) {
    GLuint shaderid = glCreateShader(shaderType);
    // compile shader
    std::string thesource = readfile(filename);
    const char *source = thesource.c_str();
    glShaderSource(shaderid, 1, &source, NULL);
    glCompileShader(shaderid);

    // check if it compiled
    GLint Result = GL_FALSE;
    int infoLogLength;
    glGetShaderiv(shaderid, GL_COMPILE_STATUS, &Result);
    if(!Result) {
        glGetShaderiv(shaderid, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<char> shaderErrorMessage(infoLogLength);
        glGetShaderInfoLog(shaderid, infoLogLength, NULL, &shaderErrorMessage[0]);
        fprintf(stdout, "%s\n", &shaderErrorMessage[0]);
    }
    shaders.push_back(shaderid);
}

void Shaderino::link() {
    assert(!linked);
    for(auto itr = shaders.begin(); itr < shaders.end(); ++itr) {
        glAttachShader(programid, *itr);
    }
    glLinkProgram(programid);
    for(auto itr = shaders.begin(); itr < shaders.end(); ++itr) {
        glDeleteShader(*itr);
    }

    // check program
    GLint Result = GL_FALSE;
    int InfoLogLength;

    glGetProgramiv(programid, GL_LINK_STATUS, &Result);
    glGetProgramiv(programid, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage(fmax(InfoLogLength, int(1)));
    glGetProgramInfoLog(programid, InfoLogLength, NULL, &ProgramErrorMessage[0]);

    if(!Result) {
        std::cout << "SHADER LINKING ERROR" << std::endl;
        fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
        std::exit(EXIT_FAILURE);
    }

    // set max array size uniform
    GLint max_array_size_loc = glGetUniformLocation(programid, "max_array_size");
    glUniform1ui(max_array_size_loc, GL_MAX_VERTEX_UNIFORM_VECTORS);

    linked = true;
}

void Shaderino::use() {
    glUseProgram(programid);
}

Shaderino::~Shaderino() {
    glDeleteProgram(programid);
}
