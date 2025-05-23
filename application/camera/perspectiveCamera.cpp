#include "perspectiveCamera.hpp"

PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float near, float far) {
    mFovy = fovy;
    mAspect = aspect;
    mNear = near;
    mFar = far;
}

PerspectiveCamera::~PerspectiveCamera() {

}

glm::mat4 PerspectiveCamera::GetProjectionMatrix() {
    return glm::perspective(mFovy, mAspect, mNear, mFar);
}

void PerspectiveCamera::scale(float deltaScale) {
    auto front = glm::cross(mUp,mRight);
    mPosition += (front * deltaScale);
}


