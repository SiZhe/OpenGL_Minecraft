#version 460 core
in vec3 uvw;
out vec4 FragColor;

uniform sampler2D sphericalSampler;

const float PI = 3.14159265358979323;

vec2 uvwToUv(vec3 uvwN){
    float phi = asin(uvwN.y);
    float theta = atan(uvwN.z,uvwN.x);
    float u = theta / (2.0 * PI) + 0.5;
    float v = phi / PI + 0.5;
    return vec2(u,v);
}

void main(){
    vec3 uvwN = normalize(uvw);
    vec2 uv = uvwToUv(uvwN);

    FragColor = texture(sphericalSampler,uv);
}