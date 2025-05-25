#include "EliminateAnima.hpp"
#include "../application/camera/McGameCameraControl.hpp"

EliminateAnima* EliminateAnima::mInstance{nullptr};

EliminateAnima::EliminateAnima() {
    for (int i = 0 ; i < 5 ;i++) {
        for (int j = 0 ; j < 9 ;j++) {
            std::string name = mcBag->cubeTypeToString(mcBag->bagIcon[i][j]);
            eliminateMeshes[mcBag->bagIcon[i][j]] = std::make_unique<EliminateMesh>(name);
            dropMeshes[mcBag->bagIcon[i][j]] = std::make_unique<DropMesh>(name);
        }
    }

    dropStencil = std::make_unique<DropStencilMesh>();

    for (int i = 0 ; i < breakStageMeshes.size() ;i++) {
        breakStageMeshes[i] = std::make_unique<CubeBreakMesh>("BreakStage"+ std::to_string(i));
    }

    frameMesh = std::make_unique<CubeFrameMesh>("Frame");
}

EliminateAnima::~EliminateAnima() {

}

EliminateAnima* EliminateAnima::getInstance() {
    if (mInstance == nullptr) {
        mInstance = new EliminateAnima();
    }
    return mInstance;
}

void EliminateAnima::init() {

    for (auto it = eliminateMeshes.begin() ; it != eliminateMeshes.end() ; it++) {
        it->second->init();
    }

    for (auto it = dropMeshes.begin() ; it != dropMeshes.end() ; it++) {
        it->second->init();
    }

    dropStencil->init();

    for (int i = 0 ; i < breakStageMeshes.size() ;i++) {
        breakStageMeshes[i]->init();
    }

    frameMesh->init();
}

void EliminateAnima::push(glm::vec3 vec, CubeType cubeType) {
    std::vector<std::vector<OffscumStruct>> offscumVec2;
    offscumVec2.reserve(10);
    for (int i = 0; i < 10; i++) {
        std::vector<OffscumStruct> offscumVec;
        offscumVec.reserve(10);
        for (int j = 0; j < 10; j++) {
            offscumVec.emplace_back(glm::vec3(vec.x + offscumPosRand(), vec.y + offscumPosRand(), vec.z + offscumPosRand()),
                    rand() % 360, float(rand() % 3 + 1) / 100, cubeType);
        }
        offscumVec2.push_back(offscumVec);
    }

    offscumVecList.emplace_back(offscumVec2);
    //掉落
    dropList.emplace_back(offscumVec2[0][0]);

    /////////////////////////////////
    if (offscumVecList.size() > 2) {
         offscumVecList.pop_front();
    }
}

void EliminateAnima::draw() {
    for (auto item = offscumVecList.begin(); item != offscumVecList.end(); ++item) {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                OffscumStruct *offscum = reinterpret_cast<OffscumStruct*>(&(*item)[i][j]);
                if (offscum->isDraw) {
                    if (offscum->isUp) {
                        offscum->pos.x += sin(offscum->angle) * offscum->speed;
                        offscum->pos.z += cos(offscum->angle) * offscum->speed;
                        offscum->pos.y += (offscum->begin - offscum->pos.y) * float(rand() % 25) / 1000 + mcControl->elapseTime;

                        if (offscum->pos.y > offscum->begin - 0.75) {
                            offscum->isUp = false;
                        }
                    } else {
                        // 掉落高度:round(offscum->pos.y)) - 0.3
                        if (offscum->pos.y >= mcWD->getAltitude(round(offscum->pos.x), round(offscum->pos.z), round(offscum->pos.y)) - 0.4) {
                            offscum->pos.x += sin(offscum->angle) * offscum->speed;
                            offscum->pos.z += cos(offscum->angle) * offscum->speed;
                            offscum->pos.y -= (offscum->begin - offscum->pos.y) * float(rand() % 25) / 1000 + mcControl->elapseTime;
                        }
                    }
                }

                if (offscum->isDraw) {
                    //控制存在时间
                    if (rand() % 15 == 0) {
                        offscum->isDraw = false;
                    } else {
                        switchEliminateDraw(*offscum);
                    }
                }
            }
        }
    }

    for (auto item = dropList.begin(); item != dropList.end(); ++item) {
        OffscumStruct *dropStruct = reinterpret_cast<OffscumStruct*>(&(*item));
        if (dropStruct->isUp) {
            dropStruct->pos.x += sin(dropStruct->angle) * dropStruct->speed;
            dropStruct->pos.z += cos(dropStruct->angle) * dropStruct->speed;
            dropStruct->pos.y += (dropStruct->begin - dropStruct->pos.y) * float(rand() % 25) / 1000 + mcControl->elapseTime;

            if (dropStruct->pos.y > dropStruct->begin - 0.75) {
                dropStruct->isUp = false;
            }
        } else {
            // 掉落高度:round(offscum->pos.y)) - 0.3
            if (dropStruct->pos.y >= mcWD->getAltitude(round(dropStruct->pos.x), round(dropStruct->pos.z), round(dropStruct->pos.y)) - 0.3) {
                dropStruct->pos.x += sin(dropStruct->angle) * dropStruct->speed;
                dropStruct->pos.z += cos(dropStruct->angle) * dropStruct->speed;
                dropStruct->pos.y -= (dropStruct->begin - dropStruct->pos.y) * float(rand() % 25) / 1000 + mcControl->elapseTime;
            }
        }

        //这里设置存活时间
        if (dropStruct->lifetime > dropCubeLifetime) {
            dropList.pop_front();
        } else {
            switchDropDraw(*dropStruct);
            dropStruct->lifetime += mcControl->elapseTime;
        }
    }
}

float EliminateAnima::offscumPosRand() {
    return float(rand() % 100 - 50) / 100;
}

void EliminateAnima::switchEliminateDraw(OffscumStruct &offstr) {
    auto it = eliminateMeshes.find(offstr.cubeType);
    if (it != eliminateMeshes.end()) {
        it->second->upDateShader(offstr.pos);
    }
}

void EliminateAnima::switchDropDraw(OffscumStruct &offstr) {
    dropStencil->upDateShader(offstr.pos,offstr.angle);

    auto it = dropMeshes.find(offstr.cubeType);
    if (it != dropMeshes.end()) {
        it->second->upDateShader(offstr.pos,offstr.angle);
    }
}

void EliminateAnima::switchAndUpdateBreakStage(int stage,const glm::vec3 &pos) {
    for (auto& cube : breakStageMeshes) {
        cube->upDateShader(glm::vec3(0.0f,0.0f,0.0f));
    }
    breakStageMeshes[stage]->upDateShader(pos);
}

void EliminateAnima::updateFrame(const glm::vec3 &pos) {
    frameMesh->upDateShader(pos);
}

void EliminateAnima::updateEliminateCube() {

    for (auto it = eliminateMeshes.begin() ; it != eliminateMeshes.end() ; it++) {
        it->second->upDataInstanceMatrices();
    }

    for (auto it = dropMeshes.begin() ; it != dropMeshes.end() ; it++) {
        it->second->upDataInstanceMatrices();
    }

    dropStencil->upDataInstanceMatrices();
}

void EliminateAnima::clearAll() {

    for (auto it = eliminateMeshes.begin() ; it != eliminateMeshes.end() ; it++) {
        it->second->clear();
    }

    for (auto it = dropMeshes.begin() ; it != dropMeshes.end() ; it++) {
        it->second->clear();
    }

    dropStencil->clear();
}

void EliminateAnima::addScene(Scene* sceneOffScreen) {

    for (auto it = eliminateMeshes.begin() ; it != eliminateMeshes.end() ; it++) {
        sceneOffScreen->addChild(it->second.get());
    }

    for (auto it = dropMeshes.begin() ; it != dropMeshes.end() ; it++) {
        sceneOffScreen->addChild(it->second.get());
    }

    sceneOffScreen->addChild(dropStencil.get());

    for (auto& cube : breakStageMeshes) {
        sceneOffScreen->addChild(cube.get());
    }

    sceneOffScreen->addChild(frameMesh.get());
}

void EliminateAnima::removeFromScene(Scene* sceneOffScreen) {

    for (auto it = eliminateMeshes.begin() ; it != eliminateMeshes.end() ; it++) {
        sceneOffScreen->removeChild(it->second.get());
    }

    for (auto it = dropMeshes.begin() ; it != dropMeshes.end() ; it++) {
        sceneOffScreen->removeChild(it->second.get());
    }

    sceneOffScreen->removeChild(dropStencil.get());

    for (auto& cube : breakStageMeshes) {
        sceneOffScreen->removeChild(cube.get());
    }

    sceneOffScreen->removeChild(frameMesh.get());
}
