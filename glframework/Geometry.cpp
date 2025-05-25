#include "Geometry.hpp"
#include <vector>

Geometry::Geometry() {

}

Geometry::Geometry(const std::vector<float> &positions, const std::vector<float> &normals, const std::vector<float> &uvs, const std::vector<unsigned int> &indices) {
    mIndicesCount = indices.size();

    glGenBuffers(1,&mPosVbo);
    glBindBuffer(GL_ARRAY_BUFFER,mPosVbo);
    glBufferData(GL_ARRAY_BUFFER,positions.size() * sizeof(GLfloat),positions.data(),GL_STATIC_DRAW);

    glGenBuffers(1,&mUvVbo);
    glBindBuffer(GL_ARRAY_BUFFER,mUvVbo);
    glBufferData(GL_ARRAY_BUFFER,uvs.size() * sizeof(GLfloat),uvs.data(),GL_STATIC_DRAW);

    glGenBuffers(1,&mNormal);
    glBindBuffer(GL_ARRAY_BUFFER,mNormal);
    glBufferData(GL_ARRAY_BUFFER,normals.size() * sizeof(GLfloat),normals.data(),GL_STATIC_DRAW);

    glGenBuffers(1,&mEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(GLuint),indices.data(),GL_STATIC_DRAW);

    glGenVertexArrays(1,&mVao);
    glBindVertexArray(mVao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mEbo);

    glBindBuffer(GL_ARRAY_BUFFER,mPosVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),0);

    glBindBuffer(GL_ARRAY_BUFFER,mUvVbo);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,2*sizeof(float),0);

    glBindBuffer(GL_ARRAY_BUFFER,mNormal);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,3*sizeof(float),0);

    glBindVertexArray(0);
    glDeleteBuffers(1,&mPosVbo);
    glDeleteBuffers(1,&mUvVbo);
    glDeleteBuffers(1,&mEbo);
    glDeleteBuffers(1,&mNormal);
}

Geometry::~Geometry() {
    if (mVao != 0) {
        glDeleteVertexArrays(1, &mVao);
    }
    if (mUvVbo != 0) {
        glDeleteBuffers(1,&mUvVbo);
    }
    if (mPosVbo != 0) {
        glDeleteBuffers(1,&mPosVbo);
    }
    if (mEbo != 0) {
        glDeleteBuffers(1,&mEbo);
    }
    if (mNormal != 0) {
        glDeleteBuffers(1,&mNormal);
    }
}

Geometry *Geometry::CreateBox(float length,float width,float height) {
    Geometry* geometry = new Geometry();
    geometry->mIndicesCount = 36;

    float halfWidth = width/2.0f;
    float halfHeight = height/2.0f;
    float halfLength = length/2.0f;

     // 顶点位置数据（Position Data）
    float position[] = {
        // Front face
        -halfLength, -halfHeight, halfWidth,   // 0
        halfLength, -halfHeight, halfWidth,    // 1
        halfLength, halfHeight, halfWidth,     // 2
        -halfLength, halfHeight, halfWidth,    // 3

        // Back face
        -halfLength, -halfHeight, -halfWidth,  // 4
        -halfLength, halfHeight, -halfWidth,   // 5
        halfLength, halfHeight, -halfWidth,    // 6
        halfLength, -halfHeight, -halfWidth,   // 7

        // Top face
        -halfLength, halfHeight, -halfWidth,   // 8
        -halfLength, halfHeight, halfWidth,    // 9
        halfLength, halfHeight, halfWidth,     // 10
        halfLength, halfHeight, -halfWidth,    // 11

        // Bottom face
        -halfLength, -halfHeight, -halfWidth,  // 12
        halfLength, -halfHeight, -halfWidth,   // 13
        halfLength, -halfHeight, halfWidth,    // 14
        -halfLength, -halfHeight, halfWidth,   // 15

        // Left face
        -halfLength, -halfHeight, -halfWidth,  // 16
        -halfLength, halfHeight, -halfWidth,   // 17
        -halfLength, halfHeight, halfWidth,    // 18
        -halfLength, -halfHeight, halfWidth,   // 19

        // Right face
        halfLength, -halfHeight, -halfWidth,   // 20
        halfLength, -halfHeight, halfWidth,    // 21
        halfLength, halfHeight, halfWidth,     // 22
        halfLength, halfHeight, -halfWidth,    // 23
    };

    // UV坐标数据（UV Data）
    float uv[] = {
        // Front face
        0.0f, 0.0f,   1.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f,

        // Back face
        0.0f, 0.0f,   1.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f,

        // Top face
        0.0f, 0.0f,   1.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f,

        // Bottom face
        0.0f, 0.0f,   1.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f,

        // Left face
        0.0f, 0.0f,   1.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f,

        // Right face
        0.0f, 0.0f,   1.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f,
    };

    // 索引数据（Indices）
    unsigned int indices[] = {
        // Front face
        0, 1, 2,    0, 2, 3,

        // Back face
        4, 5, 6,    4, 6, 7,

        // Top face
        8, 9, 10,   8, 10, 11,

        // Bottom face
        12, 13, 14, 12, 14, 15,

        // Left face
        16, 17, 18, 16, 18, 19,

        // Right face
        20, 21, 22, 20, 22, 23
    };

    float normal[] = {
        //Front face
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        // Back face
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        // Top face
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        // Bottom face
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,

        // Left face
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,

        //Right face
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
    };

    glGenBuffers(1,&geometry->mPosVbo);
    glBindBuffer(GL_ARRAY_BUFFER,geometry->mPosVbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(position),position,GL_STATIC_DRAW);

    glGenBuffers(1,&geometry->mUvVbo);
    glBindBuffer(GL_ARRAY_BUFFER,geometry->mUvVbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(uv),uv,GL_STATIC_DRAW);

    glGenBuffers(1,&geometry->mNormal);
    glBindBuffer(GL_ARRAY_BUFFER,geometry->mNormal);
    glBufferData(GL_ARRAY_BUFFER,sizeof(normal),normal,GL_STATIC_DRAW);

    glGenBuffers(1,&geometry->mEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,geometry->mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    glGenVertexArrays(1,&geometry->mVao);
    glBindVertexArray(geometry->mVao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,geometry->mEbo);

    glBindBuffer(GL_ARRAY_BUFFER,geometry->mPosVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);

    glBindBuffer(GL_ARRAY_BUFFER,geometry->mUvVbo);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);

    glBindBuffer(GL_ARRAY_BUFFER,geometry->mNormal);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);

    glBindVertexArray(0);
    glDeleteBuffers(1,&geometry->mPosVbo);
    glDeleteBuffers(1,&geometry->mUvVbo);
    glDeleteBuffers(1,&geometry->mEbo);
    glDeleteBuffers(1,&geometry->mNormal);

    return geometry;
}

Geometry *Geometry::CreateSphere(float radius,int numLatLines, int numLongLines) {
    Geometry* geometry = new Geometry();
    std::vector<GLfloat> position = {};
    std::vector<GLfloat> uv = {};
    std::vector<GLuint> indices = {};
    std::vector<GLfloat> normal = {};

    for (int i = 0; i <= numLatLines; i ++) {
        for (int j = 0; j <= numLongLines; j ++) {
            float theta = j * 2 * std::numbers::pi / numLongLines;
            float phi = i * std::numbers::pi / numLatLines;

            float y = radius * cos(phi);
            float x = radius * sin(phi) * cos(theta);
            float z = radius * sin(phi) * sin(theta);

            position.push_back(x);
            position.push_back(y);
            position.push_back(z);

            normal.push_back(x);
            normal.push_back(y);
            normal.push_back(z);

            float u = 1.0 - static_cast<float>(j) / static_cast<float>(numLongLines);
            float v = 1.0 - static_cast<float>(i) / static_cast<float>(numLatLines);

            uv.push_back(u);
            uv.push_back(v);
        }
    }

    for (int i = 0; i < numLatLines; i ++) {
        for (int j = 0; j < numLongLines; j ++) {
            int p1 = i * (numLatLines + 1) + j;
            int p2 = p1 + numLongLines + 1;
            int p3 = p1 + 1;
            int p4 = p2 + 1;

            indices.push_back(p1);
            indices.push_back(p2);
            indices.push_back(p3);

            indices.push_back(p3);
            indices.push_back(p2);
            indices.push_back(p4);
        }
    }

    geometry->mIndicesCount = indices.size();

    glGenBuffers(1,&geometry->mPosVbo);
    glBindBuffer(GL_ARRAY_BUFFER,geometry->mPosVbo);
    glBufferData(GL_ARRAY_BUFFER,position.size() * sizeof(GLfloat),position.data(),GL_STATIC_DRAW);

    glGenBuffers(1,&geometry->mUvVbo);
    glBindBuffer(GL_ARRAY_BUFFER,geometry->mUvVbo);
    glBufferData(GL_ARRAY_BUFFER,uv.size() * sizeof(GLfloat),uv.data(),GL_STATIC_DRAW);

    glGenBuffers(1,&geometry->mNormal);
    glBindBuffer(GL_ARRAY_BUFFER,geometry->mNormal);
    glBufferData(GL_ARRAY_BUFFER,normal.size() * sizeof(GLfloat),normal.data(),GL_STATIC_DRAW);

    glGenBuffers(1,&geometry->mEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,geometry->mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(GLuint),indices.data(),GL_STATIC_DRAW);

    glGenVertexArrays(1,&geometry->mVao);
    glBindVertexArray(geometry->mVao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,geometry->mEbo);

    glBindBuffer(GL_ARRAY_BUFFER,geometry->mPosVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),0);

    glBindBuffer(GL_ARRAY_BUFFER,geometry->mUvVbo);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,2*sizeof(float),0);

    glBindBuffer(GL_ARRAY_BUFFER,geometry->mNormal);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,3*sizeof(float),0);

    glBindVertexArray(0);
    glDeleteBuffers(1,&geometry->mPosVbo);
    glDeleteBuffers(1,&geometry->mUvVbo);
    glDeleteBuffers(1,&geometry->mEbo);
    glDeleteBuffers(1,&geometry->mNormal);

    return geometry;
}

Geometry* Geometry::CreatePlane(float length,float width) {
    Geometry* geometry = new Geometry();

    geometry->mIndicesCount = 6;

    float halfWidth = width/2.0f;
    float halfLength = length/2.0f;

    float position[] = {
        -halfLength,-halfWidth,0.0f,
        halfLength,-halfWidth,0.0f,
        halfLength,halfWidth,0.0f,
        -halfLength,halfWidth,0.0f,
    };

    float uv[] = {
    0.0f,0.0f,
    1.0f,0.0f,
    1.0f,1.0f,
    0.0f,1.0f};

    unsigned int indices[] = {
    0,1,2,
    0,2,3,};

    float normal[] = {
    0.0f,0.0f,1.0f,
    0.0f,0.0f,1.0f,
    0.0f,0.0f,1.0f,
    0.0f,0.0f,1.0f};

    glGenBuffers(1, &geometry->mPosVbo);
    glBindBuffer(GL_ARRAY_BUFFER, geometry->mPosVbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(position),position,GL_STATIC_DRAW);

    glGenBuffers(1, &geometry->mUvVbo);
    glBindBuffer(GL_ARRAY_BUFFER, geometry->mUvVbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(uv),uv,GL_STATIC_DRAW);

    glGenBuffers(1, &geometry->mNormal);
    glBindBuffer(GL_ARRAY_BUFFER, geometry->mNormal);
    glBufferData(GL_ARRAY_BUFFER,sizeof(normal),normal,GL_STATIC_DRAW);

    glGenBuffers(1, &geometry->mEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    glGenVertexArrays(1, &geometry->mVao);
    glBindVertexArray(geometry->mVao);

    glBindBuffer(GL_ARRAY_BUFFER, geometry->mPosVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, geometry->mUvVbo);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, geometry->mNormal);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);

    glBindVertexArray(0);
    glDeleteBuffers(1,&geometry->mPosVbo);
    glDeleteBuffers(1,&geometry->mUvVbo);
    glDeleteBuffers(1,&geometry->mEbo);
    glDeleteBuffers(1,&geometry->mNormal);

    return geometry;
}

Geometry *Geometry::CreateScreenPlane() {
    Geometry* geometry = new Geometry();
    geometry->mIndicesCount = 6;

    float position[] = {
        -1.0f, 1.0f,
        -1.0f, -1.0f,
        1.0f, -1.0f,
        1.0f, 1.0f,};

    float uv[] = {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
    };

    unsigned int indices[] = {
        0,1,2,
        0,2,3,
    };

    glGenBuffers(1, &geometry->mPosVbo);
    glBindBuffer(GL_ARRAY_BUFFER, geometry->mPosVbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(position),position,GL_STATIC_DRAW);

    glGenBuffers(1, &geometry->mUvVbo);
    glBindBuffer(GL_ARRAY_BUFFER, geometry->mUvVbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(uv),uv,GL_STATIC_DRAW);

    glGenBuffers(1, &geometry->mEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    glGenVertexArrays(1, &geometry->mVao);
    glBindVertexArray(geometry->mVao);

    glBindBuffer(GL_ARRAY_BUFFER, geometry->mPosVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, geometry->mUvVbo);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);

    glBindVertexArray(0);
    glDeleteBuffers(1,&geometry->mPosVbo);
    glDeleteBuffers(1,&geometry->mUvVbo);
    glDeleteBuffers(1,&geometry->mEbo);

    return geometry;
}

#if 0
Geometry *Geometry::CreateMcCube(float width) {
    Geometry* geometry = new Geometry();
    geometry->mIndicesCount = 36; // 6 faces × 2 triangles × 3 vertices = 36 indices

    float halfWidth = width/2.0f;

    // 顶点位置数据（与 CreateBox 相同）
    float position[] = {
        // Front face
        -halfWidth, -halfWidth,  halfWidth,   // 0
         halfWidth, -halfWidth,  halfWidth,   // 1
         halfWidth,  halfWidth,  halfWidth,   // 2
        -halfWidth,  halfWidth,  halfWidth,   // 3

        // Back face
        -halfWidth, -halfWidth, -halfWidth,   // 4
        -halfWidth,  halfWidth, -halfWidth,   // 5
         halfWidth,  halfWidth, -halfWidth,   // 6
         halfWidth, -halfWidth, -halfWidth,   // 7

        // Top face
        -halfWidth,  halfWidth, -halfWidth,   // 8
        -halfWidth,  halfWidth,  halfWidth,   // 9
         halfWidth,  halfWidth,  halfWidth,   // 10
         halfWidth,  halfWidth, -halfWidth,   // 11

        // Bottom face
        -halfWidth, -halfWidth, -halfWidth,   // 12
         halfWidth, -halfWidth, -halfWidth,   // 13
         halfWidth, -halfWidth,  halfWidth,   // 14
        -halfWidth, -halfWidth,  halfWidth,   // 15

        // Left face
        -halfWidth, -halfWidth, -halfWidth,   // 16
        -halfWidth,  halfWidth, -halfWidth,   // 17
        -halfWidth,  halfWidth,  halfWidth,   // 18
        -halfWidth, -halfWidth,  halfWidth,   // 19

        // Right face
         halfWidth, -halfWidth, -halfWidth,   // 20
         halfWidth, -halfWidth,  halfWidth,   // 21
         halfWidth,  halfWidth,  halfWidth,   // 22
         halfWidth,  halfWidth, -halfWidth,   // 23
    };

    // 法线数据（与 CreateBox 相同）
    float normal[] = {
        // Front face
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        // Back face
        0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,

        // Top face
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        // Bottom face
        0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,

        // Left face
        -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        // Right face
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f
    };

    // 索引数据（与 CreateBox 相同）
    unsigned int indices[] = {
        // Front face
        0, 1, 2, 0, 2, 3,
        // Back face
        4, 5, 6, 4, 6, 7,
        // Top face
        8, 9, 10, 8, 10, 11,
        // Bottom face
        12, 13, 14, 12, 14, 15,
        // Left face
        16, 17, 18, 16, 18, 19,
        // Right face
        20, 21, 22, 20, 22, 23
    };

    float uvWidth1 = 1.0f / 3.0f;
    float uv[] = {
        // Front face
        0.0f, 0.0f,   uvWidth1, 0.0f,   uvWidth1, 1.0f,   0.0f, 1.0f,

        // Back face
        uvWidth1, 0.0f,   uvWidth1, 1.0f,   0.0f, 1.0f,   0.0f, 0.0f,

        // Top face
        uvWidth1, 0.0f,   2 * uvWidth1, 0.0f,   2 * uvWidth1, 1.0f,   uvWidth1, 1.0f,

        // Bottom face
        2 * uvWidth1, 0.0f,   1.0f, 0.0f,   1.0f, 1.0f,   2 * uvWidth1, 1.0f,

        // Left face
        0.0f, 0.0f,   0.0f, 1.0f,   uvWidth1, 1.0f,   uvWidth1, 0.0f,

        // Right face
        uvWidth1, 0.0f,   0.0f, 0.0f,   0.0f, 1.0f,   uvWidth1, 1.0f,
    };

    // 创建并绑定 VBO
    glGenBuffers(1, &geometry->mPosVbo);
    glBindBuffer(GL_ARRAY_BUFFER, geometry->mPosVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

    glGenBuffers(1, &geometry->mUvVbo);
    glBindBuffer(GL_ARRAY_BUFFER, geometry->mUvVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);

    glGenBuffers(1, &geometry->mNormal);
    glBindBuffer(GL_ARRAY_BUFFER, geometry->mNormal);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normal), normal, GL_STATIC_DRAW);

    glGenBuffers(1, &geometry->mEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &geometry->mVao);
    glBindVertexArray(geometry->mVao);

    glBindBuffer(GL_ARRAY_BUFFER, geometry->mPosVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, geometry->mUvVbo);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, geometry->mNormal);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);

    glBindVertexArray(0);
    glDeleteBuffers(1,&geometry->mPosVbo);
    glDeleteBuffers(1,&geometry->mUvVbo);
    glDeleteBuffers(1,&geometry->mEbo);
    glDeleteBuffers(1,&geometry->mNormal);

    return geometry;
}
#endif

Geometry *Geometry::CreateMcCube(float width) {
    Geometry* geometry = new Geometry();
    geometry->mIndicesCount = 36;

    float half = width/2.0f;

    // 顶点位置数据（所有面按逆时针顺序排列）
    float position[] = {
        // 前面(Front) - Z正方向
        -half, -half, half,    // 0 左下
        half, -half, half,     // 1 右下
        half, half, half,      // 2 右上
        -half, half, half,     // 3 左上

        // 背面(Back) - Z负方向
        half, -half, -half,    // 4 右下
        -half, -half, -half,   // 5 左下
        -half, half, -half,    // 6 左上
        half, half, -half,     // 7 右上

        // 上面(Top) - Y正方向
        -half, half, -half,    // 8 左后
        -half, half, half,     // 9 左前
        half, half, half,      // 10 右前
        half, half, -half,     // 11 右后

        // 下面(Bottom) - Y负方向
        -half, -half, -half,   // 12 左后
        half, -half, -half,    // 13 右后
        half, -half, half,     // 14 右前
        -half, -half, half,    // 15 左前

        // 左面(Left) - X负方向
        -half, -half, -half,   // 16 下后
        -half, half, -half,    // 17 上后
        -half, half, half,     // 18 上前
        -half, -half, half,    // 19 下前

        // 右面(Right) - X正方向
        half, -half, half,     // 20 下前
        half, half, half,      // 21 上前
        half, half, -half,     // 22 上后
        half, -half, -half     // 23 下后
    };

    // UV坐标数据（与顶点顺序严格匹配）
    float uv[] = {
        // 前面 (0-3)
        0.0f, 0.0f,   1.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f,

        // 背面 (4-7)
        1.0f, 0.0f,   0.0f, 0.0f,   0.0f, 1.0f,   1.0f, 1.0f,

        // 上面 (8-11)
        0.0f, 1.0f,   0.0f, 0.0f,   1.0f, 0.0f,   1.0f, 1.0f,

        // 下面 (12-15)
        0.0f, 1.0f,   1.0f, 1.0f,   1.0f, 0.0f,   0.0f, 0.0f,

        // 左面 (16-19)
        1.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f,   0.0f, 0.0f,

        // 右面 (20-23)
        0.0f, 0.0f,   0.0f, 1.0f,   1.0f, 1.0f,   1.0f, 0.0f
    };

    // 索引数据（确保所有三角形都是逆时针顺序）
    unsigned int indices[] = {
        // 前面
        0, 1, 2,  0, 2, 3,
        // 背面
        4, 5, 6,  4, 6, 7,
        // 上面
        8, 9, 10, 8, 10, 11,
        // 下面
        12, 13, 14, 12, 14, 15,
        // 左面
        16, 17, 18, 16, 18, 19,
        // 右面
        20, 21, 22, 20, 22, 23
    };

    // 法线数据（每个顶点对应一个法线）
    float normal[] = {
        // 前面 (0-3)
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        // 背面 (4-7)
        0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
        // 上面 (8-11)
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        // 下面 (12-15)
        0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        // 左面 (16-19)
        -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        // 右面 (20-23)
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f
    };

    // 面索引数据（修正注释与实际数据匹配）
    float faceIndex[] = {
        0.0f, 0.0f, 0.0f, 0.0f,  // 前面(0)
        1.0f, 1.0f, 1.0f, 1.0f,  // 背面(1)
        2.0f, 2.0f, 2.0f, 2.0f,  // 上面(2)
        3.0f, 3.0f, 3.0f, 3.0f,  // 下面(3)
        4.0f, 4.0f, 4.0f, 4.0f,  // 左面(4)
        5.0f, 5.0f, 5.0f, 5.0f   // 右面(5)
    };

    glGenBuffers(1,&geometry->mPosVbo);
    glBindBuffer(GL_ARRAY_BUFFER,geometry->mPosVbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(position),position,GL_STATIC_DRAW);

    glGenBuffers(1,&geometry->mUvVbo);
    glBindBuffer(GL_ARRAY_BUFFER,geometry->mUvVbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(uv),uv,GL_STATIC_DRAW);

    glGenBuffers(1,&geometry->mFaceVbo);
    glBindBuffer(GL_ARRAY_BUFFER,geometry->mFaceVbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(faceIndex),faceIndex,GL_STATIC_DRAW);

    glGenBuffers(1,&geometry->mNormal);
    glBindBuffer(GL_ARRAY_BUFFER,geometry->mNormal);
    glBufferData(GL_ARRAY_BUFFER,sizeof(normal),normal,GL_STATIC_DRAW);

    glGenBuffers(1,&geometry->mEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,geometry->mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    glGenVertexArrays(1,&geometry->mVao);
    glBindVertexArray(geometry->mVao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,geometry->mEbo);

    glBindBuffer(GL_ARRAY_BUFFER,geometry->mPosVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);

    glBindBuffer(GL_ARRAY_BUFFER,geometry->mUvVbo);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);

    glBindBuffer(GL_ARRAY_BUFFER,geometry->mNormal);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);

    glBindBuffer(GL_ARRAY_BUFFER,geometry->mFaceVbo);
    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8,1,GL_FLOAT,GL_FALSE,sizeof(float),(void*)0);

    glBindVertexArray(0);
    glDeleteBuffers(1,&geometry->mPosVbo);
    glDeleteBuffers(1,&geometry->mUvVbo);
    glDeleteBuffers(1,&geometry->mFaceVbo);
    glDeleteBuffers(1,&geometry->mEbo);
    glDeleteBuffers(1,&geometry->mNormal);

    return geometry;
}

Geometry *Geometry::CreateFirPlayerTakeCube() {
    Geometry *geometry = new Geometry;
    geometry->mIndicesCount = 36;

    // 位置数据 (x, y, z)
   // 顶点位置数据（与 CreateBox 相同）
    float position[] = {
        // Front face
        -0.2f,  -0.375f,  1.0f,   // 0
         0.05f, -0.375f,  1.0f,   // 1
         0.05f, -0.125f,  1.0f,   // 2
        -0.2f,  -0.125f,  1.0f,   // 3

        // Back face
        -0.2f,  -0.375f, 0.5f,   // 4
        -0.2f,  -0.125f, 0.5f,   // 5
         0.05f, -0.125f, 0.5f,   // 6
         0.05f, -0.375f, 0.5f,   // 7

        // Top face
        -0.2f,  -0.125f,  0.5f,   // 8
        -0.2f,  -0.125f,  1.0f,   // 9
         0.05f, -0.125f,  1.0f,   // 10
         0.05f, -0.125,   0.5f,   // 11

        // Bottom face
        -0.2f,  -0.375f,  0.5f,   // 12
         0.05f, -0.375f,  0.5f,   // 13
         0.05f, -0.375f,  1.0f,   // 14
        -0.2f,  -0.375f,  1.0f,   // 15

        // Left face
        -0.2f, -0.375f,  0.5f,   // 16
        -0.2f, -0.125f,  0.5f,   // 17
        -0.2f, -0.125f,  1.0f,   // 18
        -0.2f, -0.375f,  1.0f,   // 19

        // Right face
         0.05f, -0.375f,  0.5f,   // 20
         0.05f, -0.375f,  1.0f,   // 21
         0.05f, -0.125f,  1.0f,   // 22
         0.05f, -0.125f,  0.5f,   // 23
    };

    // 索引数据（与 CreateBox 相同）
    unsigned int indices[] = {
        // Front face
        0, 1, 2, 0, 2, 3,
        // Back face
        4, 5, 6, 4, 6, 7,
        // Top face
        8, 9, 10, 8, 10, 11,
        // Bottom face
        12, 13, 14, 12, 14, 15,
        // Left face
        16, 17, 18, 16, 18, 19,
        // Right face
        20, 21, 22, 20, 22, 23
    };

    // UV坐标数据（UV Data）
    float uv[] = {
        // Front face
        0.0f, 0.0f,   1.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f,

        // Back face
        0.0f, 0.0f,   0.0f, 1.0f,   1.0f, 1.0f,   1.0f, 0.0f,

        // Top face
        0.0f, 0.0f,   1.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f,

        // Bottom face
        0.0f, 0.0f,   1.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f,

        // Left face
        0.0f, 0.0f,   0.0f, 1.0f,   1.0f, 1.0f,   1.0f, 0.0f,

        // Right face
        0.0f, 0.0f,   1.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f,
    };

    // 面索引（每个面 4 个顶点，索引相同）
    float faceIndex[] = {
        0.0f, 0.0f, 0.0f, 0.0f,  // 右（0）
        1.0f, 1.0f, 1.0f, 1.0f,  // 左（1）
        2.0f, 2.0f, 2.0f, 2.0f,  // 上（2）
        3.0f, 3.0f, 3.0f, 3.0f,  // 下（3）
        4.0f, 4.0f, 4.0f, 4.0f,  // 后（4）
        5.0f, 5.0f, 5.0f, 5.0f   // 前（5）
    };

    // 创建并绑定 VBO
    glGenBuffers(1, &geometry->mPosVbo);
    glBindBuffer(GL_ARRAY_BUFFER, geometry->mPosVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

    glGenBuffers(1, &geometry->mUvVbo);
    glBindBuffer(GL_ARRAY_BUFFER, geometry->mUvVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);

    glGenBuffers(1,&geometry->mFaceVbo);
    glBindBuffer(GL_ARRAY_BUFFER,geometry->mFaceVbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(faceIndex), faceIndex, GL_STATIC_DRAW);

    glGenBuffers(1, &geometry->mEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &geometry->mVao);
    glBindVertexArray(geometry->mVao);

    glBindBuffer(GL_ARRAY_BUFFER, geometry->mPosVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, geometry->mUvVbo);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER,geometry->mFaceVbo);
    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8,1,GL_FLOAT,GL_FALSE,sizeof(float),(void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);

    glBindVertexArray(0);
    glDeleteBuffers(1,&geometry->mPosVbo);
    glDeleteBuffers(1,&geometry->mUvVbo);
    glDeleteBuffers(1,&geometry->mFaceVbo);
    glDeleteBuffers(1,&geometry->mEbo);

    return geometry;
}


Geometry *Geometry::CreateMcWaterSurface(float width) {
    Geometry* geometry = new Geometry();
    geometry->mIndicesCount = 36;

    float half = width/2.0f;

    // 顶点位置数据（所有面按逆时针顺序排列）
    float position[] = {
        // 前面(Front) - Z正方向
        -half, -half, half,    // 0 左下
        half, -half, half,     // 1 右下
        half, half, half,      // 2 右上
        -half, half, half,     // 3 左上

        // 背面(Back) - Z负方向
        half, -half, -half,    // 4 右下
        -half, -half, -half,   // 5 左下
        -half, half, -half,    // 6 左上
        half, half, -half,     // 7 右上

        // 上面(Top) - Y正方向
        -half, half, -half,    // 8 左后
        -half, half, half,     // 9 左前
        half, half, half,      // 10 右前
        half, half, -half,     // 11 右后

        // 下面(Bottom) - Y负方向
        -half, -half, -half,   // 12 左后
        half, -half, -half,    // 13 右后
        half, -half, half,     // 14 右前
        -half, -half, half,    // 15 左前

        // 左面(Left) - X负方向
        -half, -half, -half,   // 16 下后
        -half, half, -half,    // 17 上后
        -half, half, half,     // 18 上前
        -half, -half, half,    // 19 下前

        // 右面(Right) - X正方向
        half, -half, half,     // 20 下前
        half, half, half,      // 21 上前
        half, half, -half,     // 22 上后
        half, -half, -half     // 23 下后
    };

    // UV坐标数据（与顶点顺序严格匹配）
    float uv[] = {
        // 前面 (0-3)
        0.0f, 0.0f,   1.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f,

        // 背面 (4-7)
        1.0f, 0.0f,   0.0f, 0.0f,   0.0f, 1.0f,   1.0f, 1.0f,

        // 上面 (8-11)
        0.0f, 1.0f,   0.0f, 0.0f,   1.0f, 0.0f,   1.0f, 1.0f,

        // 下面 (12-15)
        0.0f, 1.0f,   1.0f, 1.0f,   1.0f, 0.0f,   0.0f, 0.0f,

        // 左面 (16-19)
        1.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f,   0.0f, 0.0f,

        // 右面 (20-23)
        0.0f, 0.0f,   0.0f, 1.0f,   1.0f, 1.0f,   1.0f, 0.0f
    };

    // 索引数据（确保所有三角形都是逆时针顺序）
    unsigned int indices[] = {
        // 前面
        0, 1, 2,  0, 2, 3,
        // 背面
        4, 5, 6,  4, 6, 7,
        // 上面
        8, 9, 10, 8, 10, 11,
        // 下面
        12, 13, 14, 12, 14, 15,
        // 左面
        16, 17, 18, 16, 18, 19,
        // 右面
        20, 21, 22, 20, 22, 23
    };

    // 法线数据（每个顶点对应一个法线）
    float normal[] = {
        // 前面 (0-3)
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        // 背面 (4-7)
        0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
        // 上面 (8-11)
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        // 下面 (12-15)
        0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        // 左面 (16-19)
        -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        // 右面 (20-23)
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f
    };

    // 面索引数据（修正注释与实际数据匹配）
    float faceIndex[] = {
        0.0f, 0.0f, 0.0f, 0.0f,  // 前面(0)
        1.0f, 1.0f, 1.0f, 1.0f,  // 背面(1)
        2.0f, 2.0f, 2.0f, 2.0f,  // 上面(2)
        3.0f, 3.0f, 3.0f, 3.0f,  // 下面(3)
        4.0f, 4.0f, 4.0f, 4.0f,  // 左面(4)
        5.0f, 5.0f, 5.0f, 5.0f   // 右面(5)
    };

    glGenBuffers(1,&geometry->mPosVbo);
    glBindBuffer(GL_ARRAY_BUFFER,geometry->mPosVbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(position),position,GL_STATIC_DRAW);

    glGenBuffers(1,&geometry->mUvVbo);
    glBindBuffer(GL_ARRAY_BUFFER,geometry->mUvVbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(uv),uv,GL_STATIC_DRAW);

    glGenBuffers(1,&geometry->mFaceVbo);
    glBindBuffer(GL_ARRAY_BUFFER,geometry->mFaceVbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(faceIndex),faceIndex,GL_STATIC_DRAW);

    glGenBuffers(1,&geometry->mNormal);
    glBindBuffer(GL_ARRAY_BUFFER,geometry->mNormal);
    glBufferData(GL_ARRAY_BUFFER,sizeof(normal),normal,GL_STATIC_DRAW);

    glGenBuffers(1,&geometry->mEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,geometry->mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    glGenVertexArrays(1,&geometry->mVao);
    glBindVertexArray(geometry->mVao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,geometry->mEbo);

    glBindBuffer(GL_ARRAY_BUFFER,geometry->mPosVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);

    glBindBuffer(GL_ARRAY_BUFFER,geometry->mUvVbo);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);

    glBindBuffer(GL_ARRAY_BUFFER,geometry->mNormal);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);

    glBindBuffer(GL_ARRAY_BUFFER,geometry->mFaceVbo);
    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8,1,GL_FLOAT,GL_FALSE,sizeof(float),(void*)0);

    glBindVertexArray(0);
    glDeleteBuffers(1,&geometry->mPosVbo);
    glDeleteBuffers(1,&geometry->mUvVbo);
    glDeleteBuffers(1,&geometry->mFaceVbo);
    glDeleteBuffers(1,&geometry->mEbo);
    glDeleteBuffers(1,&geometry->mNormal);

    return geometry;
}