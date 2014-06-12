#version 330 core

in vec2 UV;
out vec3 color;

uniform sampler2D texture0;

void main() {
    color = texture(texture0, UV).rgb;
}
