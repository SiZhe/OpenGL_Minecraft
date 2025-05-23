#version 460 core
in vec3 color;
in vec2 uv;
in vec3 normal;
out vec4 FragColor;
uniform float time;
uniform sampler2D sampler;

void main() {
   vec4 objectColor = texture(sampler,uv);

   vec3 normalN = normalize(normal);

   FragColor = vec4(objectColor.xyz,1.0f);
}