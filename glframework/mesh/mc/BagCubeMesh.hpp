#pragma once
#include "../Mesh.hpp"
#include "../../minecraft/CreateWorld.hpp"
#include "../../glframework/Texture.hpp"

class BagCubeMesh : public Mesh{
public:
    BagCubeMesh(const std::string& name);
    ~BagCubeMesh();
    void init(glm::vec3 screenPosition);

    void updateTexture(const std::string& newName);
    std::string mName;
private:
    glm::vec3 mScreenPosition;
};