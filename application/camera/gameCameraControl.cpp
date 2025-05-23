#include "gameCameraControl.hpp"

GameCameraControl::GameCameraControl() {

}

GameCameraControl::~GameCameraControl() {

}

void GameCameraControl::pitch(float angle) {
    mPitch += angle;
    if (mPitch > 1.0f || mPitch < -1.0f) {
        mPitch -= angle;
        return;
    }
    //std::cout << mPitch << std::endl;
    auto mat = glm::rotate(glm::mat4(1.0f), angle, mCamera->mRight);
    mCamera->mUp = mat * glm::vec4(mCamera->mUp,0.0f);//vec4 给到 vec3//0.0f是向量，1.0f是点

}

void GameCameraControl::yaw(float angle) {
    auto mat = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
    mCamera->mUp = mat * glm::vec4(mCamera->mUp,0.0f);
    mCamera->mRight = mat * glm::vec4(mCamera->mRight,0.0f);
}

void GameCameraControl::onCursor(double xpos, double ypos) {
    float deltaX = (xpos - mXpos) * mMouseSensitivity * 0.1;
    float deltaY = (ypos - mYpos) * mMouseSensitivity * 0.1;
    if (mRightMouseDown) {
        pitch(deltaY);
        yaw(deltaX);
    }
    mXpos = xpos;
    mYpos = ypos;
}

void GameCameraControl::update() {
    glm::vec3 direction{0.0f};
    auto front = glm::cross(mCamera->mUp,mCamera->mRight);
    auto right = mCamera->mRight;
    if (mKeyStates[GLFW_KEY_W]) {
        direction -= front;
    }else if (mKeyStates[GLFW_KEY_S]) {
        direction += front;
    }else if (mKeyStates[GLFW_KEY_A]) {
        direction += right;
    }else if (mKeyStates[GLFW_KEY_D]) {
        direction -= right;
    }

    if (glm::length(direction) != 0.0f) {
        direction = glm::normalize(direction);
        mCamera->mPosition += direction * mSpeed;
    }
}