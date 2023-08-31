#version 410 core

// uniform inputs
uniform mat4 mvpMatrix;

// attribute inputs
in vec3 vPos;
in vec2 texCoord;

// varying outputs
out vec2 texCoordOut;

void main() {
    gl_Position = mvpMatrix * vec4(vPos, 1.0);
    texCoordOut = texCoord;
}