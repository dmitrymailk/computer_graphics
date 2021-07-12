#version 330 core

uniform sampler2D pos;
uniform sampler2D vel;
uniform int numParticles;

out vec2 uv;
out vec3 color;

// Offsets and UVs for the triangle around the particle
const int NUM_VERTICES_PER_PARTICLE = 3;
const vec4 TRI_VERTS[NUM_VERTICES_PER_PARTICLE] = vec4[NUM_VERTICES_PER_PARTICLE](
    // TODO [Task 18] Calculate the triangle point offsets (see diagram in handout)
    // vec4(p1.x, p1.y, 0, 0),
    // vec4(p2.x, p2.y, 0, 0),
    // vec4(p3.x, p3.y, 0, 0)
);

// Convert from HSL to RGB
// source: http://www.geekymonkey.com/Programming/CSharp/RGB2HSL_HSL2RGB.htm
vec3 HSLtoRGB(float h, float s, float l) {
    float v;
    float r, g, b;
    v = (l <= 0.5) ? (l * (1.0 + s)) : (l + s - l * s);

    if (v > 0) {
        float m;
        float sv;
        int sextant;
        float fract, vsf, mid1, mid2;

        m = l + l - v;
        sv = (v - m) / v;
        h *= 6.0;
        sextant = int(h);
        fract = h - sextant;
        vsf = v * sv * fract;
        mid1 = m + vsf;
        mid2 = v - vsf;
        if (sextant == 0) {
            r = v;
            g = mid1;
            b = m;
        } else if (sextant == 1) {
            r = mid2;
            g = v;
            b = m;
        } else if (sextant == 2) {
            r = m;
            g = v;
            b = mid1;
        } else if (sextant == 3) {
            r = m;
            g = mid2;
            b = v;
        } else if (sextant == 4) {
            r = mid1;
            g = m;
            b = v;
        } else if (sextant == 5) {
            r = v;
            g = m;
            b = mid2;
        }
    }

    return vec3(r, g, b);
}

vec3 pickRainbowColor(float x) {
    return HSLtoRGB(x, 0.5, 0.5);
}

void main() {
    int particleID = 0;
    int triID = 0;
    // TODO [Task 18] Which particle and triangle vertex are we dealing with?

    // Pass the tex coords to the fragment shader
    uv = TRI_VERTS[triID].xy;

    vec4 posTime = vec4(0,0,0,1);
    vec4 velAge = vec4(0);
    // TODO [Task 18] sample pos and vel textures

    // Calculate diameter based on age and lifetime
    float diameter = 0.02;
    diameter *= min(min(1.0, velAge.w / (0.1 * posTime.w)),
                    min(1.0, abs(posTime.w - velAge.w) / (0.1 * posTime.w)));

    // Calculate color based on particleID
    color = pickRainbowColor(float(particleID)/numParticles);

    // the offset to the points of the triangle
    vec4 triPos = diameter * TRI_VERTS[triID];

    // anchor point in clip space
    vec4 anchorPoint = vec4(posTime.xyz, 1.0);

    // Center the particle around anchorPoint
    gl_Position = anchorPoint + triPos - diameter * vec4(0.5, 0.5, 0.0, 0.0);
}
