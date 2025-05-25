#pragma once
#include "../Core.hpp"

enum class MaterialType {
    PhongMaterial,
    WhiteMaterial,
    DepthMaterial,
    OpacityMaskMaterial,
    ScreenMaterial,
    CubeMaterial,
    PhongEnvMaterial,
    SphereMaterial,
    //mc
    McCubeMaterial,
    McPlantMaterial,
    McTakeCubeMaterial,
    McEliminateMaterial,
    McDropMaterial,
    McDropStencilMaterial,
    McCrossMaterial,
    McBagMaterial,
    McBagCubeMaterial,
    McCubeBreakMaterial,
    McWaterSurfaceMaterial,
};

class Material {
public:
    Material();
    ~Material();

    MaterialType mType;

    //深度检测相关
    bool mDepthTest{true};
    GLenum mDepthFun{GL_LEQUAL};
    bool mDepthWrite{true};
    //polygonOffset相关
    bool mPolygonOffset{false};
    unsigned int mPolygonOffsetType{GL_POLYGON_OFFSET_FILL};
    float mPolygonOffsetFactor{1.0f};
    float mPolygonOffsetUnit{1.0f};
    //stencil相关
    bool mStencilTest{false};
    unsigned int mSFail {GL_KEEP}; //模板检测通过怎么办
    unsigned int mZFail {GL_KEEP}; //模板检测通过，但深度检测不通过怎么办
    unsigned int mZPass {GL_KEEP}; //模板检测通过，深度检测通过怎么办
    unsigned int mStencilMask {0xFF};
    unsigned int mStencilFunc {GL_ALWAYS};
    unsigned int mStencilRef{1};
    unsigned int mStencilFuncMask{0x00};
    //颜色混合透明度
    bool mBlendTest{false};
    unsigned int mSFactor{GL_SRC_ALPHA};
    unsigned int mDFactor{GL_ONE_MINUS_SRC_ALPHA};
    float mOpacity{1.0f};
    //面剔除设置
    bool mFaceCulling{false};
    unsigned int mFrontFace{GL_CCW};
    unsigned int mCullFace{GL_BACK};
};