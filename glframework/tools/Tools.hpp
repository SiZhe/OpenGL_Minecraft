#pragma once
#include "../Core.hpp"

class Tools {
public:
    //传入一个矩阵，解构其中的位置 旋转信息xyz 缩放信息
    static void decompose(glm::mat4 matrix,glm::vec3& position,glm::vec3& eulerAngle,glm::vec3& scale);
};

