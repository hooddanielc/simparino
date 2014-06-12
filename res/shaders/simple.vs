#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec2 uvcoord;
layout(location = 2) in vec3 normals;

out vec2 UV;
uniform mat4 MVP;

void main() {
    // Output position of the vertex, in clip space : MVP * position
    gl_Position =  MVP * vec4(vertex, 1);
    UV = uvcoord;
}
