#version 330 core

out vec4 fragColor;

uniform samplerCube skybox;

in vec3 pos_object;

void main() {
     fragColor = texture(skybox, pos_object);
}
