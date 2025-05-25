#pragma once
#include "../Core.hpp"
#include "Material.hpp"
#include "../Texture.hpp"

class ScreenMaterial : public Material{
public:
    ScreenMaterial();
    ~ScreenMaterial();

    Texture* mScreenTexture{nullptr};
};

