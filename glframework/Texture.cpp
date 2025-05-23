#include "Texture.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "../application/stb_image.h"

std::map<std::string,Texture*> Texture::mTextureCache{};

Texture *Texture::createTexture(const std::string& path, unsigned int unit) {
    auto iter = mTextureCache.find(path);
    if (iter != mTextureCache.end()) {
        return iter->second;
    }
    auto texture = new Texture(path,unit);
    mTextureCache[path] = texture;
    return texture;
}

Texture *Texture::createTextureFromMemory(const std::string& path, unsigned int unit, unsigned char* dataIn, uint32_t widthIn, uint32_t heightIn) {
    //检查是否缓存过该路径的纹理对象
    auto iter = mTextureCache.find(path);
    if (iter != mTextureCache.end()) {
        return iter->second;
    }
    //如果没有生成 则重新生成
    auto texture = new Texture(unit,dataIn,widthIn,heightIn);
    mTextureCache[path] = texture;

    return texture;
}

Texture *Texture::createColorAttachment(unsigned int width, unsigned int height, unsigned int unit) {
    return new Texture(width,height,unit);
}

Texture *Texture::createDepthStencilAttachment(unsigned int width, unsigned int height, unsigned int unit) {
    Texture* depthStencilTexture = new Texture();

    unsigned int depthStencil;
    glGenTextures(1, &depthStencil);
    glBindTexture(GL_TEXTURE_2D, depthStencil);

    glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH24_STENCIL8,width,height,0,GL_DEPTH_STENCIL,GL_UNSIGNED_INT_24_8,NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_TEXTURE_2D,depthStencil,0);

    depthStencilTexture->mTexture = depthStencil;
    depthStencilTexture->mWidth = width;
    depthStencilTexture->mHeight = height;
    depthStencilTexture->mNum = unit;

    return depthStencilTexture;
}

Texture* Texture::createMcCubeTexture(const std::vector<std::string>& paths, unsigned int unit) {
    // 为缓存创建唯一键（将路径列表转换为唯一字符串）
    std::string cacheKey;
    for (const auto& path : paths) {
        cacheKey += path + "|";  // 用分隔符连接所有路径
    }

    // 检查缓存
    auto iter = mTextureCache.find(cacheKey);
    if (iter != mTextureCache.end()) {
        return iter->second;
    }

    // 创建新纹理数组
    Texture* texture = new Texture(true, paths, unit);  // isMc=true表示是纹理数组
    mTextureCache[cacheKey] = texture;
    return texture;
}

Texture::Texture() {

}

Texture::Texture(const std::string& filename, int numTexture) {
    mNum = numTexture;
    stbi_set_flip_vertically_on_load(1);

    mData = stbi_load(filename.c_str(), &mWidth, &mHeight, &mChannels, STBI_rgb_alpha);
    if (mData == nullptr) {
        printf("Failed to load image: %s\n", filename.c_str());
    }

    GLenum format =  3;
    if (mChannels == 1) {
        format = GL_RED;
    } else if (mChannels == 3) {
        format = GL_RGB;
    } else if (mChannels == 4) {
        format = GL_RGBA;
    }

    glGenTextures(1, &mTexture);
    glActiveTexture(GL_TEXTURE0 + mNum);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mData);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(mData);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture::Texture(unsigned int unit, unsigned char* dataIn, uint32_t widthIn, uint32_t heightIn) {
    std::cout << "Create texture by memory!" << std::endl;
    mNum = unit;

    stbi_set_flip_vertically_on_load(true);

    //Assimp: 计算图片大小 如果内嵌纹理是png jpg压缩格式 那么height = 0 width代表图片大小
    uint32_t dataInSize = 0;
    if (!heightIn) {
        dataInSize = widthIn;
    }else {
        //如果内嵌纹理不是png jpg压缩格式 默认rgba
        dataInSize = widthIn * heightIn * 4;
    }

    unsigned char* data = stbi_load_from_memory(dataIn,dataInSize,&mWidth,&mHeight,&mChannels,STBI_rgb_alpha);
    if (!data) {
        std::cout << "Failed to load texture by memory: " << dataIn << std::endl;
    }

    glGenTextures(1,&mTexture);
    glActiveTexture(GL_TEXTURE0 + mNum);
    glBindTexture(GL_TEXTURE_2D , mTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture::Texture(unsigned int width, unsigned int height, unsigned int unit) {
    mNum = unit;
    mWidth = width;
    mHeight = height;

    glGenTextures(1, &mTexture);
    glActiveTexture(GL_TEXTURE0 + mNum);
    glBindTexture(GL_TEXTURE_2D, mTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

//右左上下后前
Texture::Texture(const std::vector<std::string> &paths, unsigned int unit) {
    mTextureTarget = GL_TEXTURE_CUBE_MAP;
    stbi_set_flip_vertically_on_load(false);

    mNum = unit;
    //创建CubeMap对象
    glGenTextures(1,&mTexture);
    glActiveTexture(GL_TEXTURE0 + mNum);
    //GL_TEXTURE_2D变成GL_TEXTURE_CUBE_MAP
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);

    //循环读取六张贴图，并放置到cubemap的六个GPU空间内
    int channels;
    int width, height;
    for (unsigned int i = 0; i < paths.size(); ++i) {
        unsigned char* data = stbi_load(paths[i].c_str(),&width,&height,&channels,STBI_rgb_alpha);
        if (data != nullptr) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }else {
            std::cout << "Failed to load texture by path: " << paths[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glGenerateMipmap(mTextureTarget);
    //设置纹理参数
    glTexParameteri(mTextureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(mTextureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(mTextureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(mTextureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(mTextureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

Texture::Texture(bool isMc, const std::vector<std::string> &paths, unsigned int unit) {
    mTextureTarget = GL_TEXTURE_2D_ARRAY;  // 使用 Texture Array
    stbi_set_flip_vertically_on_load(true);  // 确保 UV 方向正确

    mNum = unit;
    glGenTextures(1, &mTexture);
    glActiveTexture(GL_TEXTURE0 + mNum);
    glBindTexture(GL_TEXTURE_2D_ARRAY, mTexture);

    // 先读取第一张图片获取宽高
    int width, height, channels;
    unsigned char* firstImage = stbi_load(paths[0].c_str(), &width, &height, &channels, STBI_rgb_alpha);
    if (!firstImage) {
        std::cerr << "Failed to load first texture: " << paths[0] << std::endl;
        return;
    }
    stbi_image_free(firstImage);

    // 分配 Texture Array 存储空间
    glTexImage3D(
        GL_TEXTURE_2D_ARRAY,
        0,                  // Mipmap 级别
        GL_RGBA,            // 内部格式
        width, height,
        paths.size(),       // 层数（6 层对应 6 个面）
        0,                  // 边界
        GL_RGBA,            // 格式
        GL_UNSIGNED_BYTE,   // 数据类型
        nullptr             // 先不传数据
    );

    // 逐个加载图片到 Texture Array
    for (unsigned int i = 0; i < paths.size(); i++) {
        unsigned char* data = stbi_load(paths[i].c_str(), &width, &height, &channels, STBI_rgb_alpha);
        if (data) {
            glTexSubImage3D(
                GL_TEXTURE_2D_ARRAY,
                0,          // Mipmap 级别
                0, 0, i,    // x, y, z 偏移
                width, height, 1,  // 每层宽高
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                data
            );
            stbi_image_free(data);
        } else {
            std::cerr << "Failed to load texture: " << paths[i] << std::endl;
        }
    }
    //glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    // 设置 Texture Array 参数
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // 关闭 Mipmap（像素风格不需要）
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 0);
}

Texture::~Texture() {
    if (!mTexture)
        glDeleteTextures(1, &mTexture);
}

void Texture::Bind() {
    glActiveTexture(GL_TEXTURE0 + mNum);
    glBindTexture(mTextureTarget, mTexture);
}

void Texture::setUnit(int unit) {
    mNum = unit;
}

GLuint Texture::getTexture() {
    return mTexture;
}