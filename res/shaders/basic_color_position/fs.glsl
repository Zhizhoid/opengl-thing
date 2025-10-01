#version 330 core

out vec4 color;
in vec2 pos;

void main() {
    color = vec4(pos, 0, 0);
}
