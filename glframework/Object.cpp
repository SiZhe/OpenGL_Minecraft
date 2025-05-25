#include "Object.hpp"

#include <iostream>
#include <bits/ostream.tcc>

Object::Object() {
    mType = ObjectType::Object;
}

Object::~Object() {

}

void Object::setPosition(glm::vec3 position) {
    mPosition = position;
}


void Object::rotateX(float angel) {
    mAngleX += angel;
}

void Object::rotateY(float angel) {
    mAngleY += angel;
}

void Object::rotateZ(float angel) {
    mAngleZ += angel;
}

void Object::setAngleX(float angelX) {
    mAngleX = angelX;
}

void Object::setAngleY(float angelY) {
    mAngleY = angelY;
}

void Object::setAngleZ(float angelZ) {
    mAngleZ = angelZ;
}

void Object::setScale(glm::vec3 scale) {
    mScale = scale;
}

ObjectType Object::getType() {
    return mType;
}

glm::vec3 Object::getPosition() const{
    return mPosition;
}

glm::mat4 Object::getModelMatrix() const{
    //得到父亲的变化矩阵
    glm::mat4 parentMatrix = glm::mat4(1.0f);
    if (mParent != NULL){
        parentMatrix =  mParent->getModelMatrix();
    }
    // 缩放 旋转 平移
    glm::mat4 transform{1.0f};

    transform = glm::scale(transform, mScale);

    transform = glm::rotate(transform, mAngleX, glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, mAngleY, glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, mAngleZ, glm::vec3(0.0f, 0.0f, 1.0f));

    transform = parentMatrix * glm::translate(glm::mat4(1.0f), mPosition) * transform;

    return transform;
}

void Object::setParent(Object* parent) {
    if (!mParent) {
        mParent = parent;
    }else {
        std::cout << "Parent already exists" << std::endl;
    }
}

Object* Object::getParent() {
    return mParent;
}

void Object::addChild(Object* child) {
    if (std::find(mChildren.begin(), mChildren.end(), child) != mChildren.end()) {
        //throw std::runtime_error("Child object already exists");
        return;
    }
    mChildren.push_back(child);
    child->mParent = this;
}

void Object::removeChild(Object* child) {
    auto it = std::find(mChildren.begin(), mChildren.end(), child);
    if (it == mChildren.end()) {
        //throw std::runtime_error("Child object does not exist");
        return;
    }
    mChildren.erase(it);
    child->mParent = nullptr;
}

std::vector<Object*> Object::getChildren() {
    return mChildren;
}
