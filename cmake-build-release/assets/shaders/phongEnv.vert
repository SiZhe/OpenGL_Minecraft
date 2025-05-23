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
    //vec3 postime = aPos + sin(5*time) * (0.3);
    //float scale = 1 / (time * time);
    //将输入的顶点位置转化为齐次坐标
    vec4 attriPosition = vec4(aPos,1.0f);
    //计算当前顶点的worldPosition
    worldPosition = (transform * attriPosition).xyz;

    vec4 position = projectMatrix * viewMatrix * transform * attriPosition;

    gl_Position = position;

    //color = aColor* ((sin(time)+1.0)/2.0);
    color = aColor;

    uv = aUV;

    normal = transpose(inverse(mat3(transform)))*aNormal;

}