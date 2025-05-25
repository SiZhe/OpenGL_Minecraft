#include "TakeCubeMesh.hpp"

#include "../../../application/camera/McGameCameraControl.hpp"
#include "../../glframework/material/mc/McTakeCubeMaterial.hpp"

TakeCubeMesh::TakeCubeMesh(const std::string &name) {
    mType = ObjectType::Mesh;
    mName = name;
}

TakeCubeMesh::~TakeCubeMesh() {

}

void TakeCubeMesh::init() {
    mGeometry = Geometry::CreateFirPlayerTakeCube();

    auto cubeMat = std::make_unique<McTakeCubeMaterial>();
    std::string diffPath = "../assets/textures/cube/" + mName;
    for (int i = 0; i < 6; i++) {
        mDiffPath.push_back(diffPath + face[i]);
    }

    cubeMat->mDiffuse = Texture::createMcCubeTexture(mDiffPath,0);
    mMaterial = cubeMat.release();
}

void TakeCubeMesh::upDataShaderFir(float eyeRadians, float bodyRadians, float limbRadians, float placeValue, float takeValue) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, mcControl->mCamera->mPosition + glm::cross(mcControl->mCamera->mUp, mcControl->mCamera->mRight) * 0.85f);
    model = glm::rotate(model, glm::radians(bodyRadians - 22 + placeValue), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(eyeRadians - placeValue * 0.5f + takeValue), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(limbRadians * 0.025f), glm::vec3(0.0f, 0.0f, 1.0f));
    mModelMatrix = model;
}


