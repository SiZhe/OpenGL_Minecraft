#pragma once
#include "../../glframework/Core.hpp"

class Camera{
public:
    Camera();
    ~Camera();

    glm::mat4 GetViewMatrix();
    void setPosition(glm::vec3 position);
    virtual glm::mat4 GetProjectionMatrix();
    //void updateProjectionMatrix(int width, int height);
    virtual void scale(float deltaScale);

    glm::mat4 mProjectionMatrix{1.0f};

    glm::vec3 mPosition{0.0f, 0.0f, 1.0f};
    glm::vec3 mRight{1.0f, 0.0f, 0.0f };
    glm::vec3 mUp{0.0f, 1.0f, 0.0f };
    //glm::vec3 mFront{0.0f, 0.0f, -1.0f };

    glm::mat4 mViewMatrix{1.0f};

    float mNear{0},mFar{0};
};
