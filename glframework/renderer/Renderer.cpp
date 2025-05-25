#include "Renderer.hpp"
#include <iostream>
#include <string>

#include "../material/OpacityMaskMaterial.hpp"
#include "../material/PhongMaterial.hpp"
#include "../material/WhiteMaterial.hpp"
#include "../material/OpacityMaskMaterial.hpp"
#include "../material/ScreenMaterial.hpp"
#include "../material/CubeMaterial.hpp"
#include "../material/PhongEnvMaterial.hpp"
#include "../material/SphereMaterial.hpp"

//Mc
#include "../../application/Application.hpp"
#include "../material/mc/McCubeMaterial.hpp"
#include "../material/mc/McPlantMaterial.hpp"
#include "../material/mc/McTakeCubeMaterial.hpp"
#include "../material/mc/McEliminateMaterial.hpp"
#include "../material/mc/McDropMaterial.hpp"
#include "../material/mc/McDropStencilMaterial.hpp"
#include "../material/mc/McCrossMaterial.hpp"
#include "../material/mc/McBagMaterial.hpp"
#include "../material/mc/McBagCubeMaterial.hpp"
#include "../material/mc/McCubeBreakMaterial.hpp"
#include "../material/mc/McWaterSurfaceMaterial.hpp"

Renderer::Renderer() {
    mPhongShader = new Shader("../assets/shaders/phong.vert", "../assets/shaders/phong.frag");
    mWhiteShader = new Shader("../assets/shaders/white.vert", "../assets/shaders/white.frag");
    mDepthShader = new Shader("../assets/shaders/depth.vert", "../assets/shaders/depth.frag");
    mOpacityMaskShader = new Shader("../assets/shaders/phongOpacityMask.vert", "../assets/shaders/phongOpacityMask.frag");
    mScreenShader = new Shader("../assets/shaders/screen.vert", "../assets/shaders/screen.frag");
    mCubeShader = new Shader("../assets/shaders/cube.vert", "../assets/shaders/cube.frag");
    mEnvShader = new Shader("../assets/shaders/phongEnv.vert", "../assets/shaders/phongEnv.frag");
    mSphereShader = new Shader("../assets/shaders/sphere.vert", "../assets/shaders/sphere.frag");
    //Mc
    mMcCubeShader = new Shader ("../assets/shaders/mc/mcCube.vert", "../assets/shaders/mc/mcCube.frag");

    mMcPlantShader = new Shader("../assets/shaders/mc/mcPlant.vert", "../assets/shaders/mc/mcPlant.frag");

    mMcTakeCubeShader = new Shader ("../assets/shaders/mc/mcTakeCube.vert", "../assets/shaders/mc/mcTakeCube.frag");

    mMcEliminateShader = new Shader("../assets/shaders/mc/mcEliminateAndDrop.vert", "../assets/shaders/mc/mcEliminateAndDrop.frag");
    mMcDropShader = new Shader("../assets/shaders/mc/mcEliminateAndDrop.vert", "../assets/shaders/mc/mcEliminateAndDrop.frag");
    mMcDropStencilShader =  new Shader("../assets/shaders/mc/mcDropStencil.vert", "../assets/shaders/mc/mcDropStencil.frag");

    mMcCrossShader = new Shader("../assets/shaders/mc/mcCross.vert", "../assets/shaders/mc/mcCross.frag");
    mMcBagShader = new Shader("../assets/shaders/mc/mcBag.vert", "../assets/shaders/mc/mcBag.frag");
    mMcBagCubeShader = new Shader("../assets/shaders/mc/mcBagCube.vert", "../assets/shaders/mc/mcBagCube.frag");
    mMcCubeBreakShader = new Shader("../assets/shaders/mc/mcCubeBreak.vert", "../assets/shaders/mc/mcCubeBreak.frag");
    mMcWaterSurfaceShader = new Shader("../assets/shaders/mc/mcWaterSurface.vert", "../assets/shaders/mc/mcWaterSurface.frag");
}

void Renderer::setDepthState(Material* material) {
    //设置深度渲染
    if (material->mDepthTest) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(material->mDepthFun);
    }else {
        glDisable(GL_DEPTH_TEST);
    }

    if (material->mDepthWrite) {
        glDepthMask(GL_TRUE);
    }else {
        glDepthMask(GL_FALSE);
    }
}

void Renderer::setPolygonOffsetState(Material* material) {
    //设置polygonOffset渲染状态
    if (material->mPolygonOffset) {
        glEnable(material->mPolygonOffsetType);
        glPolygonOffset(material->mPolygonOffsetFactor, material->mPolygonOffsetUnit);
    }else {
        glDisable(GL_POLYGON_OFFSET_FILL);
        glDisable(GL_POLYGON_OFFSET_LINE);
    }
}

void Renderer::setStencilState(Material* material) {
    //设置stencil渲染状态
    if (material->mStencilTest) {
        glEnable(GL_STENCIL_TEST);
        glStencilOp(material->mSFail,material->mZFail,material->mZPass);
        glStencilMask(material->mStencilMask);
        glStencilFunc(material->mStencilFunc,material->mStencilRef,material->mStencilFuncMask);
    }else {
        glDisable(GL_STENCIL_TEST);
    }
}

void Renderer::setBlendState(Material* material) {
    if (material->mBlendTest) {
        glEnable(GL_BLEND);
        glBlendFunc(material->mSFactor,material->mDFactor);
    }else {
        glDisable(GL_BLEND);
    }
}

void Renderer::setFaceCullState(Material* material) {
    if (material->mFaceCulling) {
        glEnable(GL_CULL_FACE);
        glFrontFace(material->mFrontFace);
        glCullFace(material->mCullFace);
    }else {
        glDisable(GL_CULL_FACE);
    }
}

void Renderer::renderObject(
    Object* object,
    Camera* camera,
    DirectionLight* directionLight,
    const std::vector<std::shared_ptr<PointLight> > &pointLights,
    SpotLight* spotLight,
    AmbientLight* ambientLight)
{
    // 判断是Mesh还是Object，如果是Mesh需要渲染
    if (object->getType() == ObjectType::Mesh) {
        Mesh* mesh = static_cast<Mesh*>(object);
        Geometry* geometry = mesh->mGeometry;

        Material* material = nullptr;
        if (mGlobalMaterial != nullptr) {
            material = mGlobalMaterial;
        }else {
            material = mesh->mMaterial;
        }

        //设置深度渲染
        setDepthState(material);
        //设置polygonOffset渲染状态
        setPolygonOffsetState(material);
        //设置stencil渲染状态
        setStencilState(material);
        //设置blend渲染状态
        setBlendState(material);
        //设置faceCull状态
        setFaceCullState(material);

        Shader* shader = pickShader(material->mType);
        shader->begin();

        switch (material->mType) {
            case MaterialType::PhongMaterial: {
                PhongMaterial* phongMaterial = static_cast<PhongMaterial*>(material);

                //mvp变化矩阵
                shader->setUniformMat4f("transform", mesh->getModelMatrix());
                shader->setUniformMat4f("viewMatrix", camera->GetViewMatrix());
                shader->setUniformMat4f("projectMatrix", camera->GetProjectionMatrix());

                //光源
                shader->setUniformVec3("directionLight.direction",directionLight->mDirection);
                shader->setUniformVec3("directionLight.color",directionLight->mColor);
                shader->setUniform1f("directionLight.specularIntensity",directionLight->mSpecularIntensity);

                for (int i = 0; i < pointLights.size(); i++) {
                    std::string BaseName{"pointLights["};
                    BaseName += std::to_string(i);
                    BaseName += "]";

                    shader->setUniformVec3(BaseName + ".position",pointLights[i]->getPosition());
                    shader->setUniformVec3(BaseName + ".color",pointLights[i]->mColor);
                    shader->setUniform1f(BaseName + ".k2",pointLights[i]->mK2);
                    shader->setUniform1f(BaseName + ".k1",pointLights[i]->mK1);
                    shader->setUniform1f(BaseName + ".kc",pointLights[i]->mKc);
                    shader->setUniform1f(BaseName + ".specularIntensity",pointLights[i]->mSpecularIntensity);
                }

                shader->setUniformVec3("spotLight.position",spotLight->getPosition());
                shader->setUniformVec3("spotLight.color",spotLight->mColor);
                shader->setUniform1f("spotLight.innerCos", glm::cos(spotLight->mInnerAngle));
                shader->setUniform1f("spotLight.outerCos",glm::cos(spotLight->mOuterAngle));
                shader->setUniformVec3("spotLight.targetDirection",spotLight->mTargetDirection);
                shader->setUniform1f("spotLight.specularIntensity",spotLight->mSpecularIntensity);

                shader->setUniformVec3("ambientColor",ambientLight->mColor);
                shader->setUniform1f("shininess",phongMaterial->mShininess);

                //相机
                shader->setUniformVec3("cameraPosition",camera->mPosition);

                shader->setUniform1f("time", glfwGetTime());
                shader->setUniform1f("opacity",material->mOpacity);

                shader->setUniform1i("sampler", 0);
                phongMaterial->mDiffuse->Bind();

                shader->setUniform1i("specularMaskSampler", 1);
                phongMaterial->mSpecularMask->Bind();
            }
                break;
            case MaterialType::WhiteMaterial: {
                shader->setUniformMat4f("transform", mesh->mModelMatrix);
                shader->setUniformMat4f("viewMatrix", camera->GetViewMatrix());
                shader->setUniformMat4f("projectMatrix", camera->GetProjectionMatrix());
            }
                break;
            case MaterialType::DepthMaterial: {
                shader->setUniformMat4f("transform", mesh->getModelMatrix());
                shader->setUniformMat4f("viewMatrix", camera->GetViewMatrix());
                shader->setUniformMat4f("projectMatrix", camera->GetProjectionMatrix());
                shader->setUniform1f("near",camera->mNear);
                shader->setUniform1f("far",camera->mFar);
            }
                break;
            case MaterialType::OpacityMaskMaterial: {
                OpacityMaskMaterial* opacityMaskMaterial = static_cast<OpacityMaskMaterial*>(material);

                //mvp变化矩阵
                shader->setUniformMat4f("transform", mesh->getModelMatrix());
                shader->setUniformMat4f("viewMatrix", camera->GetViewMatrix());
                shader->setUniformMat4f("projectMatrix", camera->GetProjectionMatrix());

                //光源
                shader->setUniformVec3("directionLight.direction",directionLight->mDirection);
                shader->setUniformVec3("directionLight.color",directionLight->mColor);
                shader->setUniform1f("directionLight.specularIntensity",directionLight->mSpecularIntensity);

                for (int i = 0; i < pointLights.size(); i++) {
                    std::string BaseName{"pointLights["};
                    BaseName += std::to_string(i);
                    BaseName += "]";

                    shader->setUniformVec3(BaseName + ".position",pointLights[i]->getPosition());
                    shader->setUniformVec3(BaseName + ".color",pointLights[i]->mColor);
                    shader->setUniform1f(BaseName + ".k2",pointLights[i]->mK2);
                    shader->setUniform1f(BaseName + ".k1",pointLights[i]->mK1);
                    shader->setUniform1f(BaseName + ".kc",pointLights[i]->mKc);
                    shader->setUniform1f(BaseName + ".specularIntensity",pointLights[i]->mSpecularIntensity);
                }

                shader->setUniformVec3("spotLight.position",spotLight->getPosition());
                shader->setUniformVec3("spotLight.color",spotLight->mColor);
                shader->setUniform1f("spotLight.innerCos", glm::cos(spotLight->mInnerAngle));
                shader->setUniform1f("spotLight.outerCos",glm::cos(spotLight->mOuterAngle));
                shader->setUniformVec3("spotLight.targetDirection",spotLight->mTargetDirection);
                shader->setUniform1f("spotLight.specularIntensity",spotLight->mSpecularIntensity);

                shader->setUniformVec3("ambientColor",ambientLight->mColor);
                shader->setUniform1f("shininess",opacityMaskMaterial->mShininess);

                //相机
                shader->setUniformVec3("cameraPosition",camera->mPosition);

                shader->setUniform1f("time", glfwGetTime());
                shader->setUniform1f("opacity",material->mOpacity);

                shader->setUniform1i("sampler", 0);
                opacityMaskMaterial->mDiffuse->Bind();

                shader->setUniform1i("opacityMaskSampler", 1);
                opacityMaskMaterial->mOpacityMask->Bind();
            }
                break;
            case MaterialType::ScreenMaterial:{
                ScreenMaterial* screenMaterial = static_cast<ScreenMaterial*>(material);
                shader->setUniform1i("screenTexSampler",0);
                shader->setUniform1f("texWidth",glApp->getWidth());
                shader->setUniform1f("texHeight",glApp->getHeight());
                screenMaterial->mScreenTexture->Bind();
            }
                break;
            case MaterialType::CubeMaterial: {
                CubeMaterial* cubeMaterial = static_cast<CubeMaterial*>(material);
                mesh->setPosition(camera->mPosition);

                shader->setUniformMat4f("transform", mesh->getModelMatrix());
                shader->setUniformMat4f("viewMatrix", camera->GetViewMatrix());
                shader->setUniformMat4f("projectMatrix",camera->GetProjectionMatrix());

                shader->setUniform1i("cubeSampler",0);
                cubeMaterial->mCubeTexture->Bind();
            }
                break;
            case MaterialType::PhongEnvMaterial: {
                PhongEnvMaterial* phongEnvMaterial = static_cast<PhongEnvMaterial*>(material);

                shader->setUniformMat4f("transform", mesh->getModelMatrix());
                shader->setUniformMat4f("viewMatrix", camera->GetViewMatrix());
                shader->setUniformMat4f("projectMatrix",camera->GetProjectionMatrix());

                shader->setUniformVec3("cameraPositon",camera->mPosition);

                shader->setUniformVec3("ambientColor",ambientLight->mColor);
                shader->setUniform1f("shininess",phongEnvMaterial->mShininess);

                shader->setUniformVec3("directionLight.direction",directionLight->mDirection);
                shader->setUniformVec3("directionLight.color",directionLight->mColor);
                shader->setUniform1f("directionLight.specularIntensity",directionLight->mSpecularIntensity);

                for (int i = 0; i < pointLights.size(); i++) {
                    std::string BaseName = "pointLights[" ;
                    BaseName.append(std::to_string(i));
                    BaseName.append("]");

                    shader->setUniformVec3(BaseName + ".position",pointLights[i]->getPosition());
                    shader->setUniformVec3(BaseName + ".color",pointLights[i]->mColor);
                    shader->setUniform1f(BaseName + ".specularIntensity",pointLights[i]->mSpecularIntensity);
                    shader->setUniform1f(BaseName + ".k2",pointLights[i]->mK2);
                    shader->setUniform1f(BaseName + ".k1",pointLights[i]->mK1);
                    shader->setUniform1f(BaseName + ".kc",pointLights[i]->mKc);
                }

                shader->setUniformVec3("spotLight.position",spotLight->getPosition());
                shader->setUniformVec3("spotLight.color",spotLight->mColor);
                shader->setUniform1f("spotLight.specularIntensity",spotLight->mSpecularIntensity);
                shader->setUniformVec3("spotLight.targetDirection",spotLight->mTargetDirection);
                shader->setUniform1f("spotLight.innerCos",glm::cos(spotLight->mInnerAngle));
                shader->setUniform1f("spotLight.outerCos",glm::cos(spotLight->mOuterAngle));


                shader->setUniform1f("time",glfwGetTime());
                shader->setUniform1f("opacity",material->mOpacity);

                shader->setUniform1i("sampler",0);
                phongEnvMaterial->mDiffuse->Bind();

                shader->setUniform1i("specularMaskSampler",1);
                phongEnvMaterial->mSpecularMask->Bind();

                shader->setUniform1i("envSampler",2);
                phongEnvMaterial->mEnv->Bind();
            }
                break;
            case MaterialType::SphereMaterial: {
                SphereMaterial* sphereMaterial = static_cast<SphereMaterial*>(material);
                mesh->setPosition(camera->mPosition);

                shader->setUniformMat4f("transform", mesh->getModelMatrix());
                shader->setUniformMat4f("viewMatrix", camera->GetViewMatrix());
                shader->setUniformMat4f("projectMatrix",camera->GetProjectionMatrix());

                shader->setUniform1i("sphereSampler",0);
                sphereMaterial->mSphereTexture->Bind();
            }
                break;
            case MaterialType::McTakeCubeMaterial:{
                McTakeCubeMaterial* mcTakeCubeMaterial = static_cast<McTakeCubeMaterial*>(material);

                //mvp变化矩阵
                shader->setUniformMat4f("modelMatrix", mesh->mModelMatrix);
                shader->setUniformMat4f("viewMatrix", camera->GetViewMatrix());
                shader->setUniformMat4f("projectMatrix", camera->GetProjectionMatrix());

                shader->setUniform1i("sampler", 0);
                mcTakeCubeMaterial->mDiffuse->Bind();
            }
                break;
            case MaterialType::McCubeBreakMaterial: {
                McCubeBreakMaterial* mcCubeBreakMaterial = static_cast<McCubeBreakMaterial*>(material);

                //mvp变化矩阵
                shader->setUniformMat4f("modelMatrix", mesh->mModelMatrix);
                shader->setUniformMat4f("viewMatrix", camera->GetViewMatrix());
                shader->setUniformMat4f("projectMatrix", camera->GetProjectionMatrix());

                shader->setUniform1i("sampler", 0);
                mcCubeBreakMaterial->mDiffuse->Bind();
            }
                break;
            case MaterialType::McCrossMaterial:{
                McCrossMaterial* mcCrossMaterial = static_cast<McCrossMaterial*>(material);
                shader->setUniform1i("sampler", 0);
                mcCrossMaterial->mDiffuse->Bind();
            }
                break;
            case MaterialType::McBagMaterial:{
                McBagMaterial* mcBagMaterial = static_cast<McBagMaterial*>(material);
                shader->setUniformMat4f("modelMatrix",  mesh->getModelMatrix());
                shader->setUniform1i("sampler", 0);
                mcBagMaterial->mDiffuse->Bind();
            }
                break;
            case MaterialType::McBagCubeMaterial:{
                McBagCubeMaterial* mcBagCubeMaterial = static_cast<McBagCubeMaterial*>(material);
                shader->setUniformMat4f("modelMatrix", mesh->getModelMatrix());
                shader->setUniform1i("sampler", 0);
                mcBagCubeMaterial->mDiffuse->Bind();
            }
                break;
            default:
                break;
        }

        //绑定Vao
        glBindVertexArray(geometry->getVAO());
        //执行绘制命令
        glDrawElements(GL_TRIANGLES,geometry->getCount(),GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
    }

    if (object->getType() == ObjectType::InstancedMesh) {
        InstancedMesh* mesh = static_cast<InstancedMesh*>(object);
        Geometry* geometry = mesh->mGeometry;

        Material* material = nullptr;
        if (mGlobalMaterial != nullptr) {
            material = mGlobalMaterial;
        }else {
            material = mesh->mMaterial;
        }

        //设置深度渲染
        setDepthState(material);
        //设置polygonOffset渲染状态
        setPolygonOffsetState(material);
        //设置stencil渲染状态
        setStencilState(material);
        //设置blend渲染状态
        setBlendState(material);
        //设置faceCull状态
        setFaceCullState(material);

        //std::cout << "ObjectType::InstancedMesh:"<<  static_cast<int>(material->mType)<<std::endl;
        Shader* shader = pickShader(material->mType);

        shader->begin();

        switch (material->mType) {
                //Mc
            case MaterialType::McCubeMaterial: {
                McCubeMaterial* mcCubeMaterial = static_cast<McCubeMaterial*>(material);
                //std::cout << glm::to_string(mesh->mInstanceMatrices[0]) << std::endl;
                //mvp变化矩阵
                shader->setUniformMat4f("modelMatrix", mesh->getModelMatrix());
                shader->setUniformMat4f("viewMatrix", camera->GetViewMatrix());
                shader->setUniformMat4f("projectMatrix", camera->GetProjectionMatrix());

                //光源
                shader->setUniformVec3("directionLight.direction",directionLight->mDirection);
                shader->setUniformVec3("directionLight.color",directionLight->mColor);
                shader->setUniform1f("directionLight.specularIntensity",directionLight->mSpecularIntensity);

                for (int i = 0; i < pointLights.size(); i++) {
                    std::string BaseName{"pointLights["};
                    BaseName += std::to_string(i);
                    BaseName += "]";

                    shader->setUniformVec3(BaseName + ".position",pointLights[i]->getPosition());
                    shader->setUniformVec3(BaseName + ".color",pointLights[i]->mColor);
                    shader->setUniform1f(BaseName + ".k2",pointLights[i]->mK2);
                    shader->setUniform1f(BaseName + ".k1",pointLights[i]->mK1);
                    shader->setUniform1f(BaseName + ".kc",pointLights[i]->mKc);
                    shader->setUniform1f(BaseName + ".specularIntensity",pointLights[i]->mSpecularIntensity);
                }

                shader->setUniformVec3("spotLight.position",spotLight->getPosition());
                shader->setUniformVec3("spotLight.color",spotLight->mColor);
                shader->setUniform1f("spotLight.innerCos", glm::cos(spotLight->mInnerAngle));
                shader->setUniform1f("spotLight.outerCos",glm::cos(spotLight->mOuterAngle));
                shader->setUniformVec3("spotLight.targetDirection",spotLight->mTargetDirection);
                shader->setUniform1f("spotLight.specularIntensity",spotLight->mSpecularIntensity);

                shader->setUniformVec3("ambientColor",ambientLight->mColor);
                shader->setUniform1f("shininess",mcCubeMaterial->mShininess);

                //相机
                shader->setUniformVec3("cameraPosition",camera->mPosition);

                shader->setUniform1f("time", glfwGetTime());
                shader->setUniform1f("opacity",material->mOpacity);

                shader->setUniform1i("sampler", 0);
                mcCubeMaterial->mDiffuse->Bind();

                shader->setUniform1i("specularMaskSampler", 1);
                mcCubeMaterial->mSpecularMask->Bind();
            }
                break;
            case MaterialType::McPlantMaterial: {
                McPlantMaterial* mcPlantMaterial = static_cast<McPlantMaterial*>(material);

                shader->setUniformMat4f("modelMatrix", mesh->getModelMatrix());
                shader->setUniformMat4f("viewMatrix", camera->GetViewMatrix());
                shader->setUniformMat4f("projectMatrix",camera->GetProjectionMatrix());

                shader->setUniform1f("opacity",material->mOpacity);
                shader->setUniform1i("sampler",0);
                mcPlantMaterial->mDiffuse->Bind();
                //shader->setUniform1i("opacityMaskSampler",1);
                //mcPlantMaterial->mOpacityMask->Bind();
            }
                break;
            case MaterialType::McEliminateMaterial: {
                McEliminateMaterial* mcEliminateMaterial = static_cast<McEliminateMaterial*>(material);

                //mvp变化矩阵
                shader->setUniformMat4f("modelMatrix", mesh->getModelMatrix());
                shader->setUniformMat4f("viewMatrix", camera->GetViewMatrix());
                shader->setUniformMat4f("projectMatrix", camera->GetProjectionMatrix());

                shader->setUniform1i("sampler", 0);
                mcEliminateMaterial->mDiffuse->Bind();
            }
                break;
            case MaterialType::McDropMaterial: {
                McDropMaterial* mcDropMaterial = static_cast<McDropMaterial*>(material);

                shader->setUniformMat4f("modelMatrix", mesh->getModelMatrix());
                shader->setUniformMat4f("viewMatrix", camera->GetViewMatrix());
                shader->setUniformMat4f("projectMatrix",camera->GetProjectionMatrix());

                shader->setUniform1f("opacity",material->mOpacity);
                shader->setUniform1i("sampler",0);
                mcDropMaterial->mDiffuse->Bind();
                //shader->setUniform1i("opacityMaskSampler",1);
                //mcPlantMaterial->mOpacityMask->Bind();
            }
                break;
            case MaterialType::McDropStencilMaterial: {
                shader->setUniformMat4f("modelMatrix", mesh->mModelMatrix);
                shader->setUniformMat4f("viewMatrix", camera->GetViewMatrix());
                shader->setUniformMat4f("projectMatrix", camera->GetProjectionMatrix());
            }
                break;
            case MaterialType::McWaterSurfaceMaterial: {
                McWaterSurfaceMaterial* mcWaterSurfaceMateria = static_cast<McWaterSurfaceMaterial*>(material);
                //mvp变化矩阵
                shader->setUniformMat4f("modelMatrix", mesh->getModelMatrix());
                shader->setUniformMat4f("viewMatrix", camera->GetViewMatrix());
                shader->setUniformMat4f("projectMatrix", camera->GetProjectionMatrix());

                //光源
                shader->setUniformVec3("directionLight.direction",directionLight->mDirection);
                shader->setUniformVec3("directionLight.color",directionLight->mColor);
                shader->setUniform1f("directionLight.specularIntensity",directionLight->mSpecularIntensity);

                for (int i = 0; i < pointLights.size(); i++) {
                    std::string BaseName{"pointLights["};
                    BaseName += std::to_string(i);
                    BaseName += "]";

                    shader->setUniformVec3(BaseName + ".position",pointLights[i]->getPosition());
                    shader->setUniformVec3(BaseName + ".color",pointLights[i]->mColor);
                    shader->setUniform1f(BaseName + ".k2",pointLights[i]->mK2);
                    shader->setUniform1f(BaseName + ".k1",pointLights[i]->mK1);
                    shader->setUniform1f(BaseName + ".kc",pointLights[i]->mKc);
                    shader->setUniform1f(BaseName + ".specularIntensity",pointLights[i]->mSpecularIntensity);
                }

                shader->setUniformVec3("spotLight.position",spotLight->getPosition());
                shader->setUniformVec3("spotLight.color",spotLight->mColor);
                shader->setUniform1f("spotLight.innerCos", glm::cos(spotLight->mInnerAngle));
                shader->setUniform1f("spotLight.outerCos",glm::cos(spotLight->mOuterAngle));
                shader->setUniformVec3("spotLight.targetDirection",spotLight->mTargetDirection);
                shader->setUniform1f("spotLight.specularIntensity",spotLight->mSpecularIntensity);

                shader->setUniformVec3("ambientColor",ambientLight->mColor);
                shader->setUniform1f("shininess",mcWaterSurfaceMateria->mShininess);

                //相机
                shader->setUniformVec3("cameraPosition",camera->mPosition);

                shader->setUniform1f("time", glfwGetTime());
                shader->setUniform1f("opacity",material->mOpacity);

                shader->setUniform1i("sampler", 0);
                mcWaterSurfaceMateria->mDiffuse->Bind();

                shader->setUniform1i("specularMaskSampler", 1);
                mcWaterSurfaceMateria->mSpecularMask->Bind();
            }
                break;
            default:
                break;
        }
        //绑定Vao
        glBindVertexArray(geometry->getVAO());
        //执行绘制命令
        glDrawElementsInstanced(GL_TRIANGLES,geometry->getCount(),GL_UNSIGNED_INT,0,mesh->mInstanceCount);
        glBindVertexArray(0);
    }
}

Renderer::~Renderer() {

}

void Renderer::render(
        //const std::vector<Mesh*>& meshes,
        Scene* scene,
        Camera* camera,
        DirectionLight* directionLight,
        const std::vector<std::shared_ptr<PointLight>>& pointLights,
        SpotLight* spotLight,
        AmbientLight* ambientLight,
        unsigned int fbo)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    //设置当前帧绘制的opengl的必要状态机
    //深度检测
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    //POLYGON_OFFSET检测
    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_POLYGON_OFFSET_LINE);
    //模板检测
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF);
    //透明度检测 BLEND
    glDisable(GL_BLEND);
    //面剔除
    glDisable(GL_CULL_FACE);

    //清理画布
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //清空两个队列
    mOpacityMeshes.clear();
    mTransparentMeshes.clear();
    projectObject(scene);

    std::sort(
        mTransparentMeshes.begin(),
        mTransparentMeshes.end(),
        [camera](const auto a, const auto b) {
            auto viewMatrix = camera->GetViewMatrix();

            auto modelMatrixA = a->getModelMatrix();
            auto worldPositionA = modelMatrixA * glm::vec4(0.0f, 0.0f,0.0f,1.0f);
            auto cameraPositionA = viewMatrix * worldPositionA;

            auto modelMatrixB = b->getModelMatrix();
            auto worldPositionB = modelMatrixB * glm::vec4(0.0f, 0.0f,0.0f,1.0f);
            auto cameraPositionB = viewMatrix * worldPositionB;

            return cameraPositionA.z < cameraPositionB.z;
        });

    for (auto mesh : mOpacityMeshes) {
        renderObject(mesh, camera, directionLight, pointLights, spotLight, ambientLight);
    }

    for (auto mesh : mTransparentMeshes) {
        renderObject(mesh, camera, directionLight, pointLights, spotLight, ambientLight);
    }

}

Shader *Renderer::pickShader(MaterialType type) {
    //std::cout << "material type:"<< static_cast<int>(type) << std::endl;
    Shader* result = nullptr;

    switch (type) {
        case MaterialType::PhongMaterial:
            result = mPhongShader;
            break;
        case MaterialType::WhiteMaterial:
            result = mWhiteShader;
            break;
        case MaterialType::DepthMaterial:
            result = mDepthShader;
            break;
        case MaterialType::OpacityMaskMaterial:
            result = mOpacityMaskShader;
            break;
        case MaterialType::ScreenMaterial:
            result = mScreenShader;
            break;
        case MaterialType::CubeMaterial:
            result = mCubeShader;
            break;
        case MaterialType::PhongEnvMaterial:
            result = mEnvShader;
            break;
        case MaterialType::SphereMaterial:
            result = mSphereShader;
            break;
        //Mc
        case MaterialType::McCubeMaterial:
            result = mMcCubeShader;
            break;
        case MaterialType::McPlantMaterial:
            result = mMcPlantShader;
            break;
        case MaterialType::McTakeCubeMaterial:
            result = mMcTakeCubeShader;
            break;
        case MaterialType::McEliminateMaterial:
            result = mMcEliminateShader;
            break;
        case MaterialType::McDropMaterial:
            result = mMcDropShader;
            break;
        case MaterialType::McDropStencilMaterial:
            result = mMcDropStencilShader;
            break;
        case MaterialType::McCrossMaterial:
            result = mMcCrossShader;
            break;
        case MaterialType::McBagMaterial:
            result = mMcBagShader;
            break;
        case MaterialType::McBagCubeMaterial:
            result = mMcBagCubeShader;
            break;
        case MaterialType::McCubeBreakMaterial:
            result = mMcCubeBreakShader;
            break;
        case MaterialType::McWaterSurfaceMaterial:
            result = mMcWaterSurfaceShader;
            break;
        default:
            std::cerr << "Unknown material type:"<< static_cast<int>(type) << std::endl;
            break;
    }
    return result;
}

void Renderer::setBackgroundColor(glm::vec3 color) {
    glClearColor(color.x, color.y, color.z, 1.0f);
}

void Renderer::projectObject(Object* object) {
    if (object->getType() == ObjectType::Mesh || object->getType() == ObjectType::InstancedMesh) {
        Mesh* mesh = (Mesh*)object;
        auto material = mesh->mMaterial;
        if (material->mBlendTest) {
            mTransparentMeshes.push_back(mesh);
        }else {
            mOpacityMeshes.push_back(mesh);
        }
    }
    auto children = object->getChildren();
    for (auto child : children) {
        projectObject(child);
    }
}