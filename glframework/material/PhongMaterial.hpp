#pragma once
#include "../Core.hpp"
#include "Material.hpp"
#include "../Texture.hpp"

class PhongMaterial :public Material {
    public:
    PhongMaterial();
    ~PhongMaterial();

    Texture* mDiffuse{nullptr};
    Texture* mSpecularMask{nullptr};
    float mShininess{1.0f};

};
