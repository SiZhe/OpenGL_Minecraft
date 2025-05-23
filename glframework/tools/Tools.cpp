#include "Tools.hpp"

void Tools::decompose(glm::mat4 matrix, glm::vec3 &position, glm::vec3 &eulerAngle, glm::vec3 &scale) {
    //四元数，用来表示旋转变换
    glm::quat quaterion;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(matrix,scale,quaterion,position,skew,perspective);

    //将四元数 变成 欧拉角
    glm::mat4 rotation = glm::toMat4(quaterion);
    glm::extractEulerAngleXYZ(rotation,eulerAngle.x,eulerAngle.y,eulerAngle.z);

    //解构的为弧度
    eulerAngle.x = glm::degrees(eulerAngle.x);
    eulerAngle.y = glm::degrees(eulerAngle.y);
    eulerAngle.z = glm::degrees(eulerAngle.z);
}
