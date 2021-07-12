#version 330 core

uniform mat4 model, view, projection;

// Light properties
const vec3 WorldSpace_lightPos = vec3(2, 2, 2); // world-space light position
uniform vec3 lightColor;
uniform float lightIntensity;

// Attenuation Properties
uniform float attQuadratic;
uniform float attLinear;
uniform float attConstant;

// Material properties
uniform vec3 color;
uniform float ambientIntensity;
uniform float diffuseIntensity;
uniform float specularIntensity;
uniform float shininess;


out vec3 fragColor;

void main(){

    fragColor = color;


}
