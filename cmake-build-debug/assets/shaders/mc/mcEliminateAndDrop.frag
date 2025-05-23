#version 460 core
in vec2 uv;
in float faceIndex;

uniform sampler2DArray sampler;
uniform float opacity;

out vec4 FragColor;

void main() {
    vec4 objectColor = texture(sampler,vec3(uv, faceIndex));

    //float alpha = texture(sampler,uv).a;
    if(objectColor.a < 0.5){
        discard;
    }
    //float alpha = texture(opacityMaskSampler,uv).r;

    FragColor = objectColor;
}