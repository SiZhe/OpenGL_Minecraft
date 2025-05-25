#include "Painter.hpp"
#include "Bag.hpp"

Painter::Painter() {
    for (int i = 0 ; i < 5 ;i++) {
        for (int j = 0 ; j < 9 ;j++) {
            cubeMeshes[mcBag->bagIcon[i][j]] = std::make_unique<CubeMesh>(mcBag->cubeTypeToString(mcBag->bagIcon[i][j]));
        }
    }
    //下层水体也是cubeMesh
    cubeMeshes[CubeType::WaterBelow] = std::make_unique<CubeMesh>("WaterBelow");

    waterSurfaceMesh =  std::make_unique<WaterSurfaceMesh>("WaterSurface");

    playerMeshes[0] = std::make_unique<PlayerMesh>("box");

    plantMeshes[CubeType::Mushroom] = std::make_unique<PlantMesh>("Mushroom");
    plantMeshes[CubeType::RedMushroom] = std::make_unique<PlantMesh>("RedMushroom");
    plantMeshes[CubeType::Flower] = std::make_unique<PlantMesh>("Flower");
    plantMeshes[CubeType::Grass_1] = std::make_unique<PlantMesh>("Grass_1");
    plantMeshes[CubeType::Grass_2] = std::make_unique<PlantMesh>("Grass_2");
    plantMeshes[CubeType::Grass_3] = std::make_unique<PlantMesh>("Grass_3");

}

Painter::~Painter() {

}

void Painter::init() {
    for (auto it = cubeMeshes.begin() ; it != cubeMeshes.end() ; it++) {
        it->second->init();
    }

    for (auto it = plantMeshes.begin() ; it != plantMeshes.end() ; it++) {
        it->second->init();
    }

    for (int i = 0 ; i < playerMeshes.size() ;i++) {
        playerMeshes[i]->init();
    }

    waterSurfaceMesh->init();
}

void Painter::drawMap(glm::vec3 vec)
{
    //clearAll();
    //视野
    int begin_x = vec.x - VISION_WIDTH;
    if (begin_x < 0) {
        begin_x = 0;
    }

    int end_x = vec.x + VISION_WIDTH;
    if (end_x > mcWD->mapData.size()) {
        end_x = mcWD->mapData.size();
    }

    int begin_y = vec.y - VISION_HEIGHT;
    if (begin_y < 0) {
        begin_y = 0;
    }

    int end_y = vec.y + VISION_HEIGHT;
    if (end_y > mcWD->mapData[0].size()) {
        end_y = mcWD->mapData[0].size();
    }

    int begin_z = vec.z - VISION_WIDTH;
    if (begin_z < 0) {
        begin_z = 0;
    }

    int end_z = vec.z + VISION_WIDTH;
    if (end_z > mcWD->mapData[0][0].size()) {
        end_z = mcWD->mapData[0][0].size();
    }

    for (size_t x = size_t(begin_x) + 10; x < size_t(end_x) - 10; x++) {
        for (size_t y = size_t(begin_y) + 0; y < size_t(end_y) - 0; y++) {
            for (size_t z = size_t(begin_z) + 10; z < size_t(end_z) - 10; z++) {
                if (isDrawCube(x, y, z)) {
                    push(mcWD->mapData[x][y][z],0, x, y, z);
                }
                push(mcWD->leafData[x][y][z],0, x, y, z);
                push(mcWD->plantData[x][y][z],1, x, y, z);
                push(mcWD->waterSurfaceData[x][y][z],2, x, y, z);
            }
        }
    }
}

void Painter::clearAll()
{
    for (auto it = cubeMeshes.begin() ; it != cubeMeshes.end() ; it++) {
        it->second->clear();
    }
    for (auto it = plantMeshes.begin() ; it != plantMeshes.end() ; it++) {
        it->second->clear();
    }
    waterSurfaceMesh->clear();
}

void Painter::upDataCube() {
    for (auto it = cubeMeshes.begin() ; it != cubeMeshes.end() ; it++) {
        it->second->upDataInstanceMatrices();
    }
    waterSurfaceMesh->upDataInstanceMatrices();
}

void Painter::upDataLeaf() {
    // 现在树叶在cubeMeshes中
    // leaf1->upDataInstanceMatrices();
    // leaf2->upDataInstanceMatrices();
    // leaf3->upDataInstanceMatrices();
    // leaf4->upDataInstanceMatrices();
}

void Painter::upDataPlant() {
    for (auto it = plantMeshes.begin() ; it != plantMeshes.end() ; it++) {
        it->second->upDataInstanceMatrices();
    }
}

void Painter::push(CubeType cubeType,int type, int i, int j, int k)
{
    if (cubeType == CubeType::Null) {
        return;
    }

    if (type == 0) {
        auto it = cubeMeshes.find(cubeType);  // O(1) 查找
        if (it != cubeMeshes.end()) {
            it->second->push(i, j, k);
            return;
        }
    }

    if (type == 1) {
        auto item = plantMeshes.find(cubeType);  // O(1) 查找
        if (item != plantMeshes.end()) {
            item->second->push(i, j, k);
            return;
        }
    }
    if (type == 2) {
        if (cubeType == CubeType::WaterSurface) {
            waterSurfaceMesh->push(i, j, k);
            return;
        }
    }
}

inline bool Painter::isDrawCube(int x, int y, int z)
{
    return  mcWD->mapData[x + 1][y][z] == CubeType::Null ||
        mcWD->mapData[x - 1][y][z] == CubeType::Null ||
        mcWD->mapData[x][y + 1][z] == CubeType::Null ||
        mcWD->mapData[x][y - 1][z] == CubeType::Null ||
        mcWD->mapData[x][y][z + 1] == CubeType::Null ||
        mcWD->mapData[x][y][z - 1] == CubeType::Null;
}

void Painter::addScene(Scene* sceneOffScreen) {

    for (auto it = cubeMeshes.begin() ; it != cubeMeshes.end() ; it++) {
        sceneOffScreen->addChild(it->second.get());
    }

    for (auto it = plantMeshes.begin() ; it != plantMeshes.end() ; it++) {
        sceneOffScreen->addChild(it->second.get());
    }

    for (int i = 0 ; i < playerMeshes.size() ;i++) {
        sceneOffScreen->addChild(playerMeshes[i].get());
    }

    auto it = playerMeshes[0]->takeCubeMeshes.find(playerMeshes[0]->nowTakeCube);
    if (it != playerMeshes[0]->takeCubeMeshes.end()) {
        sceneOffScreen->addChild(it->second.get());
    }

    sceneOffScreen->addChild(waterSurfaceMesh.get());
}

void Painter::removeFromScene(Scene* sceneOffScreen) {

    for (auto it = cubeMeshes.begin() ; it != cubeMeshes.end() ; it++) {
        sceneOffScreen->removeChild(it->second.get());
    }

    for (auto it = plantMeshes.begin() ; it != plantMeshes.end() ; it++) {
        sceneOffScreen->removeChild(it->second.get());
    }

    for (int i = 0 ; i < playerMeshes.size() ;i++) {
        sceneOffScreen->removeChild(playerMeshes[i].get());
    }

    //auto it = playerMeshes[0]->takeCubeMeshes.find(playerMeshes[0]->nowTakeCube);
    for (auto it = playerMeshes[0]->takeCubeMeshes.begin(); it != playerMeshes[0]->takeCubeMeshes.end(); it++) {
        sceneOffScreen->removeChild(it->second.get());
    }

    sceneOffScreen->removeChild(waterSurfaceMesh.get());
}