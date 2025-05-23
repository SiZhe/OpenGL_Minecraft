#include "AssimpLoader.hpp"

Object *AssimpLoader::load(const std::string& filePath) {

    //拿出模型根路径
    std::size_t lastIndex = filePath.find_last_of("//");
    auto rootPath = filePath.substr(0, lastIndex+1);

    Object* rootNode = new Object();

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filePath,aiProcess_Triangulate | aiProcess_GenNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << filePath << std::endl;
        return nullptr;
    }

    processNode(scene->mRootNode,rootNode,scene,rootPath);

    return rootNode;
}

void AssimpLoader::processNode(aiNode* ainode, Object* parent, const aiScene* scene, const std::string &rootPath) {
    Object* node = new Object();
    parent->addChild(node);

    glm::mat4 localMatrix = getMat4f(ainode->mTransformation);
    glm::vec3 position , eulerAngle , scale;
    Tools::decompose(localMatrix, position, eulerAngle, scale);

    node->setPosition(position);
    node->setAngleX(eulerAngle.x);
    node->setAngleY(eulerAngle.y);
    node->setAngleZ(eulerAngle.z);
    node->setScale(scale);

    for (unsigned int i = 0; i < ainode->mNumMeshes; i++) {
        int meshID = ainode->mMeshes[i];
        aiMesh* aimesh = scene->mMeshes[meshID];
        auto mesh = processMesh(aimesh,scene,rootPath);
        node->addChild(mesh);
    }

    for (unsigned int i = 0; i < ainode->mNumChildren; i++) {
        processNode(ainode->mChildren[i],node,scene,rootPath);
    }
}

Mesh *AssimpLoader::processMesh(aiMesh* aimesh, const aiScene* scene, const std::string &rootPath) {
    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<float> uvs;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < aimesh->mNumVertices; i++) {
        // 第i个顶的位置
        positions.push_back(aimesh->mVertices[i].x);
        positions.push_back(aimesh->mVertices[i].y);
        positions.push_back(aimesh->mVertices[i].z);

        //第i个顶点的法线
        normals.push_back(aimesh->mNormals[i].x);
        normals.push_back(aimesh->mNormals[i].y);
        normals.push_back(aimesh->mNormals[i].z);

        //第i个顶点的uv （关注0号uv，一般为贴图uv）
        if (aimesh->mTextureCoords[0]) {
            uvs.push_back(aimesh->mTextureCoords[0][i].x);
            uvs.push_back(aimesh->mTextureCoords[0][i].y);
        } else {
            uvs.push_back(0);
            uvs.push_back(0);
        }
    }

    for (unsigned int i = 0; i < aimesh->mNumFaces; i++) {
        aiFace face = aimesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    auto geometry = new Geometry(positions,normals,uvs,indices);
    auto material = new PhongMaterial();

    if (aimesh->mMaterialIndex >= 0) {
        Texture* texture{nullptr};
        aiMaterial* aiMat = scene->mMaterials[aimesh->mMaterialIndex];
        // 检查所有可能的纹理类型
        for (int type = aiTextureType_NONE; type <= aiTextureType_UNKNOWN; type++) {
            aiTextureType texType = static_cast<aiTextureType>(type);
            unsigned int textureCount = aiMat->GetTextureCount(texType);
            if (textureCount > 0) {
                std::cout << "Found " << textureCount << " textures of type: "
                          << getTextureTypeName(texType) << std::endl;
            }
        }

        //diffuse
        texture = processTexture(aiMat,aiTextureType_DIFFUSE,scene,rootPath);
        if (texture == nullptr) {
            std::cout << "diffuse is default" << std::endl;
            texture = Texture::createTexture("../assets/textures/defaultMask.png",0);
        }
        texture->setUnit(0);
        material->mDiffuse = texture;

        //specular
        auto specularMask = processTexture(aiMat,aiTextureType_SPECULAR,scene,rootPath);
        if (specularMask == nullptr) {
            std::cout << "specularMask is default" << std::endl;
            specularMask = Texture::createTexture("../assets/textures/defaultMask.png",0);
        }
        specularMask->setUnit(1);
        material->mSpecularMask = specularMask;
    }else {
        material->mDiffuse = new Texture("../assets/textures/defaultMask.png",0);
        material->mSpecularMask = new Texture("../assets/textures/defaultMask.png",1);
    }

    return new Mesh(geometry,material);
}

Texture *AssimpLoader::processTexture(const aiMaterial* aimat, const aiTextureType &type, const aiScene* scene, const std::string &rootPath) {
    Texture* texture{nullptr};

    //获取图片读取路径
    aiString aipath;
    aimat->Get(AI_MATKEY_TEXTURE(type,0),aipath);
    if (!aipath.length) {
        return nullptr;
    }

    //是否是嵌入fbx的图片
    const aiTexture* aitexture = scene->GetEmbeddedTexture(aipath.C_Str());
    if (aitexture) {
        std::cout << "Texture is embedded!!"  << std::endl;
        unsigned char* dataIn = reinterpret_cast<unsigned char*>(aitexture->pcData);
        uint32_t widthIn = aitexture->mWidth;//压缩格式，代表整张图片的大小
        uint32_t heightIn = aitexture->mHeight;
        texture = Texture::createTextureFromMemory(aipath.C_Str(),0,dataIn,widthIn,heightIn);
    }else {
        std::cout << "Texture is not embedded!!"  << std::endl;
        std::string fullPath = rootPath + aipath.C_Str();
        std::cout << "Full path: " << fullPath << std::endl;
        texture = Texture::createTexture(fullPath.c_str(),0);
    }
    return texture;
}

glm::mat4 AssimpLoader::getMat4f(aiMatrix4x4 value) {
    glm::mat4 to{
        value.a1,value.a2,value.a3,value.a4,
        value.b1,value.b2,value.b3,value.b4,
        value.c1,value.c2,value.c3,value.c4,
        value.d1,value.d2,value.d3,value.d4,
    };
    return to;
}

// 辅助函数，获取纹理类型名称
std::string AssimpLoader::getTextureTypeName(aiTextureType type) {
    switch(type) {
        case aiTextureType_NONE: return "NONE";
        case aiTextureType_DIFFUSE: return "DIFFUSE";
        case aiTextureType_SPECULAR: return "SPECULAR";
        case aiTextureType_AMBIENT: return "AMBIENT";
        case aiTextureType_EMISSIVE: return "EMISSIVE";
        case aiTextureType_HEIGHT: return "HEIGHT";
        case aiTextureType_NORMALS: return "NORMALS";
        case aiTextureType_SHININESS: return "SHININESS";
        case aiTextureType_OPACITY: return "OPACITY";
        case aiTextureType_DISPLACEMENT: return "DISPLACEMENT";
        case aiTextureType_LIGHTMAP: return "LIGHTMAP";
        case aiTextureType_REFLECTION: return "REFLECTION";
        case aiTextureType_BASE_COLOR: return "BASE_COLOR";
        case aiTextureType_NORMAL_CAMERA: return "NORMAL_CAMERA";
        case aiTextureType_EMISSION_COLOR: return "EMISSION_COLOR";
        case aiTextureType_METALNESS: return "METALNESS";
        case aiTextureType_DIFFUSE_ROUGHNESS: return "DIFFUSE_ROUGHNESS";
        case aiTextureType_AMBIENT_OCCLUSION: return "AMBIENT_OCCLUSION";
        default: return "UNKNOWN";
    }
}