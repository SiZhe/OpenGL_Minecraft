#pragma once
#include "../InstancedMesh.hpp"

class LeafMesh : public InstancedMesh {
public:
    LeafMesh(const std::string& name);
    ~LeafMesh();

    void init();

    void push(int x, int y, int z);

private:
    std::string mName;
};

