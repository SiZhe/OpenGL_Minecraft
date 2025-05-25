#pragma once
#include "../Material.hpp"
#include "../../Texture.hpp"

class McCrossMaterial : public Material{
public:
    McCrossMaterial();
    ~McCrossMaterial();

    Texture* mDiffuse {nullptr};
};

