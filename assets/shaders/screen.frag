#version 460 core
in vec2 uv;

uniform sampler2D screenTexSampler;
uniform float texWidth;
uniform float texHeight;

out vec4 FragColor;

vec3 blur(){
    float du = 1.0 / texWidth;
    float dv = 1.0 / texHeight;

    vec2 offset[9] = vec2[](
    vec2(-du,dv),vec2(0.0,dv),vec2(du,dv),
    vec2(-du,0.0),vec2(0.0,0.0),vec2(du,0.0),
    vec2(-du,-dv),vec2(0.0,-dv),vec2(du,-dv)
    );

    float kernel[9] = float[](
    1.0,2.0,1.0,
    2.0,4.0,2.0,
    1.0,2.0,1.0
    );

    vec3 sumColor = vec3(0.0,0.0,0.0);
    for (int i = 0 ; i < 9 ; i++){
        vec3 samplerColor = texture(screenTexSampler,uv+offset[i]).rgb;
        sumColor += samplerColor * kernel[i];
    }
    sumColor /= 16.0;
    return sumColor;
}

void main() {
    vec3 color = texture(screenTexSampler,uv).rgb;
    //vec3 color = blur();
    FragColor = vec4(color,1.0f);
}