#pragma once
#include "../Material.hpp"
#include "../../Texture.hpp"
#include <memory>

//cube实际用的phong材质
class McCubeMaterial : public Material{
public:
    McCubeMaterial();
    ~McCubeMaterial();

    Texture* mDiffuse{nullptr};
    Texture* mSpecularMask{nullptr};
    float mShininess{1.0f};
};