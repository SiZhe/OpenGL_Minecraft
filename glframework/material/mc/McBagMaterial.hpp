#pragma once
#include "../Material.hpp"
#include "../../Texture.hpp"

class McBagMaterial : public Material{
public:
    McBagMaterial();
    ~McBagMaterial();

    Texture* mDiffuse {nullptr};
};
