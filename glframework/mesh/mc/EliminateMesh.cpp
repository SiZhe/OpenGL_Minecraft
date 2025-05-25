#include "EliminateMesh.hpp"
#include "../../material/mc/McEliminateMaterial.hpp"

EliminateMesh::EliminateMesh(const std::string &name) {
    mType = ObjectType::InstancedMesh;
    mName = name;
    mInstanceMatrices.resize(mInstanceCount);
}

EliminateMesh::~EliminateMesh() {

}

void EliminateMesh::init() {
    mGeometry = Geometry::CreateMcCube(0.1f);
    instanceInit();

    auto cubeMat = std::make_unique<McEliminateMaterial>();
    std::string diffPath = "../assets/textures/cube/" + mName;

    for (int i = 0; i < 6; i++) {
        mDiffPath.push_back(diffPath + face[i]);
    }

    cubeMat->mDiffuse = Texture::createMcCubeTexture(mDiffPath,0);
    //cubeMat->mFaceCulling = true;
    mMaterial = cubeMat.release();
}

void EliminateMesh::upDateShader(const glm::vec3& pos) {
    mInstanceCount++;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    mInstanceMatrices.push_back(model);
}
