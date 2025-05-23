#include "BagMesh.hpp"
#include "../../glframework/material/mc/McBagMaterial.hpp"

BagMesh::BagMesh(const std::string &name) {
    mType = ObjectType::Mesh;
    mName = name;
}

BagMesh::~BagMesh() {

}

void BagMesh::init(glm::vec3 screenPosition,glm::vec2 geometrySize) {
    mScreenPosition = screenPosition;

    mGeometry = Geometry::CreatePlane(geometrySize.x, geometrySize.y);

    std::string diffPath = "../assets/textures/bag/" + mName + ".png";
    auto crossMat = std::make_unique<McBagMaterial>();
    crossMat->mDiffuse = Texture::createTexture(diffPath, 0);
    crossMat->mDepthWrite = false;
    mMaterial = crossMat.release();

    this->setPosition(mScreenPosition);
}