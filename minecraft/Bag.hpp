#pragma once
#include "../glframework/mesh/mc/BagMesh.hpp"
#include "../glframework/mesh/mc/BagCubeMesh.hpp"
#include "../glframework/Scene.hpp"
#include "../glframework/Core.hpp"

#define mcBag Bag::getInstance()

class Bag {
public:
	static Bag* getInstance();
    ~Bag();

    void init();
	// 载入背包icon
    void loadBagCube();
	// 载入装备icon
	void loadEquipCube();
	//cubeType 返回 String
    std::string cubeTypeToString(CubeType ct);
	//拖拽图标
	void dragCube();

	// 加入场景，背包的icon加入到背包背景中
    void addScene(Scene* sceneOffScreen);
    void removeFromScene(Scene* sceneOffScreen);

	// 拖拽物品
	CubeType dragType;
	std::shared_ptr<BagCubeMesh> dragItem {nullptr};
	glm::vec2 dragOffset{};
	bool isDragging {false};
	bool isDraggingFromEquip {false};
	int dragStartIndex{-1};

	// 外界得到 CubeType
	CubeType getSlotCubeType (int i) const;

	CubeType bagIcon[5][9] = {
		// 第一行
		{ CubeType::Dirt, CubeType::Sand, CubeType::Stone, CubeType::Bricks, CubeType::OakPlanks,
		  CubeType::Bedrock, CubeType::Glowstone, CubeType::StoneBrick, CubeType::GrassBlock,},

		// 第二行
		{CubeType::OakLog, CubeType::Snow, CubeType::LeaveGreen,CubeType::LeaveRed,
			CubeType::LeaveYellow, CubeType::LeaveWhite, CubeType::SandStone, CubeType::Ice,CubeType::BirchPlanks,},

		// 第三行
		{ CubeType::BirchLog, CubeType::BlockOfDiamond, CubeType::BlockOfGold,CubeType::BlockOfEmerald,
			CubeType::BlockOfLapis, CubeType::BlockOfIron, CubeType::BlockOfQuartz, CubeType::BlockOfRedstone, CubeType::DiamondOre, },

		// 第四行
		{CubeType::EmeraldOre, CubeType::CoalOre, CubeType::GoldOre,CubeType::IronOre,
			CubeType::LapisLazuliOre, CubeType::RedstoneOre, CubeType::RedWool, CubeType::YellowWool,CubeType::WhiteWool},

		// 第五 行
		{ CubeType::BlackWool, CubeType::Bookshelf, CubeType::CarvedPumpkin,CubeType::CraftingTable,
			CubeType::Furnace, CubeType::Glass, CubeType::HayBale, CubeType::Melon, CubeType::TNT }
	};
private:
	static Bag* mInstance;
	Bag();
	// 背包板
    std::unique_ptr<BagMesh> bagBackground = std::make_unique<BagMesh>("bag");
	// 背包图标
	std::map<CubeType,std::shared_ptr<BagCubeMesh>> bagCube;
	// 背包装备栏信息（9个槽位）
	std::array<CubeType, 9> equipSlot;  // 存放的物品类型
	std::array<glm::vec3, 9> equipPosition;  // 装备栏位置
	std::array<std::shared_ptr<BagCubeMesh>, 9> equipCube;  // 装备栏图标
};
