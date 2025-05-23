#version 460 core

out vec4 FragColor;

void main() {
    //粉色:vec3(0.88f, 0.49f, 0.58f),vec3(1.0f, 0.75f, 0.8f)
    FragColor = vec4(vec3(1.0f, 1.0f, 0.2f),1.0f);
}