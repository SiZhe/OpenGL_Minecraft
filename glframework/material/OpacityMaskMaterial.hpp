#pragma once
#include "Material.hpp"
#include "../Texture.hpp"

class OpacityMaskMaterial : public Material {
public:
    OpacityMaskMaterial();
    ~OpacityMaskMaterial();


    Texture* mDiffuse{nullptr};
    Texture* mOpacityMask{nullptr};
    float mShininess{1.0f};

};

