#pragma once
#include "../Mesh.hpp"

class CubeBreakMesh : public Mesh{
public:
	CubeBreakMesh(const std::string& name);
	~CubeBreakMesh();

	void init();

	void upDateShader(const glm::vec3& pos);
private:
	std::string mName;

	std::string face[6] = { "/right.png",
	"/left.png",
	"/top.png",
	"/bottom.png",
	"/front.png",
	"/back.png" };

	std::vector<std::string> mDiffPath ;
};
