#pragma once

#include "Bag.hpp"
#include "../glframework/Core.hpp"
#include "CreateWorld.hpp"
#include "../glframework/mesh/mc/EliminateMesh.hpp"
#include "../glframework/mesh/mc/DropMesh.hpp"
#include "../glframework/mesh/mc/DropStencilMesh.hpp"
#include "../glframework/mesh/mc/CubeBreakMesh.hpp"
#include "../glframework/mesh/mc/CubeFrameMesh.hpp"
#include "../glframework/Scene.hpp"

#define mcEliminateA EliminateAnima::getInstance()

class EliminateAnima {
public:
    static EliminateAnima* getInstance();
    ~EliminateAnima();

    void init();
    // 爆炸动画的位置
    void push(glm::vec3 vec, CubeType cubeType);
    // 渲染所有的爆炸动画
    void draw();

    void updateEliminateCube();
    void switchAndUpdateBreakStage(int stage,const glm::vec3& pos);
    void updateFrame(const glm::vec3 &pos);
    void clearAll();

    void addScene(Scene* sceneOffScreen);
    void removeFromScene(Scene* sceneOffScreen);

    float dropCubeLifetime{7.0f};

    float placeSpeed{0.2f};
private:
    EliminateAnima();
    static EliminateAnima* mInstance;

    // 方块碎末结构体
    struct OffscumStruct
    {
        // 碎末位置
        glm::vec3 pos;
        // 移动角度
        int angle;
        // 移动速度
        float speed;
        // 碎末开始高度
        float begin;

        // 向上移动
        bool isUp;
        // 是否绘制
        bool isDraw;
        // 爆炸方块类型
        CubeType cubeType;

        //留存时间
        float lifetime;

        OffscumStruct(glm::vec3 vec, int angle, float speed, CubeType cubeType):
            pos(std::move(vec)),
            angle(angle),
            speed(speed),
            isUp(true),
            isDraw(true),
            cubeType(cubeType),
            lifetime(0.0f) {
            begin = vec.y + 1;
        }
    };

    // 爆炸的方块位置集合
    using OffscumVec = std::vector<std::vector<OffscumStruct>>;
    std::list<OffscumVec> offscumVecList;

    std::list<OffscumStruct> dropList;
    // 破坏方块程度贴图 0-7是stage 8是空白
    std::array<std::unique_ptr<CubeBreakMesh>,9> breakStageMeshes;

    std::unordered_map<CubeType,std::unique_ptr<EliminateMesh>> eliminateMeshes;
    std::unordered_map<CubeType,std::unique_ptr<DropMesh>> dropMeshes;
    std::unique_ptr<DropStencilMesh> dropStencil ;

    std::unique_ptr<CubeFrameMesh> frameMesh;

    float offscumPosRand();
    void switchEliminateDraw(OffscumStruct &offstr);
    void switchDropDraw(OffscumStruct &offstr);
};

