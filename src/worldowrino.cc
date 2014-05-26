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

// Has many tings
Shapodino::Shapodino(const char *objfile) {
    std::string err = tinyobj::LoadObj(shapes, objfile);
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
    model = glm::mat4(1.0f);  // Changes for each model !
    // Our ModelViewProjection : multiplication of our 3 matrices
    mvp = projection * view * model; // Remember, matrix multiplication is the other way around
}

Camerino::~Camerino() {

}
