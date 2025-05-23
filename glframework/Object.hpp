#pragma once

#include <vector>

#include "Core.hpp"

enum class ObjectType {
    Mesh,
    Object,
    Scene,
    InstancedMesh,
    //CubeMesh,
    //PlantMesh,
    //LeafMesh,
};

class Object {
public:
    Object();
    ~Object();

    void setPosition(glm::vec3 position);
    void rotateX(float angel);
    void rotateY(float angel);
    void rotateZ(float angel);

    void setAngleX(float angelX);
    void setAngleY(float angelY);
    void setAngleZ(float angelZ);
    void setScale(glm::vec3 scale);

    glm::mat4 getModelMatrix() const;
    glm::vec3 getPosition() const;
    ObjectType getType();

    void addChild(Object* child);
    void removeChild(Object* child);
    std::vector<Object*> getChildren();
    void setParent(Object* parent);
    Object* getParent();

    //目前只有TakeCube用到 还有 cubeBreak
    glm::mat4 mModelMatrix{1.0f};

//protected:
    glm::vec3 mPosition{0.0f,0.0f,0.0f};

    //unity旋转标准：pitch yaw roll
    float mAngleX{0.0f};
    float mAngleY{0.0f};
    float mAngleZ{0.0f};

    glm::vec3 mScale{1.0f};

    std::vector<Object*> mChildren{};
    Object* mParent{nullptr};

    ObjectType mType{};
};