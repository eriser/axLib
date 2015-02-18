//
//  axGraphicInterface.cpp
//  axLib
//
//  Created by Alexandre Arsenault on 2015-02-17.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "axGraphicInterface.h"
#include "axMath.h"
#include "axWindow.h"
#include "axApp.h"

namespace axGraphicInterface
{
    void DrawWindow(axWindow* win, bool& need_update, GLuint& frameBuffer)
    {
        if(need_update)
        {
#if _axBackBufferWindow_ == 1
            
            bool need_to_reactive_clip_test = false;
            if(glIsEnabled(GL_SCISSOR_TEST))
            {
                glDisable(GL_SCISSOR_TEST);
                need_to_reactive_clip_test = true;
            }
            
            
            // Save modelView matrix.
            glMatrixMode(GL_MODELVIEW);
            axMatrix4 modelView(GL_MODELVIEW);
            
            //        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            
            glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
            glPushAttrib(GL_DEPTH_BUFFER_BIT);
            glClearColor(0.0, 0.0, 0.0, 0.0);
            glClearDepth(1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            glBlendFuncSeparate(GL_SRC_ALPHA,
                                GL_ONE_MINUS_SRC_ALPHA,
                                GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            
            
            axRect rect(win->GetRect());
            glViewport(0, 0, rect.size.x, rect.size.y);
            
            glMatrixMode(GL_PROJECTION);
            axMatrix4 proj;
            
            glLoadIdentity();
            glOrtho(0.0, rect.size.x,
                    0.0, rect.size.y,
                    0.0, 1.0);
            
            glMatrixMode(GL_MODELVIEW);
            axMatrix4 mv_matrix;
            mv_matrix.Identity().Load();
            glTranslated(1.0, 1.0, 0.0);
            
#endif // _axBackBufferWindow_
            
            win->OnPaint();
            
#if _axBackBufferWindow_ == 1
            need_update = false;
            
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            
            axSize gSize(axApp::GetInstance()->GetCore()->GetGlobalSize());
            glViewport(0, 0, gSize.x, gSize.y);
            axOrtho2D(proj.Identity().GetData(), gSize);
            
            glMatrixMode(GL_MODELVIEW);
            modelView.Load();
            glPopAttrib();
            
            if(need_to_reactive_clip_test)
            {
                glEnable(GL_SCISSOR_TEST);
            }
            
#endif // _axBackBufferWindow_
        }
    }
    
    void Resize(const axSize& size)
    {
        glViewport(0, 0, size.x, size.y);
        
        glMatrixMode(GL_PROJECTION);
        
        axMatrix4 proj;
        axOrtho2D(proj.Identity().GetData(), size);
        
        // Select the modelview matrix.
        glMatrixMode(GL_MODELVIEW);
    }
    
    void Init()
    {
        // Enable Smooth Shading.
        glShadeModel(GL_SMOOTH);
        
        // Black Background
        glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
        
        // Depth Buffer Setup
        glClearDepth(1.0f);
        
        // Enables Depth Testing
        glEnable(GL_DEPTH_TEST);
        
        glEnable(GL_BLEND); // Enable blending.
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // The type of depth testing to do.
        glDepthFunc(GL_LEQUAL);
        
        // Really nice perspective calculations.
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    }
    
    void Draw(const axSize& size)
    {
        // Clear screen and depth buffer.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Set projection matrix.
        glMatrixMode(GL_PROJECTION);
        
        axMatrix4 proj;
        axOrtho2D(proj.Identity().GetData(), size);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        // Draw black rectangle.
        glColor4d(0.0, 0.0, 0.0, 1.0);
        
        // Draw black rectangle.
        axFloatRect rect(-1.0, -1.0, size.x * 2.0, size.y * 2.0);
        axRectFloatPoints points = rect.GetPoints();
        GLubyte indices[] = {0, 1,2, 2,3};
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_DOUBLE, 0, &points);
        glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_BYTE, indices);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    
    void InitGLWindowBackBuffer(GLuint& frameBuffer,
                                GLuint& frameBufferTexture,
                                const axSize& size)
    {
        // Create framebuffer object (FBO).
        glGenFramebuffers(1, &frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        
        // Create texture.
        glGenTextures(1, &frameBufferTexture);
        glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
        
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
                               frameBufferTexture,
                               0);
        
        // Does the GPU support current FBO configuration.
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER_EXT);
        
        switch(status)
        {
            case GL_FRAMEBUFFER_COMPLETE_EXT: break;
                
            default:
                std::cerr << "ERROR GEN FRAME BUFFER : " << status << std::endl;
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}