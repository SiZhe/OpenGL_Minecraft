#include "orthoCamera.hpp"

OrthoCamera::OrthoCamera(float left, float right, float bottom, float top,float near,float far) {
    mLeft = left;
    mRight = right;
    mBottom = bottom;
    mTop = top;
    mNear = near;
    mFar = far;
}

OrthoCamera::~OrthoCamera() {

}

glm::mat4 OrthoCamera::GetProjectionMatrix() {
    float scale = std::pow(2,mScale);
    mProjectionMatrix = glm::ortho(mLeft * scale, mRight * scale, mBottom * scale, mTop * scale, mNear, mFar);
    return mProjectionMatrix;
}

void OrthoCamera::scale(float deltaScale) {
    mScale += deltaScale;
}


