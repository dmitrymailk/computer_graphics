#version 400

layout(location = 0) in vec3 position;
// TODO (Task 4): Add a new in variable for the color attribute

// TODO (Task 5): Declare a new out vec3 to send the color attribute
void main() {
    gl_Position = vec4(position, 1.0);

    // TODO (Task 5): Set an output variable to a per-vertex color attribute.
}
