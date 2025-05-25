#pragma once
#include <vector>
#include <memory>
#include "../Core.hpp"
#include "../mesh/Mesh.hpp"
#include "../mesh/InstancedMesh.hpp"
#include "../../application/camera/camera.hpp"
#include "../light/DirectionLight.hpp"
#include "../light/PointLight.hpp"
#include "../light/SpotLight.hpp"
#include "../light/AmbientLight.hpp"
#include "../Shader.hpp"
#include "../Scene.hpp"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void renderObject(
            Object* object,
            Camera* camera,
            DirectionLight* directionLight,
            const std::vector<std::shared_ptr<PointLight>>& pointLights,
            SpotLight* spotLight,
            AmbientLight* ambientLight
            );

    //渲染函数 每次调用都会渲染一帧
    void render(
        //const std::vector<Mesh*>& meshes,
        Scene* scene,
        Camera* camera,
        DirectionLight* dirLight,
        const std::vector<std::shared_ptr<PointLight>>& pointLights,
        SpotLight* spotLight,
        AmbientLight* ambientLight,
        unsigned int fbo = 0
        );

    void setDepthState(Material* material);
    void setPolygonOffsetState(Material* material);
    void setStencilState(Material* material);
    void setBlendState(Material* material);
    void setFaceCullState(Material* material);

    void setBackgroundColor(glm::vec3 color);

    Material* mGlobalMaterial{nullptr};
private:
    //生成多种不同的shader
    Shader* mPhongShader {nullptr};
    Shader* mWhiteShader {nullptr};
    Shader* mDepthShader {nullptr};
    Shader* mOpacityMaskShader {nullptr};
    Shader* mScreenShader {nullptr};
    Shader* mCubeShader {nullptr};
    Shader* mEnvShader {nullptr};
    Shader* mSphereShader {nullptr};

    //Mc的Shader
    Shader* mMcCubeShader {nullptr};
    Shader* mMcPlantShader {nullptr};
    Shader* mMcTakeCubeShader {nullptr};
    Shader* mMcEliminateShader {nullptr};
    Shader* mMcDropShader {nullptr};
    Shader* mMcDropStencilShader {nullptr};
    Shader* mMcCrossShader {nullptr};
    Shader* mMcBagShader {nullptr};
    Shader* mMcBagCubeShader {nullptr};
    Shader* mMcCubeBreakShader {nullptr};
    Shader* mMcWaterSurfaceShader {nullptr};

    Shader* pickShader(MaterialType type);

    //不透明物体与透明物体的队列
    std::vector<Mesh*> mOpacityMeshes{};
    std::vector<Mesh*> mTransparentMeshes{};
    void projectObject(Object* object);
};