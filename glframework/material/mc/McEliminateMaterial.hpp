#pragma once
#include "../Material.hpp"
#include "../../Texture.hpp"

class McEliminateMaterial : public Material{
public:
    McEliminateMaterial();
    ~McEliminateMaterial();

    Texture* mDiffuse{nullptr};
};
