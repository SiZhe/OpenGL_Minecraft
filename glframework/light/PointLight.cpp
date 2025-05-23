#include "PointLight.hpp"

PointLight::PointLight() {

}

PointLight::PointLight(float k2, float k1, float kc) {
    mK2 = k2;
    mK1 = k1;
    mKc = kc;
}

PointLight::~PointLight() {

}
