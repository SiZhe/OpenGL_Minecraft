#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aUv;
layout (location = 4) in mat4 aInstanceMatrices;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectMatrix;

out vec2 uv;

void main() {
    vec4 aPos = vec4(aPos,1.0f);

    vec4 position = projectMatrix * viewMatrix * modelMatrix * aInstanceMatrices * aPos;

    gl_Position = position;

    uv = aUv;
}