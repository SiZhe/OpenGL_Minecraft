#include "PlayerMesh.hpp"
#include "../../../application/camera/McGameCameraControl.hpp"
#include "../../../glframework/material/PhongMaterial.hpp"
#include "../../../minecraft/Bag.hpp"
#include "../../../minecraft/EliminateAnima.hpp"

PlayerMesh::PlayerMesh(const std::string &name) {
    mName = name;
    mType = ObjectType::Mesh;
    for (int i = 0 ; i < 5 ;i++) {
        for (int j = 0 ; j < 9 ;j++) {
            takeCubeMeshes[mcBag->bagIcon[i][j]] = std::make_unique<TakeCubeMesh>(mcBag->cubeTypeToString(mcBag->bagIcon[i][j]));
        }
    }
}

PlayerMesh::~PlayerMesh() {
}

void PlayerMesh::takeCubeInit() {
    for (auto it = takeCubeMeshes.begin() ; it != takeCubeMeshes.end() ; it++) {
        it->second->init();
    }
}

void PlayerMesh::init() {
    takeCubeInit();
    mGeometry = Geometry::CreateBox(1.0, 1.0, 1.0);

    PhongMaterial* cubeMat = new PhongMaterial();

    std::string diffPath = "../assets/textures/" + mName +".png";
    cubeMat->mDiffuse = Texture::createTexture(diffPath,0);
    cubeMat->mSpecularMask = Texture::createTexture("../assets/textures/defaultMask.png",1);
    //cubeMat->mFaceCulling = true;
    mMaterial = cubeMat;
}

void PlayerMesh::runAnimation(const glm::vec3 &pos) {
    if (limbRadians >= 45 && isLimbDir == true) {
        isLimbDir = false;
    }else if (limbRadians <= -45 && isLimbDir == false) {
        isLimbDir = true;
    }

    if (isLimbDir) {
        if (limbRadians > -30 && limbRadians < 30) {
            limbRadians += 500 * mcControl->elapseTime;
        } else {
            limbRadians += 200 * mcControl->elapseTime;
        }
    } else {
        if (limbRadians > -30 && limbRadians < 30) {
            limbRadians -= 500 * mcControl->elapseTime;
        } else {
            limbRadians -= 200 * mcControl->elapseTime;
        }
    }

    if (pos == oldPos) {
        limbRadians = 0;
    }
    oldPos = pos;
}

void PlayerMesh::placeAnimation(bool& placeAnimation) {
    // 只在placeAnimation为true且当前动画结束时触发新动画
    if (placeAnimation && placeCubeAnimation == Animation::End) {
        placeCubeAnimation = Animation::GO;
        placeValue = 0.0f;
        placeAnimation = false; // 立即重置标志
    }
}

void PlayerMesh::takeCubeAnimation(CubeType nowCube) {
    if (newTakeCube != nowCube ) {
        newTakeCube = nowCube;
        takeAnimation = Animation::GO;
        takeValue = 0;
    }
}

void PlayerMesh::draw(const glm::vec3 &pos, bool isMy) {
    runAnimation(pos);
    if (isMy) {
        firstPersonDrawTakeCube();
    }else {
        std::cout << " NO FOR THIRD PLAYER!!" << std::endl;
    }
}

void PlayerMesh::firstPersonDrawTakeCube() {
    auto it = takeCubeMeshes.find(nowTakeCube);
    if (it != takeCubeMeshes.end()) {
        it->second->upDataShaderFir(eyeRadians, bodyRadians, limbRadians, placeValue, takeValue);
    }
    placeValueChange();
    takeValueChange();
}

void PlayerMesh::placeValueChange() {
    // 如果游戏暂停，直接返回
    if (mcControl->isGameSTOP || mcControl->isBag) {
        placeCubeAnimation = Animation::End;
        placeValue = 0;
        return;
    }

    if (placeCubeAnimation != Animation::End) {
        if (placeCubeAnimation == Animation::GO) {
            float timerGO = mcControl->elapseTime / (mcEliminateA->placeSpeed /2) * 12;
            placeValue += timerGO;
            if (placeValue >= 12)
                placeCubeAnimation = Animation::Return;
        } else {
            float timerReturn = mcControl->elapseTime / (mcEliminateA->placeSpeed /2) * 12;
            placeValue -= timerReturn;
            if (placeValue <= 0) {
                placeCubeAnimation = Animation::End;
                // 如果右键仍然按下，立即开始新的动画循环
                if (mIsPlacing) {
                    placeCubeAnimation = Animation::GO;
                    placeValue = 0;
                }
            }
        }
    }else if (mIsPlacing) {
        // 动画结束但仍在放置状态，开始新动画
        placeCubeAnimation = Animation::GO;
        placeValue = 0;
    }
}

void PlayerMesh::takeValueChange()
{
    if (takeAnimation != Animation::End) {
        if (takeAnimation == Animation::GO) {
            float timerGO = mcControl->elapseTime / (takeSpeed/2) * 10;
            takeValue += timerGO;
            if (takeValue >= 10) {
                nowTakeCube = newTakeCube;
                takeAnimation = Animation::Return;
            }
        } else {
            float timerReturn = mcControl->elapseTime / (takeSpeed/2) * 10;
            takeValue -= timerReturn;
            if (takeValue <= 0)
                takeAnimation = Animation::End;
        }
    }
}