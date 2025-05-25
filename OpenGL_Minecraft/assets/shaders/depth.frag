#version 460 core

uniform float near;
uniform float far;

out vec4 FragColor;

void main() {
    float Zndc = gl_FragCoord.z * 2.0 -1.0;
    float linearDepth = 2.0 * near / (far + near - Zndc * (far - near));

    vec3 finalColor = vec3(linearDepth,linearDepth,linearDepth);
    FragColor = vec4(finalColor,1.0f);
}