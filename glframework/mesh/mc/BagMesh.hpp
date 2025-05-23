#pragma once
#include "../Mesh.hpp"

class BagMesh : public Mesh {
public:
    BagMesh(const std::string& name);
    ~BagMesh();

    void init(glm::vec3 screenPosition, glm::vec2 geometrySize);

private:
    std::string mName;
    glm::vec3 mScreenPosition;
};