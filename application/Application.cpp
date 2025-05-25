#include <iostream>
#include "Application.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <format>


Application* Application::mInstance{nullptr};

Application::Application() {

}

Application::~Application() {
    delete mInstance;
}

Application* Application::getInstance() {
    if (mInstance == nullptr) {
        mInstance = new Application();
    }
    return mInstance;
}

bool Application::init(bool fullScreen, const uint32_t& Width,const uint32_t& Height ,const char* Title)
{
    mWidth = Width;
    mHeight = Height;
    mTitle = Title;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 全屏模式：使用主显示器的分辨率
    GLFWmonitor* monitor = fullScreen ? glfwGetPrimaryMonitor() : nullptr;
    mWindow = glfwCreateWindow(mWidth, mHeight, mTitle, monitor, nullptr);

    // 隐藏鼠标并锁定到窗口
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!mWindow) {
        std::cout << "Failed to create GLFW window." << std::endl;
        return false;
    }

    glfwMakeContextCurrent(mWindow);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);
    glfwSetKeyCallback(mWindow, key_callback);
    glfwSetScrollCallback(mWindow, scroll_callback);
    glfwSetCursorPosCallback(mWindow,cursor_callback);
    glfwSetMouseButtonCallback(mWindow,mouse_callback);

    glfwSetWindowUserPointer(mWindow,this);

    return true;
}

bool Application::update()
{
    if(glfwWindowShouldClose(mWindow)){
        return false;
    }
    glfwPollEvents();
    glfwSwapBuffers(mWindow);
    Application* self = (Application*)glfwGetWindowUserPointer(mWindow);
    //std::cout << self->getWidth() << std::endl;
    return true;
}

void Application::destroy() {
    glfwTerminate();
}

void Application::SetResizeCallback(ResizeCallback Callback) {
    mResizeCallback = Callback;
}

void Application::SetKeyCallback(KeyCallback Callback) {
    mKeyCallback = Callback;
}

void Application::SetCursorCallback(CursorCallback Callback) {
    mCursorCallback = Callback;
}

void Application::SetMouseCallback(MouseCallback Callback) {
    mMouseCallback = Callback;
}

void Application::SetScrollCallback(ScrollCallback Callback) {
    mScrollCallback = Callback;
}

void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    //std::cout << "Framebuffer size: " << width << " x " << height << std::endl;
    Application* self = (Application*)glfwGetWindowUserPointer(window);
    self->setHeight(height);
    self->setWidth(width);

    // 关键：更新 OpenGL 视口
    glViewport(0, 0, width, height);

    if (self->mResizeCallback != nullptr) {
        self->mResizeCallback(width, height);
    }
}

void Application::key_callback(GLFWwindow* window, int key,int scancode, int action, int mods) {
    //std::cout << std::format("key->{}, action->{}, mods->{}",key,action,mods)<< std::endl;
    Application* self = (Application*)glfwGetWindowUserPointer(window);
    if (self->mKeyCallback != nullptr) {
        self->mKeyCallback(key, scancode, action, mods);
    }
}

void Application::mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    //std::cout << std::format("mouseDown->{}",button) << std::endl;
    Application* self = (Application*)glfwGetWindowUserPointer(window);
    if (self->mMouseCallback != nullptr) {
        self->mMouseCallback(button, action, mods);
    }
}

void Application::cursor_callback(GLFWwindow* window, double xpos, double ypos) {
    //std::cout << std::format("cursorPos: x->{},y->{} ",xpos,ypos) << std::endl;
    Application* self = (Application*)glfwGetWindowUserPointer(window);
    if (self->mCursorCallback != nullptr) {
        self->mCursorCallback(xpos, ypos);
    }
}


void Application::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    //std::cout << std::format("scrollDown->{}",yoffset) << std::endl;
    Application* self = (Application*)glfwGetWindowUserPointer(window);
    if (self->mScrollCallback != nullptr) {
        self->mScrollCallback(yoffset);
    }
}



