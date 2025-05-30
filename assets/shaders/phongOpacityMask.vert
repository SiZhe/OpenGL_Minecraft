#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aNormal;

uniform float time;
uniform mat4 transform;
uniform mat4 viewMatrix;
uniform mat4 projectMatrix;

out vec3 color;
out vec2 uv;
out vec3 normal;
out vec3 worldPosition;

void main() {
    vec4 aPos = vec4(aPos,1.0f);

    vec4 position = projectMatrix * viewMatrix * transform * aPos;

    gl_Position = position;

    color = aColor;

    uv = aUV;

    normal = transpose(inverse(mat3(transform)))*aNormal;

    worldPosition = (transform * aPos).xyz;
}