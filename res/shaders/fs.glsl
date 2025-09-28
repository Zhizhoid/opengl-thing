#version 330 core

layout(location = 0) out vec4 color;
in vec2 pos;

void main() {
    // color = vec4(1.0, 0.0, 0.0, 1.0);
    color = vec4(pos, 0, 0);
}
