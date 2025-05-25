#pragma once
#include "../Mesh.hpp"

class CubeFrameMesh : public Mesh{
public:
	CubeFrameMesh(const std::string& name);
	~CubeFrameMesh();

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
