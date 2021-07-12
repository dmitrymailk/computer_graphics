#version 330 core

in vec3 position;
in vec3 normal;

out vec3 vertex;	    // The position of the vertex, in camera space
out vec3 vertexToCamera;    // Vector from the vertex to the eye, which is the camera
out vec3 eyeNormal;	    // Normal of the vertex, in camera space

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main()
{
    vertex = ((view*model)*(vec4(position, 1.0))).xyz;
    eyeNormal = normalize(mat3(transpose(inverse(view*model))) * normal);
    vertexToCamera = -normalize(vertex);
    gl_Position = projection*view*model*vec4(position,1.0);
}
