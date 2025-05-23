#include "PlantMesh.hpp"
#include "../../material/mc/McPlantMaterial.hpp"

PlantMesh::PlantMesh(const std::string & name) {
    mType = ObjectType::InstancedMesh;
    mName = name;
    mInstanceMatrices.resize(mInstanceCount);
}

PlantMesh::~PlantMesh() {

}

void PlantMesh::init() {
    mGeometry = Geometry::CreatePlane(1.0f, 1.0f);
    instanceInit();
    auto plantMat = std::make_unique<McPlantMaterial>();

    std::string diffPath = "../assets/textures/plant/" + mName +".png";
    plantMat->mDiffuse = Texture::createTexture(diffPath,0);

    // std::string opaPath = "../assets/textures/plant/" + mName +"Mask.png";
    // plantMat->mOpacityMask = Texture::createTexture(opaPath,1);
    // plantMat->mBlendTest = true;
    // plantMat->mDepthWrite = false;
    mMaterial = plantMat.release();
    //delete plantMat;
}

void PlantMesh::push(int x, int y, int z) {
    mInstanceCount++;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, { x, y, z});
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    mInstanceMatrices.push_back(model);

    mInstanceCount++;
    model = glm::mat4(1.0f);
    model = glm::translate(model, { x, y, z});
    model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    mInstanceMatrices.push_back(model);
}
