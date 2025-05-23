#include "Shader.hpp"
#include "../wrapper/CheckError.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESS" << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    GLuint vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex,1,&vShaderCode,NULL);
    glCompileShader(vertex);
    checkErrors(vertex,"COMPILE");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment,1,&fShaderCode,NULL);
    glCompileShader(fragment);
    checkErrors(fragment,"COMPILE");

    this->mProgram = glCreateProgram();
    glAttachShader(this->mProgram, vertex);
    glAttachShader(this->mProgram, fragment);
    glLinkProgram(this->mProgram);
    checkErrors(this->mProgram, "LINK");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::begin() {
    glUseProgram(this->mProgram);
}

void Shader::end() {
    glUseProgram(0);
}

void Shader::checkErrors(GLuint shader, std::string type) {
    int success;
    char infoLog[512];
    if (type == "COMPILE") {
        glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
        if (!success) {
            glGetShaderInfoLog(shader,512,NULL,infoLog);
            std::cout << "ERROR::COMPILE::COMPILE_FAILED\n" << infoLog << std::endl;
        }
    }else if (type == "LINK") {
        glGetProgramiv(shader,GL_LINK_STATUS,&success);
        if (!success) {
            glGetProgramInfoLog(shader,512,NULL,infoLog);
            std::cout << "ERROR::LINK::LINK_FAILED\n" << infoLog << std::endl;
        }
    }
}

void Shader::setUniform1f(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(this->mProgram, name.c_str()), value);
}

void Shader::setUniform1i(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(this->mProgram, name.c_str()), value);
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix) {
    glUniformMatrix4fv(glGetUniformLocation(this->mProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniformVec3(const std::string& name, const glm::vec3& vector) {
    auto location = glGetUniformLocation(this->mProgram, name.c_str());
    glUniform3fv(location, 1, glm::value_ptr(vector));
}

void Shader::setUniformMat4fv(const std::string &name, const glm::mat4 &matrix, unsigned int count) {
    auto location = glGetUniformLocation(this->mProgram, name.c_str());
    glUniformMatrix4fv(location, count, GL_FALSE, glm::value_ptr(matrix[0]));
}
