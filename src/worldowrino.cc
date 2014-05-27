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
    // adding a collision shape
    btCollisionShape* shape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
    btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(0,-1,0)));
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
        0,                  // mass
        motionState,        // initial position
        shape,              // collision shape of body
        btVector3(0,0,0)    // local inertia
    );
    btRigidBody *rigidBody = new btRigidBody(rigidBodyCI);
    dynamicsWorld->addRigidBody(rigidBody);
}

Worldowrino::~Worldowrino() {
    // Clean up the Bullet physicserino
    delete dynamicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
    delete broadphase;
}

/*
* Responsible for parsing
* and drawing object
* * * * * */
Shapodino::Shapodino(const char *objfile) {
    std::string err = tinyobj::LoadObj(shapes, objfile);
}

void Shapodino::printToConsole() {
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
        for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
            printf("  v[%ld] = (%f, %f, %f)\n", v,
            shapes[i].mesh.positions[3*v+0],
            shapes[i].mesh.positions[3*v+1],
            shapes[i].mesh.positions[3*v+2]);
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

std::vector<float> Shapodino::getMesh() {
    // Cound all indices
    std::vector<float> vtxPositions;
    unsigned count = 0;
    for (size_t i = 0; i < shapes.size(); ++i) {
        for (size_t f = 0; f < shapes[i].mesh.indices.size(); ++f) {
            vtxPositions.push_back(shapes[i].mesh.positions[
                shapes[i].mesh.indices[f] * 3
            ]);
            vtxPositions.push_back(shapes[i].mesh.positions[
                (shapes[i].mesh.indices[f] * 3) + 1
            ]);
            vtxPositions.push_back(shapes[i].mesh.positions[
                (shapes[i].mesh.indices[f] * 3) + 2
            ]);
        }
    }
    return vtxPositions;
}

Shapodino::~Shapodino() {

}

/*
* Used for calculating the View matrix
* * * * * * * * * * * * * * * * * * */
Camerino::Camerino() {
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    // Camera matrix
    view = glm::lookAt(
        glm::vec3(5,5,5), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    // Model matrix : an identity matrix (model will be at the origin)
    model = glm::mat4(1.0f); // Changes for each model !
    // Our ModelViewProjection : multiplication of our 3 matrices
    mvp = projection * view * model; // Remember, matrix multiplication is the other way around
}

Camerino::~Camerino() {

}

/*
* For managing a Vertex Array Object
* with many Vertex Buffer Objects
* * * * * * * * * * * * * * * * * */
Bufferino::Bufferino() {
    // generate a vertex array object
    glGenVertexArrays(1, &vao);
}

void Bufferino::addBuffer(int idx, GLfloat *data, int size) {
    glBindVertexArray(vao);
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    vbos[idx] = vbo;
}

void Bufferino::bind() {
    for(auto it = vbos.begin(); it != vbos.end(); ++it) {
        glEnableVertexAttribArray(it->first);
        glBindBuffer(GL_ARRAY_BUFFER, it->second);
        glVertexAttribPointer(
           it->first,          // number must match the layout in the shader
           3,                  // size
           GL_FLOAT,           // type
           GL_FALSE,           // normalized?
           0,                  // stride
           (void*)0            // array buffer offset
        );
    }
}

void Bufferino::disable() {
    for(auto it = vbos.begin(); it != vbos.end(); ++it) {
        glDisableVertexAttribArray(it->first);
    }
}

Bufferino::~Bufferino() {
    // delete vertex array object
    glDeleteVertexArrays(1, &vao);
    // delete shaders
    for(auto it = vbos.begin(); it != vbos.end(); ++it) {
        glDeleteBuffers(1, &it->second);
    }
}

/*
* Helperino for compiling and
* using shader programinos  *
* * * * * * * * * * * * * * */
Shaderino::Shaderino() {
    id = glCreateProgram();
}

void Shaderino::compile(const char *filename, GLenum shaderType) {
    GLuint shaderid = glCreateShader(shaderType);
    // compile shader
    std::string thesource = Apperino::get()->readfile(filename);
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
    std::vector<char> ProgramErrorMessage(fmax(InfoLogLength, int(1)));
    glGetProgramInfoLog(id, InfoLogLength, NULL, &ProgramErrorMessage[0]);

    if(!Result) {
        std::cout << "SHADER LINKING ERROR" << std::endl;
        fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
        std::exit(EXIT_FAILURE);
    }
}

void Shaderino::use() {
    glUseProgram(id);
}

Shaderino::~Shaderino() {
    glDeleteProgram(id);
}
