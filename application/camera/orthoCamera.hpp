#pragma once
#include "../../glframework/Core.hpp"
#include "camera.hpp"

class OrthoCamera : public Camera {
public:
    OrthoCamera(float left, float right, float bottom, float top,float near,float far);
    ~OrthoCamera();

    glm::mat4 GetProjectionMatrix() override;
    void scale(float deltaScale) override;

private:
    float mLeft{0.0f}, mRight{0.0f}, mTop{0.0f}, mBottom{0.0f};
    float mScale{0.0f};
};
