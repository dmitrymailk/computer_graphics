#version 330 core

in vec2 uv;
in vec3 color;

out vec4 fragColor;

void main() {
    fragColor = vec4(1);
    // TODO [Task 19] Only color fragment if length(uv - 0.5) < 0.5, else discard
}
