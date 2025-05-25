#pragma once
#include <map>

#include "camera.hpp"
#include "../../glframework/Core.hpp"

class CameraControl {
public:
    CameraControl();
    virtual ~CameraControl();

    virtual void onMouse(int button, int action, double xpos, double ypos);
    virtual void onCursor(double xpos, double ypos);
    virtual void onScroll(double yoffset);
    virtual void onKey(int button, int action,int mods);
    virtual void update();

    virtual void setCamera(Camera* camera);

    float mMouseSensitivity = 0.3f;
    float mZoomSensitivity = 0.1f;

    bool mLeftMouseDown = false;
    bool mLeftMousePressed = false;
    bool mRightMouseDown = false;
    float mRightMouseDownTimer {0.0f};

    bool mMiddleMouseDown = false;
    std::map<int, int> mKeyStates;
    Camera* mCamera{nullptr};

    double mXpos{0.0};
    double mYpos{0.0};
};