#version 460 core
layout (location  = 0) in vec3 aPos;
layout (location  = 1) in vec3 aColor;
layout (location  = 2) in vec2 aUv;
layout (location  = 3) in vec3 aNormal;
uniform float time;
uniform sampler2D sampler;
uniform mat4 transform;
uniform mat4 viewMatrix;
uniform mat4 projectMatrix;
out vec3 color;
out vec2 uv;
out vec3 normal;
void main() {
    //vec3 postime = aPos + sin(10*time) * (0.3);
    float scale = 1/ (time+1);
    vec4 aPos = vec4(aPos,1.0f);
    vec4 position = projectMatrix * viewMatrix * transform * aPos;

    gl_Position = position;

    color = aColor * (sin(5*time)+1);

    uv = aUv;

    normal = aNormal;
}