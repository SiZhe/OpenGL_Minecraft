#version 460 core
in vec2 uv;

uniform sampler2D sampler;

out vec4 FragColor;

void main() {
    vec4 objectColor = texture(sampler, uv);
    if(objectColor.a < 0.5){
        discard;
    }
    FragColor = objectColor;
}