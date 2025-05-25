#include "InstancedMesh.hpp"

InstancedMesh::InstancedMesh() {
    mType = ObjectType::InstancedMesh;
}

InstancedMesh::InstancedMesh(Geometry* geometry, Material* material)
    :Mesh(geometry, material)
{
    mType = ObjectType::InstancedMesh;

    mInstanceMatrices.resize(mInstanceCount);
}

InstancedMesh::InstancedMesh(Geometry* geometry, Material* material, unsigned int instanceCount)
    :Mesh(geometry, material)
{
    mType = ObjectType::InstancedMesh;
    mInstanceCount = instanceCount;
    mInstanceMatrices.resize(mInstanceCount);
}

void InstancedMesh::instanceInit() {
    //std::cout << "InstancedMesh::instanceInit()" << std::endl;
    glGenBuffers(1,&mMatrixVbo);
    glBindBuffer(GL_ARRAY_BUFFER,mMatrixVbo);
    glBufferData(GL_ARRAY_BUFFER,mInstanceCount * sizeof(glm::mat4),mInstanceMatrices.data(), GL_DYNAMIC_DRAW);

    glBindVertexArray(mGeometry->getVAO());
    glBindBuffer(GL_ARRAY_BUFFER,mMatrixVbo);

    for (unsigned int i = 0; i < 4; i++) {
        glEnableVertexAttribArray(4 + i);
        glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * 4 * i));
        glVertexAttribDivisor(4 + i, 1);
    }
    glBindVertexArray(0);
}

InstancedMesh::~InstancedMesh() {

}

void InstancedMesh::clear() {
    mInstanceCount = 0;
    std::vector<glm::mat4>().swap(mInstanceMatrices);
}

void InstancedMesh::upDataInstanceMatrices() {
    // if (mInstanceMatrices.empty()) {
    //     std::cerr << "Warning: mInstanceMatrices is empty!" << std::endl;
    //     return;
    // }
    glBindBuffer(GL_ARRAY_BUFFER, mMatrixVbo);
    // 检查缓冲区大小是否足够
    GLint bufferSize;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
    if (bufferSize < mInstanceMatrices.size() * sizeof(glm::mat4)) {
        glBufferData(GL_ARRAY_BUFFER, mInstanceMatrices.size() * sizeof(glm::mat4),
                    mInstanceMatrices.data(), GL_DYNAMIC_DRAW);
    } else {
        glBufferSubData(GL_ARRAY_BUFFER, 0,
                       mInstanceMatrices.size() * sizeof(glm::mat4),
                       mInstanceMatrices.data());
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

