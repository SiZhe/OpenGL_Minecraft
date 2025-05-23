#pragma once
#include "../Material.hpp"
#include "../../Texture.hpp"

class McCubeBreakMaterial : public Material{
public:
	McCubeBreakMaterial();
	~McCubeBreakMaterial();

	Texture* mDiffuse {nullptr};
};

