#version 330 core

layout(location = 0) in vec4 inPos;
out vec2 pos;

void main() {
    gl_Position = inPos;
    pos = inPos.xy;
}
