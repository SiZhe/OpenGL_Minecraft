#pragma once
#include "../Material.hpp"
#include "../../Texture.hpp"

//cube实际用的phong材质
class McTakeCubeMaterial : public Material{
public:
    McTakeCubeMaterial();
    ~McTakeCubeMaterial();

    Texture* mDiffuse{nullptr};
    float mShininess{1.0f};
};

