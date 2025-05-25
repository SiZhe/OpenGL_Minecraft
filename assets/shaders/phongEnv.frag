#version 460 core
in vec3 color;
in vec2 uv;
in vec3 normal;
in vec3 worldPosition;

uniform float time;
uniform float opacity;
uniform sampler2D sampler;
uniform sampler2D specularMaskSampler;
uniform samplerCube envSampler;

uniform vec3 ambientColor;
uniform float shininess;
uniform vec3 cameraPositon;

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
    vec3 targetDirection;
    float innerCos;
    float outerCos;
    float specularIntensity;
};

uniform DirectionLight directionLight;
uniform int pointLightNum;
#define POINT_LIGHT_NUM 2
uniform PointLight pointLights[POINT_LIGHT_NUM];
uniform SpotLight spotLight;

vec3 calculateDiffuse(vec3 lightColor, vec3 lightDirectionN, vec3 objectColor , vec3 normalN){
    float diffuse = clamp(dot(-normalN,lightDirectionN),0.0f,1.0f);
    vec3 diffuseColor = lightColor * objectColor * diffuse;
    return diffuseColor;
}

vec3 calcualateSpecular(vec3 lightColor,vec3 lightDirectionN,vec3 normalN,vec3 viewDirN,float intensity){
    //防止背部
    float dotResult = dot(-lightDirectionN,normalN);
    float flag = step(0.0f,dotResult);

    vec3 lightReflectN = normalize(reflect(lightDirectionN,normalN));

    float specular = clamp(dot(lightReflectN,-viewDirN),0.0f,1.0f);

    specular = pow(specular,shininess);

    float specularMask = texture(specularMaskSampler,uv).r;

    vec3 specularColor = lightColor * specular * flag * intensity * specularMask;

    return specularColor;
}


vec3 calculateDirectionLight(DirectionLight light, vec3 objectColor, vec3 normalN, vec3 viewDirN){
    vec3 lightDirectionN = normalize(light.direction);

    vec3 diffuseColor = calculateDiffuse(light.color,lightDirectionN,objectColor,normalN);

    vec3 specularColor = calcualateSpecular(light.color,lightDirectionN,normalN,viewDirN,light.specularIntensity);

    return diffuseColor + specularColor;
}

vec3 calcualatePointLight(PointLight light, vec3 objectColor, vec3 normalN, vec3 viewDirN){
    vec3 lightDirectionN = normalize(worldPosition - light.position);
    //计算衰减值（点光源）
    float dist = length(worldPosition-light.position);
    float attenuation = 1.0f / (light.k2 * pow(dist,2) + light.k1*(dist) + light.kc);

    vec3 diffuseColor = calculateDiffuse(light.color,lightDirectionN,objectColor,normalN);

    vec3 specularColor = calcualateSpecular(light.color,lightDirectionN,normalN,viewDirN,light.specularIntensity);

    return (diffuseColor + specularColor) * attenuation;
}

vec3 calcualateSpotLight(SpotLight light, vec3 objectColor, vec3 normalN, vec3 viewDirN){

    vec3 lightDirectionN = normalize(worldPosition - light.position);

    vec3 targetDirectionN = normalize(light.targetDirection);

    float gamma = dot(targetDirectionN,lightDirectionN);

    float intensity = clamp((gamma - light.outerCos) / (light.innerCos - light.outerCos),0.0,1.0f);

    vec3 diffuseColor = calculateDiffuse(light.color,lightDirectionN,objectColor,normalN);

    vec3 specularColor = calcualateSpecular(light.color,lightDirectionN,normalN,viewDirN,light.specularIntensity);

    return (diffuseColor + specularColor) * intensity;
}

vec3 calcualateEnv(vec3 normalN, vec3 viewDirN){
    vec3 reflectN = normalize(reflect(viewDirN,normalN));
    vec3 color = texture(envSampler,reflectN).rgb;
    return color;
}

void main() {

    vec3 finalColor = vec3(0.0f,0.0f,0.0f);

    vec3 objectColor = texture(sampler,uv).xyz;
    float alpha = texture(sampler,uv).a;
    vec3 viewDirN = normalize(worldPosition - cameraPositon);
    vec3 normalN = normalize(normal);
    vec3 ambientColor = calcualateEnv(normalN,viewDirN);
    finalColor += objectColor * ambientColor;

    //finalColor += calculateDirectionLight(directionLight, objectColor, normalN, viewDirN);
    for (int i = 0; i < POINT_LIGHT_NUM; i++){
        //finalColor += calcualatePointLight(pointLights[i], objectColor, normalN, viewDirN);
    }
    //finalColor += calcualateSpotLight(spotLight, objectColor, normalN, viewDirN);


    //float flag = step(400,gl_FragCoord.x);

    //vec3 blendColor = mix(vec3(1.0,0.0,0.0),vec3(0.0,0.0,1.0),flag);

    //finalColor *= blendColor;

    //finalColor = vec3(gl_FragCoord.z);

    FragColor = vec4(finalColor,alpha * opacity);
}