#include "Bag.hpp"

#include "EquipSolt.hpp"
#include "../application/Application.hpp"
#include "../application/camera/McGameCameraControl.hpp"

Bag* Bag::mInstance{nullptr};

Bag *Bag::getInstance() {
    if (mInstance == nullptr) {
        mInstance = new Bag();
    }
    return mInstance;
}

Bag::Bag() {
    loadBagCube();
    loadEquipCube();
}

Bag::~Bag() {
    delete mInstance;
}


void Bag::loadBagCube() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 9; j++) {
            CubeType type = bagIcon[i][j];
            std::string name = cubeTypeToString(type);
            bagCube[type] = std::make_shared<BagCubeMesh>(name);
        }
    }
}

void Bag::loadEquipCube() {
    for (int i = 0; i < 9; i++) {
        CubeType type = bagIcon[0][i];
        std::string name = cubeTypeToString(type);
        equipCube[i] = std::make_shared<BagCubeMesh>(name);
    }
}

void Bag::init() {
    bagBackground->init(glm::vec3(0.0f,0.0f,0.0f),glm::vec2(1.0f,0.745f));
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 9; j++) {
            CubeType type = bagIcon[i][j];
            bagCube[type]->init(glm::vec3(float(j) * 0.107 - 0.428,- float(i) * 0.107 + 0.25,-0.5f));
            // 加入到背包背景中
            bagBackground->addChild(bagCube[type].get());
        }
    }

    // 初始化装备栏（9个槽位，index=0~8）
    for (int i = 0; i < 9; i++) {
        equipSlot[i] = bagIcon[0][i];  //第一列

        // 计算装备栏位置（背包正下方，横向排列）
        equipPosition[i] = glm::vec3(
            float(i) * 0.107f - 0.428f,  // X坐标（和背包对齐）
            -0.31f,                      // Y坐标（在背包下方）
            -0.5f                        // Z坐标
        );

        // 初始化 EMPTY 图标
        equipCube[i]->init(equipPosition[i]);
        // 加入到背包背景中
        bagBackground->addChild(equipCube[i].get());
    }
}

std::string Bag::cubeTypeToString(CubeType ct) {
    switch(ct){
        case CubeType::Dirt:          return "Dirt";
        case CubeType::Sand:          return "Sand";
        case CubeType::Stone:         return "Stone";
        case CubeType::Bricks:        return "Bricks";
        case CubeType::OakPlanks:     return "OakPlanks";
        case CubeType::Bedrock:       return "Bedrock";
        case CubeType::Glowstone:     return "Glowstone";
        case CubeType::StoneBrick:    return "StoneBrick";
        case CubeType::GrassBlock:    return "GrassBlock";

        case CubeType::OakLog:        return "OakLog";
        case CubeType::Snow:          return "Snow";
        case CubeType::LeaveGreen:    return "LeaveGreen";
        case CubeType::LeaveRed:      return "LeaveRed";
        case CubeType::LeaveYellow:   return "LeaveYellow";
        case CubeType::LeaveWhite:    return "LeaveWhite";
        case CubeType::SandStone:     return "SandStone";
        case CubeType::Ice:           return "Ice";
        case CubeType::BirchPlanks:   return "BirchPlanks";

        case CubeType::BirchLog:      return "BirchLog";
        case CubeType::BlockOfDiamond: return "BlockOfDiamond";
        case CubeType::BlockOfGold:   return "BlockOfGold";
        case CubeType::BlockOfEmerald: return "BlockOfEmerald";
        case CubeType::BlockOfLapis:  return "BlockOfLapis";
        case CubeType::BlockOfIron:   return "BlockOfIron";
        case CubeType::BlockOfQuartz: return "BlockOfQuartz";
        case CubeType::BlockOfRedstone: return "BlockOfRedstone";
        case CubeType::DiamondOre:    return "DiamondOre";

        case CubeType::EmeraldOre:    return "EmeraldOre";
        case CubeType::CoalOre:       return "CoalOre";
        case CubeType::GoldOre:       return "GoldOre";
        case CubeType::IronOre:       return "IronOre";
        case CubeType::LapisLazuliOre: return "LapisLazuliOre";
        case CubeType::RedstoneOre:   return "RedstoneOre";
        //case CubeType::PinkWool:      return "PinkWool";
        case CubeType::RedWool:       return "RedWool";
        case CubeType::YellowWool:    return "YellowWool";
        case CubeType::WhiteWool:     return "WhiteWool";

        case CubeType::BlackWool:     return "BlackWool";
        case CubeType::Bookshelf:     return "Bookshelf";
        case CubeType::CarvedPumpkin: return "CarvedPumpkin";
        case CubeType::CraftingTable: return "CraftingTable";
        case CubeType::Furnace:       return "Furnace";
        case CubeType::Glass:         return "Glass";
        case CubeType::HayBale:       return "HayBale";
        case CubeType::Melon:         return "Melon";
        case CubeType::TNT:         return "TNT";

        // 植物
        case CubeType::Mushroom:       return "Mushroom";
        case CubeType::RedMushroom:         return "RedMushroom";
        case CubeType::Flower:       return "Flower";
        case CubeType::Grass_1:         return "Grass_1";
        case CubeType::Grass_2:         return "Grass_2";
        case CubeType::Grass_3:         return "Grass_3";
        case CubeType::Null:            return "Null";
        default:
            throw std::runtime_error("Unrecognized cube type: " + std::to_string(static_cast<int>(ct)));
    }
}

void Bag::dragCube() {
    // 只在鼠标按下且未开始拖拽时检测
    if (mcControl->mLeftMouseDown && !isDragging) {
        //检测时候点击装备列表
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 9; j++) {
                CubeType type = bagIcon[i][j];
                glm::vec3 itemPos = bagCube[type]->mPosition;
                float itemSize = 0.085f;

                // 转换鼠标坐标到NDC
                float ndcX = (2.0f * mcControl->mXpos) / glApp->getWidth() - 1.0f;
                float ndcY = 1.0f - (2.0f * mcControl->mYpos) / glApp->getHeight();

                // 检测点击
                if (ndcX >= itemPos.x - itemSize/2 &&
                    ndcX <= itemPos.x + itemSize/2 &&
                    ndcY >= itemPos.y - itemSize/2 &&
                    ndcY <= itemPos.y + itemSize/2) {

                    isDraggingFromEquip = false;
                    dragItem = bagCube[type];
                    dragType = type;

                    // 计算正确的NDC偏移
                    dragOffset = glm::vec2(itemPos.x, itemPos.y) - glm::vec2(ndcX, ndcY);
                    isDragging = true;
                    break;
                    }
            }
        }
        // 检测是否点击了装备栏
        for (int i = 0; i < 9; i++) {
            float slotSize = 0.085f;
            float ndcX = (2.0f * mcControl->mXpos) / glApp->getWidth() - 1.0f;
            float ndcY = 1.0f - (2.0f * mcControl->mYpos) / glApp->getHeight();

            if (ndcX >= equipPosition[i].x - slotSize/2 &&
                ndcX <= equipPosition[i].x + slotSize/2 &&
                ndcY >= equipPosition[i].y - slotSize/2 &&
                ndcY <= equipPosition[i].y + slotSize/2) {
                dragStartIndex = i; // 记录原始位置

                // 记录是从装备栏开始拖拽
                isDraggingFromEquip = true;
                dragItem = equipCube[i];
                dragType = equipSlot[i];

                // 计算偏移量
                dragOffset = glm::vec2(equipPosition[i].x, equipPosition[i].y) -
                            glm::vec2(ndcX, ndcY);
                isDragging = true;
                return;
                }
            }
        }
    // 拖拽过程中更新位置
    else if (isDragging && mcControl->mLeftMouseDown) {
        float ndcX = (2.0f * mcControl->mXpos) / glApp->getWidth() - 1.0f;
        float ndcY = 1.0f - (2.0f * mcControl->mYpos) / glApp->getHeight();

        dragItem->setPosition( glm::vec3(ndcX + dragOffset.x,ndcY + dragOffset.y,
            -1.0f));  // 提高Z值确保显示在最上层
    }
    // 释放鼠标更新位置
    else if (!mcControl->mLeftMouseDown && isDragging) {
        isDragging = false;

        // 转换鼠标坐标到NDC
        float ndcX = (2.0f * mcControl->mXpos) / glApp->getWidth() - 1.0f;
        float ndcY = 1.0f - (2.0f * mcControl->mYpos) / glApp->getHeight();

        bool placedInValidSlot = false;
        // 如果是从装备栏拖拽的
        if (isDraggingFromEquip) {
            // 只能在装备栏内交换
            // 检查是否拖拽到其他装备槽
            for (int i = 0; i < 9; i++) {
                float slotSize = 0.085f;
                if (ndcX >= equipPosition[i].x - slotSize/2 &&
                    ndcX <= equipPosition[i].x + slotSize/2 &&
                    ndcY >= equipPosition[i].y - slotSize/2 &&
                    ndcY <= equipPosition[i].y + slotSize/2) {

                    // 只交换贴图，不改变实际位置
                    std::string tempTexture = equipCube[i]->mName;
                    equipCube[i]->updateTexture(equipCube[dragStartIndex]->mName);
                    equipCube[dragStartIndex]->updateTexture(tempTexture);

                    // 交换数据
                    CubeType temp = equipSlot[i];
                    equipSlot[i] = equipSlot[dragStartIndex];
                    equipSlot[dragStartIndex] = temp;
                    placedInValidSlot = true;
                    break;
                    }
            }
            // 无论是否交换，都回到原位（位置不变，只是重置Z轴）
            dragItem->setPosition(glm::vec3(
                equipPosition[dragStartIndex].x,
                equipPosition[dragStartIndex].y,
                -0.5f  // 恢复原始Z值
            ));
        }
        else{
            // 检查是否拖拽到了装备栏
            for (int i = 0; i < 9; i++) {
                float slotSize = 0.085f;

                // 检测是否在装备栏范围内
                if (ndcX >= equipPosition[i].x - slotSize/2 &&
                    ndcX <= equipPosition[i].x + slotSize/2 &&
                    ndcY >= equipPosition[i].y - slotSize/2 &&
                    ndcY <= equipPosition[i].y + slotSize/2) {

                    // 更新装备栏
                    equipSlot[i] = dragType;
                    std::string name = cubeTypeToString(equipSlot[i]);
                    // 更新装备栏图标
                    equipCube[i]->updateTexture(name);
                    placedInValidSlot = true;
                    break;
                    }
            }
        }
        // 回到原位
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 9; j++) {
                if (bagCube[bagIcon[i][j]] == dragItem) {
                    dragItem->setPosition(glm::vec3(
                        float(j) * 0.107 - 0.428,
                        -float(i) * 0.107 + 0.25,
                        -0.5f
                    ));
                    break;
                }
            }
        }
        // 每次释放鼠标都更新装备条
        mcEquipS->updateIcon();
    }
}

void Bag::addScene(Scene* sceneOffScreen) {
    sceneOffScreen->addChild(bagBackground.get());
}

void Bag::removeFromScene(Scene* sceneOffScreen) {
    sceneOffScreen->removeChild(bagBackground.get());
}

CubeType Bag::getSlotCubeType(int i) const {
    return equipSlot[i - 1];
}
