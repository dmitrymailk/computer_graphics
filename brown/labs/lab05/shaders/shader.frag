#version 330 core

in vec3 WS_position; // world-space position
in vec3 WS_normal;   // world-space normal

out vec3 fragColor;

void main(){
    vec3 WS_toLight = normalize(vec3(10.0) - WS_position);
    fragColor = vec3(0.3 + 0.7 * max(0.0, dot(normalize(WS_normal), WS_toLight)));
}
