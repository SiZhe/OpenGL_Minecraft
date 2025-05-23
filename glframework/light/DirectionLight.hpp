#pragma once
#include "Light.hpp"

class DirectionLight : public Light {
public:
    DirectionLight();
    DirectionLight(glm::vec3 color,glm::vec3 directiom);
    ~DirectionLight();

    glm::vec3 mDirection{-1.0f};
};
