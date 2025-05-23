#pragma once
#include "../Material.hpp"
#include "../../Texture.hpp"

class McBagCubeMaterial : public Material{
public:
    McBagCubeMaterial();
    ~McBagCubeMaterial();

    Texture* mDiffuse {nullptr};
};

