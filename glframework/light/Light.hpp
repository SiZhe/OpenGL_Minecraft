#pragma once
#include "../Core.hpp"
class Light {
    public:
    Light();
    ~Light();

    void setColor(glm::vec3 color);
    void setSpecularIntensity(float specularIntensity);

    glm::vec3 mColor{1.0f};
    float mSpecularIntensity{0.5f};
};
