#include "BagCubeMesh.hpp"
#include "../../glframework/material/mc/McBagCubeMaterial.hpp"

BagCubeMesh::BagCubeMesh(const std::string &name) {
    mType = ObjectType::Mesh;
    mName = name;
}

BagCubeMesh::~BagCubeMesh() {

}

void BagCubeMesh::init(glm::vec3 screenPosition) {
    mScreenPosition = screenPosition;
    mGeometry = Geometry::CreatePlane(0.108f,0.118f);

    std::string diffPath = "../assets/textures/icon/" + mName + ".png";
    auto bagCubeMat = std::make_unique<McBagCubeMaterial>();
    bagCubeMat->mDiffuse = Texture::createTexture(diffPath, 0);
    mMaterial = bagCubeMat.release();

    this->setScale(glm::vec3(0.75f));
    this->setPosition(mScreenPosition);
}

void BagCubeMesh::updateTexture(const std::string& newName) {
    if (mMaterial) {
        mName = newName;
        std::string diffPath = "../assets/textures/icon/" + mName + ".png";
        auto newBagCubeMat = std::make_unique<McBagCubeMaterial>();
        newBagCubeMat->mDiffuse = Texture::createTexture(diffPath, 0);
        mMaterial = newBagCubeMat.release();
    }else {
        std::cout << "BagCubeMesh::updateTexture: No material exists!" << std::endl;
    }
}