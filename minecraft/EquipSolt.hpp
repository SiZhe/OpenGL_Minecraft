#pragma once
#include "../glframework/Core.hpp"
#include "../glframework/Scene.hpp"
#include "../glframework/mesh/mc/BagMesh.hpp"
#include "../glframework/mesh/mc/BagCubeMesh.hpp"
#include "CreateWorld.hpp"

#define mcEquipS EquipSolt::getInstance()

class EquipSolt {
public:
	static EquipSolt* getInstance();
	~EquipSolt();

	void init();

	void updateIcon();
	void updateSlot(int i);

	// 加入场景，背包的icon加入到背包背景中
	void addScene(Scene* sceneOffScreen);
	void removeFromScene(Scene* sceneOffScreen);

private:
	static EquipSolt* mInstance;
	EquipSolt();

	float mYPosition {-0.7f};
	// 装备条
	std::unique_ptr<BagMesh> equipBar = std::make_unique<BagMesh>("equipBar");
	// 装备框
	std::unique_ptr<BagMesh> equipSlot = std::make_unique<BagMesh>("equipSlot");
	// 装备icon
	std::array<std::unique_ptr<BagCubeMesh>,9> equipMeshes;
};
