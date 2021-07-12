#version 400 core

in vec3 fragPos;

out vec4 fragColor;

uniform float time;

vec4 getColor() {
    float r = (sin(time/1.5)+1)/2;
    float g = (sin(time/1.5*1.3)+1)/2;
    float b = (sin(time/1.5*1.6)+1)/2;
    return vec4(r,g,b,1);
}

void main() {
    fragColor = getColor();
}
