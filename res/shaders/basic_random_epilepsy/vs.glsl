#version 330 core

layout(location = 0) in vec4 inPos;

uniform float uTime;

float r(float x) {
    return mod(mod(x*117,17)*119,9);
}

float hash(float x) {
    return mod(sin(1000*r(1)*x) + sin(1000*r(1)*x), 1);
}

void main() {
    float sum = inPos.x + inPos.y;
    vec2 randDir = vec2(hash(sum+uTime), hash(sum+uTime+0.3));
    randDir /= sqrt(randDir.x*randDir.x + randDir.y*randDir.y);
    randDir -= vec2(0.5, 0.5);

    gl_Position = inPos + vec4(randDir, 0, 0)*0.1;
}
