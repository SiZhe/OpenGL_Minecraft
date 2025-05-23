#include "FrameBuffer.hpp"

FrameBuffer::FrameBuffer(unsigned int width, unsigned int height)
    : mWidth(width), mHeight(height){
    initFramebuffer();
}

void FrameBuffer::initFramebuffer() {
    glGenFramebuffers(1, &mFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, mFbo);

    mColorAttachment = Texture::createColorAttachment(mWidth,mHeight,0);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,mColorAttachment->getTexture(),0);

    mDepthStencilAttachment = Texture::createDepthStencilAttachment(mWidth,mHeight,0);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_TEXTURE_2D,mDepthStencilAttachment->getTexture(),0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer() {
    cleanUp();
}

void FrameBuffer::cleanUp() {
    if (mFbo) {
        glDeleteFramebuffers(1, &mFbo);
    }
    if (mColorAttachment) {
        delete mColorAttachment;
    }
    if (mDepthStencilAttachment) {
        delete mDepthStencilAttachment;
    }
}

void FrameBuffer::resize(unsigned int width, unsigned int height) {

    if (width == mWidth && height == mHeight) {
        return; // 尺寸未改变，无需处理
    }

    std::cout << "FrameBuffer::resize from (" << mWidth << "x" << mHeight
              << ") to (" << width << "x" << height << ")" << std::endl;

    // 清理旧资源
    cleanUp();

    // 更新尺寸
    mWidth = width;
    mHeight = height;

    // 重新初始化
    initFramebuffer();
}
