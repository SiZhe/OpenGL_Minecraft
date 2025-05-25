#include "CheckError.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <assert.h>


void CheckError() {
    GLenum errCode = glGetError();
    if(errCode != GL_NO_ERROR) {
        std::cout << "ErrorCode: " << errCode << std::endl;
        assert(!errCode);
    }
}
