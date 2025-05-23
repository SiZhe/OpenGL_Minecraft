#include "SpotLight.hpp"

SpotLight::SpotLight() {

}

SpotLight::SpotLight(float innerAngle, float outerAngle, glm::vec3 targetDirection) {
    mInnerAngle = innerAngle;
    mOuterAngle = outerAngle;
    mTargetDirection = targetDirection;
}


SpotLight::~SpotLight() {

}
