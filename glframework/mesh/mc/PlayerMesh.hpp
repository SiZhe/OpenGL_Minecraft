#pragma once
#include "../Mesh.hpp"
#include "../mc/TakeCubeMesh.hpp"
#include "../../minecraft/CreateWorld.hpp"
#include "../../glframework/Scene.hpp"
#include "../../glframework/Core.hpp"

class PlayerMesh : public Mesh{
public:
    PlayerMesh(const std::string& name);
    ~PlayerMesh();

    void init();

    void takeCubeInit();

    void draw(const glm::vec3& pos, bool isMy);
    // 人物放置消除动画
    void placeAnimation(bool &placeAnimation);
    // 人物手持切换动画
    void takeCubeAnimation(CubeType nowTakeCube);

    // 仰角
    float eyeRadians = 0;
    // 朝向
    float bodyRadians = 0;

    // 动画状态
    enum class Animation
    {
        GO,
        Return,
        End,
    };

    // 第一人 称手持方块绘制
    void firstPersonDrawTakeCube();
    // 放置消除动画
    Animation placeCubeAnimation = Animation::End;
    // 放置进度
    float placeValue = 0;
    // 手持切换动画
    Animation takeAnimation = Animation::End;
    // 切换进度
    float takeValue = 0;
    float takeSpeed {0.3f};
    // 现在手持方块
    CubeType nowTakeCube = CubeType::Dirt;
    // 更换手持方块
    CubeType newTakeCube = CubeType::Dirt;

    // 人物上次位置
    glm::vec3 oldPos;
    // 手臂摆动角度
    float limbRadians = 0.0f;
    // 手臂摆动方向
    bool isLimbDir = false;

    // 是否正在放置/破坏状态
    bool mIsPlacing = false;
//private:
    std::string mName;
    // 手持方块
    std::unordered_map<CubeType,std::unique_ptr<TakeCubeMesh>> takeCubeMeshes;

    // 跑步动画
    void runAnimation(const glm::vec3& pos);

    // 放置进度改变
    void placeValueChange();
    // 切换进度改变
    void takeValueChange();
};
