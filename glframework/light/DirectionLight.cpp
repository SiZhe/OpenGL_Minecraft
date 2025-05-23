#include "DirectionLight.hpp"

DirectionLight::DirectionLight() {

}

DirectionLight::~DirectionLight() {

}

DirectionLight::DirectionLight(glm::vec3 color, glm::vec3 directiom)
    : mDirection(directiom)
{
    mColor = color;
}

