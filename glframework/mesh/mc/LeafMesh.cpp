#include "LeafMesh.hpp"
#include "../../material/mc/McPlantMaterial.hpp"

LeafMesh::LeafMesh(const std::string &name) {
    mType = ObjectType::InstancedMesh;
    mName = name;
    mInstanceMatrices.resize(mInstanceCount);
}

LeafMesh::~LeafMesh() {

}

void LeafMesh::init() {
    mGeometry = Geometry::CreateBox(1.0f, 1.0f,1.0f);
    instanceInit();

    std::unique_ptr<McPlantMaterial> leafMat = std::make_unique<McPlantMaterial>();

    std::string diffPath = "../assets/textures/leaf/" + mName +".png";
    leafMat->mDiffuse = Texture::createTexture(diffPath,0);
    // std::string opaPath = "../assets/textures/leaf/" + mName +"Mask.png";
    // leafMat->mOpacityMask = Texture::createTexture(opaPath,1);
    // leafMat->mBlendTest = true;
    // leafMat->mDepthWrite = false;
    //leafMat->mFaceCulling = true;
    mMaterial = leafMat.release();
}

void LeafMesh::push(int x, int y, int z) {
    mInstanceCount++;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, y, z));

    mInstanceMatrices.push_back(model);
}