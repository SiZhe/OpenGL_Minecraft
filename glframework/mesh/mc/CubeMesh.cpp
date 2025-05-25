#include "CubeMesh.hpp"
#include "../../material/mc/McCubeMaterial.hpp"

CubeMesh::CubeMesh(const std::string &name) {
    mType = ObjectType::InstancedMesh;
    mName = name;
    mInstanceMatrices.resize(mInstanceCount);
}

CubeMesh::~CubeMesh() {

}

void CubeMesh::init() {
    mGeometry = Geometry::CreateMcCube(1.0f);
    instanceInit();

    auto cubeMat = std::make_unique<McCubeMaterial>();
    std::string diffPath = "../assets/textures/cube/" + mName ;

    for (int i = 0; i < 6; i++) {
        mDiffPath.push_back(diffPath + face[i]);
    }

    cubeMat->mDiffuse = Texture::createMcCubeTexture(mDiffPath,0);
    cubeMat->mSpecularMask = Texture::createTexture("../assets/textures/defaultMask.png",1);
    //cubeMat->mFaceCulling = true;
    mMaterial = cubeMat.release();
}

void CubeMesh::push(int x, int y, int z) {
    mInstanceCount++;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, y, z));

    mInstanceMatrices.push_back(model);
}
