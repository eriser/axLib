//
//  axGraphicInterface.h
//  axLib
//
//  Created by Alexandre Arsenault on 2015-02-17.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __axLib__axGraphicInterface__
#define __axLib__axGraphicInterface__

#include "axC++.h"

class axWindow;

namespace axGraphicInterface
{

    void Resize(const axSize& size);
    
    void Init();
    
    void Draw(const axSize& size);
    
    void InitGLWindowBackBuffer(GLuint& frameBuffer,
                                GLuint& frameBufferTexture,
                                const axSize& size);
    
    void DrawWindow(axWindow* win, bool& need_update, GLuint& frameBuffer);
}


class axFrameBuffer
{
public:
    axFrameBuffer(const axSize& size)
    {
        Init(size);
    }
    
    void Init(const axSize& size)
    {
        glGenFramebuffers(1, &_frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
        
        // Create texture.
        glGenTextures(1, &_frameBufferTexture);
        glBindTexture(GL_TEXTURE_2D, _frameBufferTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        
        // NULL means reserve texture memory, but texels are undefined.
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGBA8,
                     size.x,
                     size.y,
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     NULL);
        
        // Attach 2D texture to this FBO.
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D,
                               _frameBufferTexture,
                               0);
        
        // Does the GPU support current FBO configuration.
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER_EXT);
        
        switch(status)
        {
            case GL_FRAMEBUFFER_COMPLETE_EXT:
                break;
            default:
                std::cerr << "ERROR GEN FRAME BUFFER : " << status << std::endl;
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    void Resize(const axSize& size)
    {
        Init(size);
    }
    
    void DrawOnFrameBuffer(const std::function<void()>& fct,
                           const axSize& size);
    
    void DrawFrameBuffer(const axSize& shownSize);
    
private:
    GLuint _frameBuffer, _frameBufferTexture, _depthBuffer;
};

#endif /* defined(__axLib__axGraphicInterface__) */
