#pragma once
#include "../Core.hpp"
#include "Material.hpp"
#include "../texture.hpp"

class CubeMaterial : public Material{
public:
    CubeMaterial();
    ~CubeMaterial();

    Texture* mCubeTexture{nullptr};
};
