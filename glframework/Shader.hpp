#pragma once
#include <string>
#include "Core.hpp"

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void begin();//开始使用当前的shader

    void end();//结束使用当前的shader

    GLuint getProgram() const { return mProgram; }

    void setUniform1f(const std::string& name, float value);

    void setUniform1i(const std::string& name, int value);

    void setUniformMat4f(const std::string& name, const glm::mat4& matrix);

    void setUniformVec3(const std::string& name, const glm::vec3& vector);

    void setUniformMat4fv(const std::string& name, const glm::mat4& matrix, unsigned int count);

    void checkErrors(GLuint shader, std::string type);

private:
    GLuint mProgram{0};
};
