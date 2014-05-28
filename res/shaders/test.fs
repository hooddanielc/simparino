#version 330 core

in vec2 UV;
out vec3 color;
uniform sampler2D myTextureSampler;

void main() {
    // using texture unit 0
    color = texture(myTextureSampler, UV).rgb;
}