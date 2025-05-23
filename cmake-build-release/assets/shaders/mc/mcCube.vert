#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aUv;
layout (location = 3) in vec3 aNormal;
layout (location = 4) in mat4 aInstanceMatrices;
layout (location = 8) in float aFaceIndex;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectMatrix;

out vec2 uv;
out vec3 normal;
out vec3 worldPosition;
out float faceIndex;  // 传递给片段着色器

void main() {
    vec4 aPos = vec4(aPos,1.0f);

    vec4 position = projectMatrix * viewMatrix * modelMatrix * aInstanceMatrices * aPos;

    gl_Position = position;

    uv = aUv;

    normal = transpose(inverse(mat3(modelMatrix)))*aNormal;

    worldPosition = (modelMatrix * aPos).xyz;

    faceIndex = aFaceIndex;  // 传递面索引
}