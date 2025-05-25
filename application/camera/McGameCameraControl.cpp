#include "McGameCameraControl.hpp"

#include "../Application.hpp"
#include "../../minecraft/Bag.hpp"
#include "../../minecraft/EliminateAnima.hpp"
#include "../../minecraft/EquipSolt.hpp"

McGameCameraControl* McGameCameraControl::mInstance{nullptr};

McGameCameraControl::McGameCameraControl() {

}

McGameCameraControl::~McGameCameraControl() {
    delete mInstance;
}

McGameCameraControl* McGameCameraControl::getInstance() {
    if (mInstance == nullptr) {
        mInstance = new McGameCameraControl();
    }
    return mInstance;
}

void McGameCameraControl::eyeInit(int altitude)
{
    // 眼睛高度 = 地面高度 + 1
    mEyePos.y = altitude + EYE_HEIGHT;
    mCamera->mPosition.y = mEyePos.y;
    mEyeAltitude = mEyePos.y;
}

void McGameCameraControl::eyeHeight() {
    float currentGround = mcWD->getAltitude(ceil(mEyePos.x - 0.5), ceil(mEyePos.z - 0.5), ceil(mEyePos.y - 1.5)) + EYE_HEIGHT;

    if (mPlayerStatus == PlayerStatus::JumpUp || mPlayerStatus == PlayerStatus::Decline) {
        mJumpTimer += elapseTime;

        // 物理抛物线运动方程
        float h = mInitialJumpY + mJumpInitialVelocity * mJumpTimer - 0.5f * gravity * mJumpTimer * mJumpTimer;

        // 更新摄像机高度
        mEyePos.y = h;
        mCamera->mPosition.y = h;
        mEyeAltitude = h;

        // 到达最高点时切换为下落状态
        if (mPlayerStatus == PlayerStatus::JumpUp && (mJumpInitialVelocity - gravity * mJumpTimer) <= 0.0f) {
            mPlayerStatus = PlayerStatus::Decline;
            // 更新下落参数以确保平滑过渡
            mInitialJumpY = h; // 从当前高度开始下落
            mJumpInitialVelocity = 0.0f;
            // 重新计算下落时间
            mJumpTotalTime = sqrt(2 * (h - currentGround) / gravity);
            mJumpTimer = 0.0f;
        }

        // 落地检测
        if (mJumpTimer >= mJumpTotalTime || h <= currentGround + 0.01f) { // 允许微小误差
            mEyePos.y = currentGround;
            mCamera->mPosition.y = mEyePos.y;
            mEyeAltitude = mEyePos.y;
            mPlayerStatus = PlayerStatus::Normal;
        }
    } else if (isCollision(mEyePos.x, mEyePos.y - 1, mEyePos.z)) {
        mPlayerStatus = PlayerStatus::Decline;
        mInitialJumpY = mEyePos.y;
        mJumpInitialVelocity = 0.0f;
        mJumpTotalTime = sqrt(2 * (mInitialJumpY - currentGround) / gravity); // 计算下落时间
        mJumpTimer = 0.0f;
    }
}

void McGameCameraControl::mouseLeft()
{
    // 只在第一次调用时触发动画
    if (!mLeftMousePressed) {
        placeAnimation = true;
        mLeftMousePressed = true;
    }

    glm::vec3 place;

    glm::vec3 cameraFront = glm::cross(mCamera->mUp, mCamera->mRight);

    for (int i = 1; i < 15; i++) {
        place = mCamera->mPosition + mPlaceSpeed * i * cameraFront;
        if (mcWD->mapData[round(place.x)][round(place.y)][round(place.z)] != CubeType::Null) {
            place = mCamera->mPosition + mPlaceSpeed * (i - 1) * cameraFront;
            if (mcWD->mapData[round(place.x)][round(place.y)][round(place.z)] == CubeType::Null &&
                !(round(place.x) == ceil(mEyePos.x - 0.5) &&
                  round(place.z) == ceil(mEyePos.z - 0.5) &&
                  round(place.y) == ceil(mEyePos.y - 1.5))) {
                mcWD->mapData[round(place.x)][round(place.y)][round(place.z)] = mNowTakeCube;
                if (mcWD->plantData[round(place.x)][round(place.y)][round(place.z)] != CubeType::Null) {
                    mcWD->plantData[round(place.x)][round(place.y)][round(place.z)] = CubeType::Null;
                }
                if (mcWD->leafData[round(place.x)][round(place.y)][round(place.z)] != CubeType::Null) {
                    mcWD->leafData[round(place.x)][round(place.y)][round(place.z)] = CubeType::Null;
                }
                break;
            }
        }
    }
}

void McGameCameraControl::mouseRight() {
	placeAnimation = true;

	glm::vec3 place;

    glm::vec3 cameraFront = glm::normalize(glm::cross(mCamera->mUp, mCamera->mRight));

	for (int i = 1; i < 15; i++) {
		place = mCamera->mPosition + mPlaceSpeed * i * cameraFront;
	    glm::vec3 blockPos = {std::round(place.x), std::round(place.y), std::round(place.z)};
	    if (mcWD->leafData[blockPos.x][blockPos.y][blockPos.z] != CubeType::Null ||
	        mcWD->mapData[blockPos.x][blockPos.y][blockPos.z] != CubeType::Null ) {
	        mcEliminateA->updateFrame(blockPos);
	    }
	    // 检查叶子方块
		if (mcWD->leafData[std::round(place.x)][std::round(place.y)][std::round(place.z)] != CubeType::Null) {
			CubeType cubeType = mcWD->leafData[std::round(place.x)][std::round(place.y)][std::round(place.z)];

		    // 如果是新方块，重置计时器
		    if (!mIsBreaking || mBreakingBlockPos != blockPos) {
		        mIsBreaking = true;
		        mBreakingBlockPos = blockPos;
		        mRightMouseDownTimer = 0.0f;
		    }

		    // 更新计时器
		    mRightMouseDownTimer += elapseTime;

		    // 检查是否达到破坏时间
		    if (mRightMouseDownTimer >= static_cast<float>(CubeEliminateTimes[cubeType]) * mcEliminateA->placeSpeed) {
		        mcEliminateA->push({ std::round(place.x), std::round(place.y), std::round(place.z) }, cubeType);
		        mcWD->leafData[std::round(place.x)][std::round(place.y)][std::round(place.z)] = CubeType::Null;
                mRightMouseDownTimer = 0.0f;
		    }
			return;
		}
        //方块 下层水体也不可以消除
		if (mcWD->mapData[round(place.x)][round(place.y)][round(place.z)] != CubeType::Null ) {
		    CubeType cubeType = mcWD->mapData[round(place.x)][round(place.y)][round(place.z)];
            if (cubeType == CubeType::WaterBelow) {
                return;
            }
		    // 如果是新方块，重置计时器
		    if (!mIsBreaking || mBreakingBlockPos != blockPos) {
		        mIsBreaking = true;
		        mBreakingBlockPos = blockPos;
		        mRightMouseDownTimer = 0.0f;
		    }

		    // 更新计时器
		    mRightMouseDownTimer += elapseTime;

		    // 计算当前应该显示哪个破坏阶段(0-7)
		    if (mRightMouseDownTimer < static_cast<float>(CubeEliminateTimes[cubeType]) * mcEliminateA->placeSpeed) {
		        int currentStage = static_cast<int>(
                    (mRightMouseDownTimer * 8) /
                    (static_cast<float>(CubeEliminateTimes[cubeType]) * mcEliminateA->placeSpeed)
                );

		        std::clamp(currentStage,0,7);

		        mcEliminateA->switchAndUpdateBreakStage(
                    currentStage,
                    glm::vec3(std::round(place.x), std::round(place.y), std::round(place.z)));
		    }

		    // 检查是否达到破坏时间
		    if (mRightMouseDownTimer >= static_cast<float>(CubeEliminateTimes[cubeType]) * mcEliminateA->placeSpeed) {
		        mcEliminateA->push({ std::round(place.x), std::round(place.y), std::round(place.z) }, cubeType);
		        mcWD->mapData[std::round(place.x)][std::round(place.y)][std::round(place.z)] = CubeType::Null;
		        // 同时消除方块上的植物
		        if (mcWD->plantData[std::round(place.x)][std::round(place.y) + 1][std::round(place.z)] != CubeType::Null) {
		            mcWD->plantData[std::round(place.x)][std::round(place.y) + 1][std::round(place.z)] = CubeType::Null;
		        }
		        mcEliminateA->switchAndUpdateBreakStage(8,glm::vec3(std::round(place.x), std::round(place.y), std::round(place.z)));
		        mRightMouseDownTimer = 0.0f;
		    }
		    return;
		}
	}

    // 如果没有瞄准任何方块，重置状态
    mIsBreaking = false;
    mRightMouseDownTimer = 0.0f;
}


void McGameCameraControl::upCubeFrame() {
	glm::vec3 place;

    glm::vec3 cameraFront = glm::normalize(glm::cross(mCamera->mUp, mCamera->mRight));

	for (int i = 1; i < 100; i++) {
		place = mCamera->mPosition + mPlaceSpeed * i * cameraFront;
	    glm::vec3 blockPos = {std::round(place.x), std::round(place.y), std::round(place.z)};
	    if (mcWD->leafData[blockPos.x][blockPos.y][blockPos.z] != CubeType::Null ||
	        mcWD->mapData[blockPos.x][blockPos.y][blockPos.z] != CubeType::Null ) {
	        mcEliminateA->updateFrame(blockPos);
	        return;
	    }
	    mcEliminateA->updateFrame(glm::vec3(0.0f,0.0f,0.0f));
	}
}

void McGameCameraControl::cameraMoveKey() {
    float cameraSpeed = mMoveSpeed * elapseTime;

    // 检查是否站在水体中
    bool isInWater = false;
    isInWater = (mcWD->waterSurfaceData[std::round(mEyePos.x)][std::round(mEyePos.y - 1.0)][std::round(mEyePos.z)] == CubeType::WaterSurface);
    // 水体中速度缓慢
    if (isInWater) {
        cameraSpeed *= 0.3f;
    }

    glm::vec3 oldPos = mEyePos;
    glm::vec3 cameraFront = glm::cross(mCamera->mUp, mCamera->mRight);

    if (mKeyStates[GLFW_KEY_W]) {
        mEyePos += cameraSpeed * cameraFront;
    }
    if (mKeyStates[GLFW_KEY_S]) {
        mEyePos -= cameraSpeed * cameraFront;
    }
    if (mKeyStates[GLFW_KEY_A]) {
        mEyePos -= glm::normalize(glm::cross(cameraFront, mCamera->mUp)) * cameraSpeed;
    }
    if (mKeyStates[GLFW_KEY_D]) {
        mEyePos += glm::normalize(glm::cross(cameraFront, mCamera->mUp)) * cameraSpeed;
    }
    //可能有问题
    if (mEyePos != oldPos) {
        glm::vec3 newPos = mEyePos + mCamera->mPosition - oldPos;

        if (isCollision(newPos.x, newPos.y, newPos.z)) {
            mCamera->mPosition = newPos;
            mEyePos.y = mEyeAltitude;
            mCamera->mPosition.y = mEyePos.y;
        }
        else {
            mEyePos = oldPos;
        }
    }
}

void McGameCameraControl::cameraJumpKey() {
    if (mKeyStates[GLFW_KEY_SPACE]) {
        if (mPlayerStatus == PlayerStatus::Normal) {
            mEyeAltitude = mEyePos.y;
            // 初始化跳跃参数
            mPlayerStatus = PlayerStatus::JumpUp;
            // 起跳点高度
            mInitialJumpY = mEyePos.y;
            // 最高点高度
            mJumpPeakY = mEyePos.y + mJumpHeight;
            // 计算初速度
            mJumpInitialVelocity = sqrt(2 * gravity * (mJumpPeakY - mInitialJumpY));
            // 总跳跃时间
            mJumpTotalTime = 2 * mJumpInitialVelocity / gravity;
            // 重置计时器
            mJumpTimer = 0.0f;
        }
    }
    eyeHeight();
}

void McGameCameraControl::takeCube() {
    for (int i = 0; i < 9; ++i) {
        if (mKeyStates[GLFW_KEY_1 + i]) {
            mNowTakeCube = mcBag->getSlotCubeType(i + 1);
            mScrollValue = i + 1;
            // 更改装备框位置
            mcEquipS->updateSlot(mScrollValue);
            //isChangeCube = true;
            break;
        }
    }
    // 每帧都更新方块,使得滚轮切换方块正确
    mNowTakeCube = mcBag->getSlotCubeType(mScrollValue);
}

inline bool McGameCameraControl::isCollision(float x, float y, float z) {
    if (x <= 10 || z <= 10 || y <= 10 || x >= WORLD_WIDTH - 10 || z >= WORLD_WIDTH - 10 || y >= WORLD_HEIGHT - 10) {
        return false;
    }
    //ceil：向上取整 脚部 (y - 1.5) 和头部 (y - 0.5)
    if (mcWD->mapData[ceil(x - 0.5)][ceil(y - 1.5)][ceil(z - 0.5)] == CubeType::Null &&
        mcWD->mapData[ceil(x - 0.5)][ceil(y - 0.5)][ceil(z - 0.5)] == CubeType::Null) {
        if (mcWD->mapData[ceil(x - 0.5 + 0.2)][ceil(y - 1.5)][ceil(z - 0.5)] == CubeType::Null &&
            mcWD->mapData[ceil(x - 0.5 - 0.2)][ceil(y - 1.5)][ceil(z - 0.5)] == CubeType::Null &&
            mcWD->mapData[ceil(x - 0.5)][ceil(y - 1.5)][ceil(z - 0.5 + 0.2)] == CubeType::Null &&
            mcWD->mapData[ceil(x - 0.5)][ceil(y - 1.5)][ceil(z - 0.5 - 0.2)] == CubeType::Null &&
            mcWD->mapData[ceil(x - 0.5 + 0.2)][ceil(y - 0.5)][ceil(z - 0.5)] == CubeType::Null &&
            mcWD->mapData[ceil(x - 0.5 - 0.2)][ceil(y - 0.5)][ceil(z - 0.5)] == CubeType::Null &&
            mcWD->mapData[ceil(x - 0.5)][ceil(y - 0.5)][ceil(z - 0.5 + 0.2)] == CubeType::Null &&
            mcWD->mapData[ceil(x - 0.5)][ceil(y - 0.5)][ceil(z - 0.5 - 0.2)] == CubeType::Null) {
            return true;
        }
    }
    return false;
}

void McGameCameraControl::onMouse(int button, int action, double xpos, double ypos) {
    mXpos = xpos;
    mYpos = ypos;
    switch(button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            mLeftMouseDown = action;
            if (action == GLFW_PRESS) {
                mLeftMousePressed = false; // 重置按压状态
            }
            break;
        // 右键消除
        case GLFW_MOUSE_BUTTON_RIGHT:
            mRightMouseDown = action;
            if (action == GLFW_PRESS) {
                placeAnimation = true;
                mIsPlacing = true;
            } else {
                // 右键释放时重置状态
                mIsBreaking = false;
                mRightMouseDownTimer = 0.0f;
                mIsPlacing = false;
                placeAnimation = false;
            }
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            mMiddleMouseDown = action;
            break;
        default:
            break;
    }

    // 如果游戏暂停，直接返回
    if (isGameSTOP || isBag) {
        return;
    }
    takeCube();
    //左键
    if (mLeftMouseDown) {
         mouseLeft();
    }
    // 右键
    // if (mRightMouseDown) {
    //     mouseRight();
    // }
}

void McGameCameraControl::pitch(float angle) {
    mEyeRadians += angle;
    if (mEyeRadians > 89.0f || mEyeRadians < -89.0f) {
        mEyeRadians -= angle;
        return;
    }
    auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), mCamera->mRight);
    mCamera->mUp = glm::normalize(mat * glm::vec4(mCamera->mUp,0.0f));//vec4 给到 vec3//0.0f是向量，1.0f是点
}

void McGameCameraControl::yaw(float angle) {
    mEyeRadians += angle;
    auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    mCamera->mUp = glm::normalize(mat * glm::vec4(mCamera->mUp,0.0f));
    mCamera->mRight = glm::normalize(mat * glm::vec4(mCamera->mRight,0.0f));
}

void McGameCameraControl::onCursor(double xpos, double ypos) {
    // 打开背包后，记录鼠标位置
    if (isBag) {
        mXpos = xpos;
        mYpos = ypos;
    }

    // 如果游戏暂停，直接返回
    if (isGameSTOP || isBag) {
        return;
    }

    // 获取窗口中心位置
    int centerX = glApp->getWidth() / 2;
    int centerY = glApp->getHeight() / 2;

    // 如果光标不在中心，设置回中心
    if (xpos != centerX || ypos != centerY) {
        glfwSetCursorPos(glApp->getWindow(), centerX, centerY);
    }

    // 计算偏移量时使用中心位置作为参考
    float xoffset = (xpos - centerX) * mMouseSensitivity;
    float yoffset = (ypos - centerY) * mMouseSensitivity;

    mBodyRadians += xoffset;
    mEyeRadians += yoffset;

    if (mEyeRadians > 89.0f) {
        mEyeRadians = 89.0f;
    }
    if (mEyeRadians < -89.0f) {
        mEyeRadians = -89.0f;
    }

    glm::vec3 front;
    front.x = glm::cos(glm::radians(mBodyRadians)) * cos(glm::radians(mEyeRadians));
    front.y = glm::sin(glm::radians(mEyeRadians));
    front.z = glm::sin(glm::radians(mBodyRadians)) * cos(glm::radians(mEyeRadians));
    // 假设初始的 Up 方向是世界 Y 轴
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // 计算 Right 向量
    glm::vec3 right = glm::normalize(glm::cross(worldUp, front));
    // 计算 Up 向量
    glm::vec3 up = glm::normalize(glm::cross(front, right));
    mCamera->mRight = right;
    mCamera->mUp = up;

    if (EYE_HEIGHT == 1) {
        float radius = 0.0f;
        float camX = glm::cos(glm::radians(mBodyRadians)) * radius;
        float camZ = glm::sin(glm::radians(mBodyRadians)) * radius;
        mCamera->mPosition.x = mEyePos.x + camX;
        mCamera->mPosition.z = mEyePos.z + camZ;
    }
}

void McGameCameraControl::onKey(int button, int action, int mods) {
    mKeyStates[button] = action;
    // 仅在 ESC 键按下时触发（避免重复切换）
    if (mKeyStates[GLFW_KEY_ESCAPE]) {
        isGameSTOP = !isGameSTOP;
        // 切换鼠标状态
        glfwSetInputMode(glApp->getWindow(), GLFW_CURSOR,
            isGameSTOP ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }

    // 按B打开书包
    if (mKeyStates[GLFW_KEY_B] == GLFW_PRESS) {
        isBag = !isBag;
        // 切换鼠标状态
        glfwSetInputMode(glApp->getWindow(), GLFW_CURSOR,
            isBag ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }

    // 打开书包的情况下可以按下esc关闭
    if (isBag && mKeyStates[GLFW_KEY_ESCAPE]) {
        isBag = false;
        isGameSTOP = false;
        // 切换鼠标状态
        glfwSetInputMode(glApp->getWindow(), GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    }
}

void McGameCameraControl::onScroll(double yoffset) {
    // 如果游戏暂停，直接返回
    if (isGameSTOP || isBag) {
        return;
    }
    // 滚轮向上（yoffset > 0）：切换到下一个方块
    if (yoffset > 0) {
        mScrollValue++;
        if (mScrollValue > 9) {
            mScrollValue = 1; // 超过10就回到1
        }
    }
    // 滚轮向下（yoffset < 0）：切换到上一个方块
    else if (yoffset < 0) {
        mScrollValue--;
        if (mScrollValue < 1) {
            mScrollValue = 9; // 低于1就回到10
        }
    }
    // 更改装备框位置
    mcEquipS->updateSlot(mScrollValue);

    // 更新当前方块
    mNowTakeCube = static_cast<CubeType>(mScrollValue);
}

void McGameCameraControl::update() {

    if (isBag) {
        mcBag->dragCube();
    }

    // 如果游戏暂停，直接返回
    if (isGameSTOP || isBag) {
        return;
    }

    // 如果右键持续按下且游戏未暂停
    if (mRightMouseDown) {
        mouseRight();
    }else {
        mcEliminateA->switchAndUpdateBreakStage(8,glm::vec3(0.0f, 0.0f, 0.0f));
    }

    upCubeFrame();
    cameraMoveKey();
    cameraJumpKey();
    takeCube();
}