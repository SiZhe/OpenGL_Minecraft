#pragma once
#include "material.hpp"
#include "../texture.hpp"

class SphereMaterial : public Material{
public:
    SphereMaterial();
    ~SphereMaterial();

    Texture* mSphereTexture{nullptr};
};

