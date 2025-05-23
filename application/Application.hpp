#pragma once
#include <stdint.h>
#include "../glframework/Core.hpp"

#define glApp Application::getInstance()

using ResizeCallback = void(*)(int width,int height);
using KeyCallback = void(*)(int key, int scancode, int action, int mods);
using MouseCallback = void(*)(int button, int action, int mods);
using CursorCallback = void(*)(double x, double y);
using ScrollCallback = void(*)(double yoffset);

class Application{
public:
    ~Application();

    static Application* getInstance();

    uint32_t getWidth() const { return mWidth; }
    void setWidth(uint32_t width) {
        mWidth = width;
    }
    void setHeight(uint32_t height) {
        mHeight = height;
    }

    uint32_t getHeight() const { return mHeight; }
    const char* getTitle() const {return mTitle; }
    GLFWwindow* getWindow() const { return mWindow; }

    bool init(bool fullScreen, const uint32_t& Width = 800, const uint32_t& Height = 640 ,const char* Title = "Opengl Try");
    bool update();
    void destroy();

    void SetResizeCallback(ResizeCallback Callback);
    void SetKeyCallback(KeyCallback Callback);
    void SetMouseCallback(MouseCallback Callback);
    void SetScrollCallback(ScrollCallback Callback);
    void SetCursorCallback(CursorCallback Callback);

    void getCursorPos(double& x, double& y) const {
        glfwGetCursorPos(mWindow, &x, &y);
    }

private:
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void cursor_callback(GLFWwindow* window, double xpos, double ypos);

private:
    static Application* mInstance;
    GLFWwindow* mWindow{nullptr};
    uint32_t mWidth{0};
    uint32_t mHeight{0};
    const char* mTitle{};
    Application();

    ResizeCallback mResizeCallback{nullptr};
    KeyCallback mKeyCallback{nullptr};
    MouseCallback mMouseCallback{nullptr};
    CursorCallback mCursorCallback{nullptr};
    ScrollCallback mScrollCallback{nullptr};
};



