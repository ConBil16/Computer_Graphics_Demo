#version 410 core

// uniform inputs
uniform sampler2D texMap;

// varying inputs
in vec2 texCoordOut;

// fragment outputs
out vec4 fragColorOut;

void main() {
    vec4 texLookup = texture(texMap, texCoordOut);

    fragColorOut = texLookup;
}