#pragma once
#include "../InstancedMesh.hpp"

class EliminateMesh : public InstancedMesh {
public:
    EliminateMesh(const std::string& name);
    ~EliminateMesh();

    void init();

    void upDateShader(const glm::vec3& pos);

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
