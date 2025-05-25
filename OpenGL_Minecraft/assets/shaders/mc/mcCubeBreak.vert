#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aUv;
layout (location = 8) in float aFaceIndex;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectMatrix;

out float faceIndex;  // 传递给片段着色器
out vec2 uv;

void main() {
    vec4 aPos = vec4(aPos,1.0f);

    vec4 position = projectMatrix * viewMatrix * modelMatrix * aPos;

    gl_Position = position;

    uv = aUv;

    faceIndex = aFaceIndex;  // 传递面索引
}