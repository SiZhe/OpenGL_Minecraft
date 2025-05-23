#include "DropStencilMesh.hpp"
#include "../../material/mc/McDropStencilMaterial.hpp"

DropStencilMesh::DropStencilMesh() {
    mType = ObjectType::InstancedMesh;
    mInstanceMatrices.resize(mInstanceCount);
}

DropStencilMesh::~DropStencilMesh() {

}

void DropStencilMesh::init() {
    mGeometry = Geometry::CreateMcCube(0.34f);
    instanceInit();

    //模板
    mMaterial = new McDropStencilMaterial();
    //mMaterial->mDepthTest = false;
    mMaterial->mStencilTest = true;
    mMaterial->mSFail = GL_KEEP; //模板检测通过怎么办
    mMaterial->mZFail = GL_KEEP; //模板检测通过，但深度检测不通过怎么办
    mMaterial->mZPass = GL_KEEP; //模板检测通过，深度检测通过怎么办
    mMaterial->mStencilMask = 0x00;
    mMaterial->mStencilFunc = GL_NOTEQUAL;
    mMaterial->mStencilRef = 1;
    mMaterial->mStencilFuncMask = 0xFF;
}

void DropStencilMesh::upDateShader(const glm::vec3& pos,const float& angle) {
    mInstanceCount++;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));
    mInstanceMatrices.push_back(model);
}