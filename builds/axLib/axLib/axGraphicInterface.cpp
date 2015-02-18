//
//  axGraphicInterface.cpp
//  axLib
//
//  Created by Alexandre Arsenault on 2015-02-17.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "axGraphicInterface.h"
#include "axWindow.h"
#include "axApp.h"

void axGraphicInterface::DrawWindow(axWindow* win,
                                    bool& need_update,
                                    GLuint& frameBuffer)
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