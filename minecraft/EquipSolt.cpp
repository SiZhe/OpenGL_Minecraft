#include "EquipSolt.hpp"

#include "Bag.hpp"

EquipSolt* EquipSolt::mInstance{nullptr};

EquipSolt::EquipSolt() {
	for (int i = 0 ; i < equipMeshes.size(); i++) {
		CubeType type = mcBag->getSlotCubeType( i +1 );
		equipMeshes[i] = std::make_unique<BagCubeMesh>(mcBag->cubeTypeToString(type));
	}
}

EquipSolt::~EquipSolt() {

}

EquipSolt *EquipSolt::getInstance() {
	if (mInstance == nullptr) {
		mInstance = new EquipSolt();
	}
	return mInstance;
}

void EquipSolt::init() {
	// 初始化装备条
	equipBar->init(glm::vec3(0.0f,mYPosition,0.0f),glm::vec2(0.975f,0.125f));

	// 初始化装备框
	equipSlot->init(glm::vec3(-0.428f,0.0f,-0.6f),glm::vec2(0.125f,0.125f));
	// 将装备框加入到装备条中
	equipBar->addChild(equipSlot.get());

	// 初始化装备icon
	for (int i = 0; i < 9; i++) {
		// 这里是相对与[装备条]的位置:父亲是装备条
		equipMeshes[i]->init(glm::vec3(float(i) * 0.107f - 0.428f,0,-0.5f));
		// 将装备icon加入到装备条中
		equipBar->addChild(equipMeshes[i].get());
	}
}

//移动背包改变
void EquipSolt::updateIcon() {
	for (int i = 0; i < 9; i++) {
		equipMeshes[i]->updateTexture(mcBag->cubeTypeToString(mcBag->getSlotCubeType(i+1)));
	}
}

// 滚轮和数字调用
void EquipSolt::updateSlot(int i) {
	equipSlot->setPosition(glm::vec3(float(i-1) * 0.107f - 0.428f,0.0f,-0.5f));
}

void EquipSolt::addScene(Scene* sceneOffScreen) {
	sceneOffScreen->addChild(equipBar.get());
}

void EquipSolt::removeFromScene(Scene* sceneOffScreen) {
	sceneOffScreen->removeChild(equipBar.get());
}