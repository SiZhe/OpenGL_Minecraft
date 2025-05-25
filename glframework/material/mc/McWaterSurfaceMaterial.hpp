#pragma once
#include "../Material.hpp"
#include "../../Texture.hpp"

class McWaterSurfaceMaterial : public Material{
public:
	McWaterSurfaceMaterial();
	~McWaterSurfaceMaterial();

	Texture* mDiffuse {nullptr};
	Texture* mSpecularMask{nullptr};
	float mShininess{1.0f};
};
