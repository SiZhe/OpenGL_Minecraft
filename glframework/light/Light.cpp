#include "Light.hpp"

Light::Light() {

}

Light::~Light() {

}

void Light::setColor(glm::vec3 color) {
    mColor = color;
}

void Light::setSpecularIntensity(float specularIntensity) {
    mSpecularIntensity = specularIntensity;
}