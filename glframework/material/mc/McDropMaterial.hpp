#pragma once
#include "../Material.hpp"
#include "../../Texture.hpp"

class McDropMaterial : public Material {
public:
    McDropMaterial();
    ~McDropMaterial();

    Texture* mDiffuse{nullptr};
};

