#version 460 core
in vec2 uv;

uniform sampler2D sampler;
//uniform sampler2D opacityMaskSampler;
uniform float opacity;

out vec4 FragColor;

void main() {
    vec4 objectColor = texture(sampler,uv);

    //float alpha = texture(sampler,uv).a;
    if(objectColor.a < 0.5){
        discard;
    }
    //float alpha = texture(opacityMaskSampler,uv).r;

    FragColor = objectColor;
}