#pragma once
#include "Light.hpp"

class AmbientLight :public Light{
    public:
    AmbientLight();
    AmbientLight(glm::vec3 color);
    ~AmbientLight();
};
