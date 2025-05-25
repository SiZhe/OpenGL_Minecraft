#pragma once
#include "../glframework/Core.hpp"

class Texture {
public:
    Texture();
    Texture(const std::string& filename,int numTexture);
    Texture(unsigned int unit, unsigned char* dataIn, uint32_t widthIn ,uint32_t heightIn);
    Texture(unsigned int width, unsigned int height,unsigned int unit);
    Texture(const std::vector<std::string>& paths,unsigned int unit);
    Texture(bool isMc, const std::vector<std::string>& paths,unsigned int unit);
    ~Texture();

    void Bind();

    void setUnit(int unit);

    GLuint getTexture();

    static Texture* createTexture(const std::string& path,unsigned int unit);
    static Texture* createTextureFromMemory(const std::string& path,unsigned int unit,unsigned char* dataIn, uint32_t widthIn ,uint32_t heightIn);
    static Texture* createColorAttachment(unsigned int width, unsigned int height,unsigned int unit);
    static Texture* createDepthStencilAttachment(unsigned int width, unsigned int height,unsigned int unit);
    static Texture* createMcCubeTexture(const std::vector<std::string>& paths,unsigned int unit);

private:
    GLuint mTexture;
    unsigned int mNum;
    int mWidth{0};
    int mHeight{0};
    int mChannels{0};
    unsigned char* mData{nullptr};

    static std::map<std::string, Texture*> mTextureCache;

    unsigned int mTextureTarget{GL_TEXTURE_2D};
};
