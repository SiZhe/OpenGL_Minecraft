#include "trackBallCameraControl.hpp"

#include <iostream>
#include <ostream>

TrackBallCameraControl::TrackBallCameraControl() {

}

TrackBallCameraControl::~TrackBallCameraControl() {

}

void TrackBallCameraControl::pitch(float angel) {
    //绕mRight旋转
    auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angel), mCamera->mRight);
    //影响up 和 position
    mCamera->mUp = mat * glm::vec4(mCamera->mUp,0.0f);//vec4 给到 vec3//0.0f是向量，1.0f是点
    mCamera->mPosition = mat * glm::vec4(mCamera->mPosition,1.0f);
}

void TrackBallCameraControl::yaw(float angel) {
    auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angel), glm::vec3(0.0f, 1.0f, 0.0f));
    mCamera->mUp = mat * glm::vec4(mCamera->mUp,0.0f);
    mCamera->mRight = mat * glm::vec4(mCamera->mRight,0.0f);
    mCamera->mPosition = mat * glm::vec4(mCamera->mPosition,1.0f);
}

void TrackBallCameraControl::onCursor(double xpos, double ypos) {
    if (mLeftMouseDown) {
        float deltaX = static_cast<float>(xpos - mXpos) * mMouseSensitivity;
        float deltaY = static_cast<float>(ypos - mYpos) * mMouseSensitivity;
        //pitch
        pitch(-deltaY);
        //yaw
        yaw(-deltaX);

    }else if(mMiddleMouseDown) {
        float deltaX = static_cast<float>(xpos - mXpos) * mMouseSpeed;
        float deltaY = static_cast<float>(ypos - mYpos) * mMouseSpeed;

        mCamera->mPosition = mCamera->mPosition - mCamera->mRight * deltaX;
        mCamera->mPosition = mCamera->mPosition + mCamera->mUp * deltaY;
    }
    mXpos = xpos;
    mYpos = ypos;
}

void TrackBallCameraControl::onScroll(double yoffset) {
    mCamera->scale(mZoomSensitivity * yoffset);
}

