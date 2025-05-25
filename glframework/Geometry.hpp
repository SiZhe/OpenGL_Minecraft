#pragma once

#include "Core.hpp"

class Geometry {
public:
    Geometry();
    Geometry(
        const std::vector<float>& positions,
        const std::vector<float>& normals,
        const std::vector<float>& uvs,
        const std::vector<unsigned int>& indices);
    ~Geometry();

    static Geometry* CreateBox(float length,float width,float height);
    static Geometry* CreateSphere(float radius,int numLatLines, int numLongLines);
    static Geometry* CreatePlane(float length,float width);
    static Geometry* CreateScreenPlane();
    static Geometry* CreateMcCube(float width);
    //static Geometry* CreateMcCube(float width);
    static Geometry* CreateFirPlayerTakeCube();
    static Geometry* CreateMcWaterSurface(float width);

    GLuint getVAO() const {
        return mVao;
    }

    GLuint getCount() const {
        return mIndicesCount;
    }

private:
    GLuint mPosVbo{0},mUvVbo{0},mEbo{0},mVao{0},mNormal{0},mFaceVbo;
    GLuint mIndicesCount{0};
};