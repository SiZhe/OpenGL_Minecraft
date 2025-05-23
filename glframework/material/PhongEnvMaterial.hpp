#pragma once
#include "Material.hpp"
#include "../Core.hpp"
#include "../Texture.hpp"

class PhongEnvMaterial : public Material{
public:
    PhongEnvMaterial();
    ~PhongEnvMaterial();

    Texture* mDiffuse{nullptr};
    Texture* mSpecularMask{nullptr};
    Texture* mEnv{nullptr};
    float mShininess{1.0f};
};

