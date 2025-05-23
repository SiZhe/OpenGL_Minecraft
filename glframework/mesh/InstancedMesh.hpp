#pragma once
#include "Mesh.hpp"

class InstancedMesh : public Mesh{
public:
    InstancedMesh();
    InstancedMesh(Geometry* geometry, Material* material);
    InstancedMesh(Geometry* geometry, Material* material,unsigned int instanceCount);
    ~InstancedMesh();

    void clear();

    void instanceInit();

    unsigned int mInstanceCount{};
    std::vector<glm::mat4> mInstanceMatrices;

    //draw之前的准备 还要glDrawElementsInstanced
    void upDataInstanceMatrices();

    unsigned int mMatrixVbo {0};

};
