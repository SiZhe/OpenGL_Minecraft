#pragma once
#include "Light.hpp"
#include "../Object.hpp"

class SpotLight : public Light,public Object{
    public:
    SpotLight();
    SpotLight(float innerAngle,float outerAngle, glm::vec3 targetDirection);
    ~SpotLight();

    float mInnerAngle{glm::radians(25.0f)};
    float mOuterAngle{glm::radians(30.0f)};
    glm::vec3 mTargetDirection{glm::vec3(-1.0f,0.0f,0.0f)};
};

