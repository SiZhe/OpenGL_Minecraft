#version 460 core
in vec2 uv;
in float faceIndex;

uniform sampler2DArray sampler;

out vec4 FragColor;

void main() {
    vec4 objectColor = texture(sampler,vec3(uv, faceIndex));

    if (objectColor.a < 0.5){
        discard;
    }

    FragColor = objectColor;
}