#pragma once
#include "../InstancedMesh.hpp"

class CubeMesh : public InstancedMesh{
public:
    CubeMesh(const std::string& name);
    ~CubeMesh();

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

