#pragma once

#include "../glframework/mesh/mc/CubeMesh.hpp"
#include "../glframework/mesh/mc/LeafMesh.hpp"
#include "../glframework/mesh/mc/PlantMesh.hpp"
#include "../glframework/mesh/mc/PlayerMesh.hpp"
#include "../glframework/mesh/mc/WaterSurfaceMesh.hpp"
#include "CreateWorld.hpp"
#include "../glframework/Scene.hpp"
#include "../glframework/Core.hpp"

#define VISION_WIDTH	50		// 视野宽度
#define VISION_HEIGHT	30		// 视野高度

class Painter {
public:
    Painter();
    ~Painter();

    void init();

    std::unordered_map<CubeType,std::unique_ptr<CubeMesh>> cubeMeshes;
    std::unordered_map<CubeType,std::unique_ptr<PlantMesh>> plantMeshes;
	std::array<std::unique_ptr<PlayerMesh>, 1> playerMeshes;

	std::unique_ptr<WaterSurfaceMesh> waterSurfaceMesh;

    void drawMap(glm::vec3 vec);
    void upDataCube();
    void upDataPlant();
    void upDataLeaf();
    void clearAll();
    void push(CubeType cubeType,int type, int i, int j, int k);
    inline bool isDrawCube(int x, int y, int z);

    void addScene(Scene* sceneOffScreen);
    void removeFromScene(Scene* sceneOffScreen);
};

