#include "CubeFrameMesh.hpp"
#include "../../material/mc/McCubeBreakMaterial.hpp"

CubeFrameMesh::CubeFrameMesh(const std::string &name) : mName(name){
	mType = ObjectType::Mesh;
}

CubeFrameMesh::~CubeFrameMesh() {

}

void CubeFrameMesh::init() {
	mGeometry = Geometry::CreateMcCube(1.0f);

	auto cubeMat = std::make_unique<McCubeBreakMaterial>();
	std::string diffPath = "../assets/textures/cube/" + mName ;

	for (int i = 0; i < 6; i++) {
		mDiffPath.push_back(diffPath + face[i]);
	}
	cubeMat->mDiffuse = Texture::createMcCubeTexture(mDiffPath,0);
	mMaterial = cubeMat.release();
}

void CubeFrameMesh::upDateShader(const glm::vec3& pos) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	mModelMatrix = model;
}