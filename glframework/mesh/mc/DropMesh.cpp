#include "DropMesh.hpp"
#include "../../material/mc/McDropMaterial.hpp"

DropMesh::DropMesh(const std::string &name) {
    mType = ObjectType::InstancedMesh;
    mName = name;
    mInstanceMatrices.resize(mInstanceCount);
}

DropMesh::~DropMesh() {

}

void DropMesh::init() {
    mGeometry = Geometry::CreateMcCube(0.3f);
    instanceInit();

    auto dropCubeMat = std::make_unique<McDropMaterial>();
    std::string diffPath = "../assets/textures/cube/" + mName ;

    for (int i = 0; i < 6; i++) {
        mDiffPath.push_back(diffPath + face[i]);
    }

    dropCubeMat->mDiffuse = Texture::createMcCubeTexture(mDiffPath,0);

    mMaterial = dropCubeMat.release();
    mMaterial->mStencilTest = true;
    mMaterial->mSFail = GL_KEEP; //模板检测通过怎么办
    mMaterial->mZFail = GL_KEEP; //模板检测通过，但深度检测不通过怎么办
    mMaterial->mZPass = GL_REPLACE; //模板检测通过，深度检测通过怎么办
    mMaterial->mStencilMask = 0xFF;
    mMaterial->mStencilFunc = GL_ALWAYS;
    mMaterial->mStencilRef = 1;
    mMaterial->mStencilFuncMask = 0xFF;
}

void DropMesh::upDateShader(const glm::vec3& pos,const float& angle) {
    mInstanceCount++;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));
    mInstanceMatrices.push_back(model);
}