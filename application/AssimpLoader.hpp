#pragma once

#include "../glframework/Core.hpp"
#include "../glframework/tools/Tools.hpp"
#include "../glframework/mesh/Mesh.hpp"
#include "../glframework/material/PhongMaterial.hpp"
#include "../glframework/Texture.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"


#include "../glframework/Object.hpp"

class AssimpLoader {
public:
    static Object* load(const std::string& filePath);

    static void processNode(aiNode* ainode , Object* parent,const aiScene* scene,const std::string & rootPath);
    static glm::mat4 getMat4f(aiMatrix4x4 value);
    static Mesh* processMesh(aiMesh* aimesh, const aiScene* scene,const std::string & rootPath);
    static Texture* processTexture(const aiMaterial* aimat,const aiTextureType& type, const aiScene* scene, const std::string & rootPath);
    static std::string getTextureTypeName(aiTextureType type);
};

