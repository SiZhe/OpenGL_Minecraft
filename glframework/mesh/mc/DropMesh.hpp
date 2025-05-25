#pragma once
#include "../InstancedMesh.hpp"

class DropMesh : public InstancedMesh{
public:
    DropMesh(const std::string& name);
    ~DropMesh();

    void init();

    void upDateShader(const glm::vec3& pos,const float& angle);
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

