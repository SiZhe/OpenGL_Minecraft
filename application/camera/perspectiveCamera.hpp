#pragma once
#include "../../glframework/Core.hpp"
#include "camera.hpp"

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(float fovy,float aspect,float near,float far);
    ~PerspectiveCamera();

    glm::mat4 GetProjectionMatrix() override;
    void scale(float deltaScale) override;

private:
    float mFovy{0},mAspect{0};
    float mScale{0.0f};
};
