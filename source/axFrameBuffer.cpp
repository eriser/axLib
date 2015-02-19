/*******************************************************************************
 * Copyright (c) 2015 Alexandre Arsenault.
 *
 * This file is part of axLibrary.
 *
 * axLibrary is free or commercial software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 or any later version of the
 * License or use a commercial axLibrary License.
 *
 * axLibrary is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with axLibrary. If not, see <http://www.gnu.org/licenses/>.
 *
 * To release a closed-source product which uses axLibrary, commercial
 * licenses are available, email alx.arsenault@gmail.com for more information.
 ******************************************************************************/

#include "axFrameBuffer.h"
#include "axMath.h"
#include "axWindow.h"
#include "axApp.h"
#include "axConfig.h"

/*******************************************************************************
 * axFrameBuffer.
 ******************************************************************************/
axFrameBuffer::axFrameBuffer(const axSize& size)
{
#if _axBackBufferWindow_ == 1
    Init(size);
#endif // _axBackBufferWindow_
}

void axFrameBuffer::Init(const axSize& size)
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
    
    if(status != GL_FRAMEBUFFER_COMPLETE_EXT)
    {
        axError("Generating frame buffer : ", status);
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void axFrameBuffer::Resize(const axSize& size)
{
#if _axBackBufferWindow_ == 1
    Init(size);
#endif //_axBackBufferWindow_ == 1
}

void axFrameBuffer::DrawOnFrameBuffer(const std::function<void()>& fct,
                                      const axSize& size)
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
    
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
    glPushAttrib(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    DrawingOnFrameBufferBlendFunction();
    
    glViewport(0, 0, size.x, size.y);
    
    glMatrixMode(GL_PROJECTION);
    
    // Using axMath for projection flip every upside down.
    axMatrix4 proj;
    
    /// @todo Use axMath for projection.
    glLoadIdentity();
    glOrtho(0.0, size.x,
            0.0, size.y,
            0.0, 1.0);
    
    glMatrixMode(GL_MODELVIEW);
    axMatrix4 mv_matrix;
    mv_matrix.Identity().Load();

    if(fct)
    {
        fct();
    }
    
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
#endif //_axBackBufferWindow_
}

void axFrameBuffer::DrawFrameBuffer(const axSize& shownSize)
{
#if _axBackBufferWindow_ == 1
    glEnable(GL_TEXTURE_2D);
    
    DrawingFrameBufferBlendFunction();

    axFloatPoint pos(0.0, 0.0);
    axFloatSize size(shownSize.x, shownSize.y);
    
    glBindTexture(GL_TEXTURE_2D, _frameBufferTexture);
    
    glBegin(GL_QUADS);
    
    // Bottom left.
    glTexCoord2d(0.0, 0.0);
    glVertex2d(pos.x, pos.y);
    
    // Top left.
    glTexCoord2d(0.0, 1.0);
    glVertex2d(pos.x, pos.y + size.y);
    
    // Top right.
    glTexCoord2d(1.0, 1.0);
    glVertex2d(pos.x + size.x, pos.y + size.y);
    
    // Buttom right.
    glTexCoord2d(1.0, 0.0);
    glVertex2d(pos.x + size.x, pos.y);
    
    glEnd();
    
    //	glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    
#endif //_axBackBufferWindow_
}

void axFrameBuffer::DrawingOnFrameBufferBlendFunction()
{
    // All other blend function doesn't work except this one when drawing
    // multiple transparent frame buffer.
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
                        GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}

void axFrameBuffer::DrawingFrameBufferBlendFunction()
{
    // This seem to be the only way to make the alpha and color blend properly.
    glBlendFuncSeparate(GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
                        GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}