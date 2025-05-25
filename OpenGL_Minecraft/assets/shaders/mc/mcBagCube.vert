#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aUv;

uniform mat4 modelMatrix;

out vec2 uv;

void main()
{
    gl_Position = modelMatrix * vec4(aPos, 1.0f);
    uv = aUv;
}