#pragma once
#include "../InstancedMesh.hpp"

class WaterSurfaceMesh : public InstancedMesh {
public:
	WaterSurfaceMesh(const std::string& name);
	~WaterSurfaceMesh();

	void init();

	void push(int x, int y, int z);

	std::string mName;
private:
	std::string face[6] = { "/right.png",
	"/left.png",
	"/top.png",
	"/bottom.png",
	"/front.png",
	"/back.png" };

	std::vector<std::string> mDiffPath ;
};

