#pragma once
#include "../Core.hpp"
#include "../Texture.hpp"

class FrameBuffer {
public:
    FrameBuffer(unsigned int width,unsigned int height);
    ~FrameBuffer();

    void initFramebuffer();
    void cleanUp();
    void resize(unsigned int width,unsigned int height);

    unsigned int mWidth{0};
    unsigned int mHeight{0};

    unsigned int mFbo{0};
    Texture* mColorAttachment{nullptr};
    Texture* mDepthStencilAttachment{nullptr};
};

