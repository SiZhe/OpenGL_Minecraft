#pragma once
#include "Light.hpp"
#include "../Object.hpp"

class PointLight : public Light ,public Object{
    public:
    PointLight();
    PointLight(float k2,float k1,float kc);
    ~PointLight();

    float mK2{0.017f};
    float mK1{0.07f};
    float mKc{1.0f};
};
