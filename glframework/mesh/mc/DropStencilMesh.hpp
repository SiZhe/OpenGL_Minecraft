#pragma once
#include "../InstancedMesh.hpp"

class DropStencilMesh : public InstancedMesh{
public:
    DropStencilMesh();
    ~DropStencilMesh();

    void init();

    void upDateShader(const glm::vec3& pos,const float& angle);
};

