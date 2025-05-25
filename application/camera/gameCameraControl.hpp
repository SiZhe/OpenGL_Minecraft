#pragma once
#include "cameraControl.hpp"
#include "../../glframework/Core.hpp"

class GameCameraControl : public CameraControl{
public:
    GameCameraControl();
    ~GameCameraControl();
    void onCursor(double xpos, double ypos) override;
    void update() override;

private:
    void pitch(float angle);
    void yaw(float angle);

    float mPitch{0.0f};
    float mSpeed{0.01f};
};