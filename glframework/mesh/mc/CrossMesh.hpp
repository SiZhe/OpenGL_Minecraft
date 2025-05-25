#pragma once
#include "../Mesh.hpp"

class CrossMesh :public Mesh{
public:
    CrossMesh(const std::string& name);
    ~CrossMesh();

    void init();
private:
    std::string mName;
};