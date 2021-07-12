#version 400

layout(location = 0) in vec3 position;
layout(location = 5) in vec2 texCoord;
out vec2 uv;
void main() {
    uv = texCoord;
    gl_Position = vec4(position, 1.0);

}
