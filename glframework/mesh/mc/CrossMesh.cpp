#include "CrossMesh.hpp"
#include "../../material/mc/McCrossMaterial.hpp"

CrossMesh::CrossMesh(const std::string& name) {
    mType = ObjectType::Mesh;
    mName = name;
}

CrossMesh::~CrossMesh() {

}

void CrossMesh::init() {
    mGeometry = Geometry::CreatePlane(0.05f,0.05f);

    std::string diffPath = "../assets/textures/" + mName + ".png";
    auto crossMat = std::make_unique<McCrossMaterial>();
    //crossMat->mDiffuse = Texture::createTexture(diffPath, 0);
    crossMat->mDiffuse = new Texture(diffPath,0);
    mMaterial = crossMat.release();
}
