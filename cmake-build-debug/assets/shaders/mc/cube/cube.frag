#version 460 core
in vec2 uv;
in vec3 normal;
in vec3 worldPosition;

uniform sampler2D samplerH;
uniform sampler2D samplerV;

out vec4 FragColor;

void main(){
    // 根据法线方向选择纹理
    if ( normal.y > 0.5) {
        FragColor = texture(samplerH, uv);  // 上下
    } else {
        FragColor = texture(samplerV, uv);  // 四周
    }
}