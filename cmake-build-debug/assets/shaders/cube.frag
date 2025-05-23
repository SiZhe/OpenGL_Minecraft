#version 460 core
in vec3 uvw;
out vec4 FragColor;

uniform samplerCube cubeSampler;

void main(){
    FragColor = texture(cubeSampler,uvw);
}