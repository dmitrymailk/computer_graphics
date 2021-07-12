#version 330 core

in vec3 vertex;                 // The position of the vertex, in camera space!
in vec3 vertexToCamera;         // Vector from the vertex to the eye, which is the camera
in vec3 eyeNormal;	        // Normal of the vertex, in camera space!

uniform float r0;		// The R0 value to use in Schlick's approximation
uniform float eta1D;		// The eta value to use initially
uniform vec3  eta;              // Contains one eta for each channel (use eta.r, eta.g, eta.b in your code)

uniform mat4 view;
uniform mat4 model;

uniform samplerCube envMap;

out vec4 fragColor;

void main()
{
    vec3 n = normalize(eyeNormal);
    vec3 cameraToVertex = normalize(vertex); //remember we are in camera space!
    vec3 vertexToCamera = normalize(vertexToCamera);
    // TODO: fill the rest in


    fragColor = vec4(0.0);
}
