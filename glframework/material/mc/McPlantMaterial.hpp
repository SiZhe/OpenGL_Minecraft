#pragma once
#include "../Material.hpp"
#include "../../Texture.hpp"

class McPlantMaterial : public Material{
public:
    McPlantMaterial();
    ~McPlantMaterial();

    Texture* mDiffuse{nullptr};
    Texture* mOpacityMask{nullptr};
    float mShininess{1.0f};
};
