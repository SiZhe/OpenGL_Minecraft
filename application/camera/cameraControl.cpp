#include "cameraControl.hpp"

CameraControl::CameraControl() {

}

CameraControl::~CameraControl() {

}

void CameraControl::onMouse(int button, int action, double xpos, double ypos) {
    bool pressed = action == GLFW_PRESS;
    if (pressed) {
        mXpos = xpos;
        mYpos = ypos;
    }
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            mLeftMouseDown = pressed;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            mRightMouseDown = pressed;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            mMiddleMouseDown = pressed;
            break;
    }
}

void CameraControl::onCursor(double xpos, double ypos) {

}

void CameraControl::onScroll(double yoffset) {

}

void CameraControl::onKey(int button, int action, int mods) {
    if (action == GLFW_REPEAT) {
        return;
    }
    bool pressed = action == GLFW_PRESS;

    mKeyStates[button] = pressed;
}

void CameraControl::update() {

}

void CameraControl::setCamera(Camera* camera) {
    mCamera = camera;
}