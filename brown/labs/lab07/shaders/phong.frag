#version 330 core

in vec3 WorldSpace_position; // world-space position
in vec3 WorldSpace_normal;   // world-space normal

out vec3 fragColor;

void main(){
    vec3 WorldSpace_toLight = normalize(vec3(10.0) - WorldSpace_position);
    fragColor = vec3(0.3 + 0.7 * max(0.0, dot(normalize(WorldSpace_normal), WorldSpace_toLight)));
}
