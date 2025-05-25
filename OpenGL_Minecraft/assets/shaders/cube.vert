#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 transform;
uniform mat4 viewMatrix;
uniform mat4 projectMatrix;

out vec3 uvw;

void main() {

    vec4 attriPosition = vec4(aPos,1.0f);

    vec4 position = projectMatrix * viewMatrix * transform * attriPosition;

    gl_Position = position.xyww;

    uvw = aPos;
}