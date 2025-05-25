#pragma once
#include "../../glframework/Core.hpp"
#include "cameraControl.hpp"

class TrackBallCameraControl : public CameraControl {
public:
    TrackBallCameraControl();
    ~TrackBallCameraControl() override;

    void onCursor(double xpos, double ypos) override;
    void onScroll(double yOffset) override;

private:
    void pitch(float angel);
    void yaw(float angel);
    float mMouseSpeed = 0.005f;
};