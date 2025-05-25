#pragma once
#include "../InstancedMesh.hpp"

class PlantMesh : public InstancedMesh {
public:
    PlantMesh(const std::string& name);
    ~PlantMesh();

    void init();

    void push(int x, int y, int z);

//private:
    std::string mName;
};
