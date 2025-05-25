#include "camera.hpp"

Camera::Camera() {

}

Camera::~Camera() {

}

void Camera::setPosition(glm::vec3 position) {
    mPosition = position;
}

glm::mat4 Camera::GetViewMatrix() {
    glm::vec3 front = glm::cross(mUp, mRight);
    glm::vec3 center = mPosition + front;
    mViewMatrix = glm::lookAt(mPosition,center,mUp);
    return mViewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix() {
    return mProjectionMatrix;
}

// void Camera::updateProjectionMatrix(int width, int height) {
//     float aspectRatio = static_cast<float>(width) / height;
//     mProjectionMatrix = glm::perspective(glm::radians(90.0f), aspectRatio, mNear, mFar);
// }


void Camera::scale(float deltaScale) {

}
