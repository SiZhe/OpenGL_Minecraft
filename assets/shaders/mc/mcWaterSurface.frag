#version 460 core
in vec2 uv;
in vec3 normal;
in vec3 worldPosition;
in float faceIndex;

uniform sampler2DArray sampler;
uniform sampler2D specularMaskSampler;

uniform vec3 cameraPosition;
uniform vec3 ambientColor;
uniform float shininess;

uniform float opacity;
uniform float time;

out vec4 FragColor;

struct DirectionLight{
    vec3 direction;
    vec3 color;
    float specularIntensity;
};

struct PointLight{
    vec3 position;
    vec3 color;
    float k2;
    float k1;
    float kc;
    float specularIntensity;
};

struct SpotLight{
    vec3 position;
    vec3 color;
    float innerCos;
    float outerCos;
    vec3 targetDirection;
    float specularIntensity;
};

uniform DirectionLight directionLight;
#define POINT_LIGHT_NUM 2
uniform PointLight pointLights[POINT_LIGHT_NUM];
uniform SpotLight spotLight;

//漫反射
vec3 calculateDiffuse(vec3 lightColor,vec3 objectColor,vec3 lightDirectionN,vec3 normalN){
    float diffuse = clamp(dot(-lightDirectionN,normalN),0.0f,1.0f);
    vec3 diffuseColor = lightColor * objectColor * diffuse;
    return diffuseColor;
}

//高光反射
vec3 calculateSpecular(vec3 lightColor,vec3 lightDirectionN,vec3 normalN,vec3 viewDirectionN,float intensity){
    float dotResult = dot(-lightDirectionN,normalN);

    float flag = step(0.0f,dotResult);

    vec3 lightReflectN = normalize(reflect(lightDirectionN,normalN));

    float specular = clamp(dot(lightReflectN,-viewDirectionN),0.0f,1.0f);

    specular = pow(specular,shininess);

    float specularMask = texture(specularMaskSampler,uv).r;

    vec3 specularColor = lightColor * specular * flag * intensity * specularMask;

    return specularColor;
}


vec3 calculateDirectionLight(DirectionLight light, vec3 objectColor,vec3 normalN,vec3 viewDirectionN){
    vec3 lightDirectionN = normalize(light.direction);

    vec3 diffuesColor = calculateDiffuse(light.color,objectColor,lightDirectionN,normalN);
    vec3 specularColor = calculateSpecular(light.color,lightDirectionN,normalN,viewDirectionN,light.specularIntensity);

    vec3 directionColor = diffuesColor + specularColor;
    return directionColor;
}

vec3 calculatePointLight(PointLight light, vec3 objectColor,vec3 normalN,vec3 viewDirectionN){
    vec3 lightDirectionN = normalize(worldPosition-light.position);
    float dist = length(worldPosition - light.position);

    float attenuation = 1.0f / (light.k2 * pow(dist,2) + light.k1*(dist) + light.kc);

    vec3 diffuesColor = calculateDiffuse(light.color,objectColor,lightDirectionN,normalN);

    vec3 specularColor = calculateSpecular(light.color,lightDirectionN,normalN,viewDirectionN,light.specularIntensity);

    vec3 pointColor = (diffuesColor + specularColor) * attenuation;

    return pointColor;
}

vec3 calculateSpotLight(SpotLight light, vec3 objectColor,vec3 normalN,vec3 viewDirectionN){
    vec3 lightDirectionN = normalize(worldPosition - light.position);
    vec3 targetDirectionN = normalize(light.targetDirection);

    float gamma = dot(targetDirectionN,lightDirectionN);

    float  spot = clamp((gamma - light.outerCos) / (light.innerCos - light.outerCos),0.0,1.0f);

    vec3 diffuesColor = calculateDiffuse(light.color,objectColor,lightDirectionN,normalN);

    vec3 specularColor = calculateSpecular(light.color,lightDirectionN,normalN,viewDirectionN,light.specularIntensity);

    vec3 spotColor = (diffuesColor + specularColor) * spot ;

    return spotColor;
}

void main() {
    vec3 finalColor = vec3(0.0f,0.0f,0.0f);

    // ---------- 修改部分：处理top面UV动画 ----------
    vec2 animatedUV = uv;
    // top面的faceIndex为2
    if(faceIndex == 2.0) {
        // 向上滚动：增加UV的Y分量（UV Y轴向下，增加Y值会让纹理向上移动）
        // 使用fract()保证UV循环（超出[0,1]时自动取模）
        animatedUV = fract(uv + vec2(0.0, time  * 0.4));
    }

    vec3 objectColor = texture(sampler,vec3(animatedUV, faceIndex)).xyz;
    float alpha = texture(sampler,vec3(animatedUV, faceIndex)).a;

    vec3 normalN = normalize(normal);
    vec3 viewDirectionN = normalize(worldPosition - cameraPosition);

    finalColor += ambientColor * objectColor;
    finalColor += calculateDirectionLight(directionLight, objectColor, normalN, viewDirectionN);

    //for (int i = 0 ; i < POINT_LIGHT_NUM ; i++){
    //    finalColor += calculatePointLight(pointLights[i], objectColor, normalN, viewDirectionN);
    //}
    //finalColor += calculateSpotLight(spotLight,objectColor, normalN, viewDirectionN);

    vec4 result = vec4( finalColor ,alpha * opacity);

    //处理透明物体
    if( result.a < 0.2){
        discard;
    }

    //FragColor = vec4(vec3(faceIndex/6.0), 1.0);
    FragColor = result;
}