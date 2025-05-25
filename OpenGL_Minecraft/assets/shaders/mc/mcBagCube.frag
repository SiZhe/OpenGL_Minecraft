#version 460 core
in vec2 uv;

uniform sampler2D sampler;

out vec4 FragColor;

void main()
{
    vec4 texColor = texture(sampler, uv);
    if(texColor.a < 0.3){
        discard;
    }

    FragColor = texColor;
}