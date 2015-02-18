/*******************************************************************************
 * Copyright (c) 2013 Alexandre Arsenault.
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
#include "axWindow.h"
#include "axApp.h"
#include "axMath.h"
#include "axConfig.h"
#include "axGraphicInterface.h"

axWindow::axWindow(axWindow* parent, const axRect& rect):
// Members.
_parent(parent),
_rect(rect),
_isHidden(false),
_isPopup(false),
_isBlockDrawing(false),
_shownRect(axPoint(0, 0), rect.size),
_isSelectable(true),
_windowColor(0.0, 0.0, 0.0, 0.0),
_contourColor(0.0, 0.0, 0.0, 0.0),
_needUpdate(true)
{
//#ifdef _axDebugEditor_
    _isEditingWidget = false;
    _isEditable = true;
//#endif // _axDebugEditor_
    
	if (parent == nullptr)
	{
		_absolutePosition = rect.position;
	}
	else
	{
		_absolutePosition = parent->_absolutePosition + rect.position;
	}
    
	_gc = new axGC(this);
    
    InitGLWindowBackBufferDrawing();
}

axWindow* axWindow::GetParent() const
{
	return _parent;
}

void axWindow::SetScrollDecay(const axPoint& decay)
{
	_scrollDecay = decay;
}

axPoint axWindow::GetScrollDecay() const
{
	return _scrollDecay;
}

bool axWindow::IsShown() const
{
    const axWindow* win = this;
    
    while(win != nullptr)
    {
        if(win->_isHidden == true)
        {
            return false;
        }
        
        win = win->GetParent();
        
    }
    return true;
    //		return !_isHidden;
}

void axWindow::Show()
{
    if (_isHidden != false)
    {
        _isHidden = false;
        Update();
    }
}

void axWindow::Hide()
{
    if (_isHidden != true)
    {
        _isHidden = true;
        Update();
    }
}

void axWindow::Reparent(axWindow* parent, const axPoint& position)
{
	_parent = parent;
	SetPosition(position);

	axID temp = _parent->GetId();
	_parent->ChangeId(GetId());
	ChangeId(temp);

	Update();
}

axRect axWindow::GetShownRect() const
{
    return _shownRect;
}

void axWindow::SetShownRect(const axRect& rect)
{
    _shownRect = rect;
}

axRect axWindow::GetRect() const
{
	return _rect;
}

axPoint axWindow::GetBottomLeftPosition() const
{
	return axPoint(_rect.position.x, 
		_rect.position.y + _rect.size.y);
}

axPoint axWindow::GetTopRightPosition() const
{
    return axPoint(_rect.position.x + _rect.size.x, _rect.position.y);
}

axPoint axWindow::GetNextPosRight(const int& delta) const
{
    return axPoint(_rect.position.x + _rect.size.x + delta, _rect.position.y);
}

axPoint axWindow::GetNextPosDown(const int& delta) const
{
    return axPoint(_rect.position.x, _rect.position.y + _rect.size.y + delta);
}

axPoint axWindow::GetNextPosLeft(const int& delta) const
{
     return axPoint(_rect.position.x, _rect.position.y - delta);
}

axPoint axWindow::GetNextPosUp(const int& delta) const
{
    return axPoint(_rect.position.x, _rect.position.y - delta);
}


axRect axWindow::GetAbsoluteRect() const
{
	axPoint pos = _rect.position;
	const axWindow* win = this;

	// Too slow.
	while (win->GetParent() != nullptr)
	{
		pos += win->GetParent()->GetRect().position;
        pos -= win->GetParent()->GetScrollDecay();
		win = win->GetParent();
	}

	return axRect(pos, _rect.size);
}

axSize axWindow::GetSize() const
{
	return _rect.size;
}

axGC* axWindow::GetGC()
{
	return _gc;
}

void axWindow::SetSize(const axSize& size)
{
	_rect.size = size;
    _shownRect.size = size;

    InitGLWindowBackBufferDrawing();
    Update();
}

void axWindow::SetPosition(const axPoint& pos)
{
	if (_parent == nullptr)
	{
		_absolutePosition = pos;
	}
	else
	{
		_absolutePosition = _parent->GetAbsoluteRect().position + pos;
	}

	_rect.position = pos;

	Update();
}

bool axWindow::IsSelectable() const
{
    if(IsEditingWidget() &&
       axApp::GetInstance()->IsDebugEditorActive() == false)
    {
        return false;
    }
    
    return _isSelectable;
}

bool axWindow::IsEditingWidget() const
{
    return _isEditingWidget;
}

void axWindow::SetEditingWidget(const bool& editing)
{
    _isEditingWidget = editing;
}

void axWindow::SetEditable(const bool& editable)
{
    _isEditable = editable;
}

bool axWindow::IsEditable() const
{
    return _isEditable;
}

void axWindow::SetWindowColor(const axColor& color)
{
    _windowColor = color;
}

void axWindow::SetContourColor(const axColor& color)
{
    _contourColor = color;
}

void axWindow::OnPaint()
{
    
    axGC* gc = GetGC();
    axRect rect(GetRect());
    
    gc->SetColor(_windowColor);
    gc->DrawRectangle(axRect(axPoint(0, 0), rect.size));
    
    gc->SetColor(_contourColor);
    gc->DrawRectangle(axRect(axPoint(0, 0), rect.size));
}

// https://www.opengl.org/wiki/Framebuffer_Object_Examples
void axWindow::InitGLWindowBackBufferDrawing()
{
    
    // Create framebuffer object (FBO).
    glGenFramebuffers(1, &_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
    
    // Create texture.
    glGenTextures(1, &_frameBufferTexture);
    glBindTexture(GL_TEXTURE_2D, _frameBufferTexture);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    // NULL means reserve texture memory, but texels are undefined.
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA8,
                 _rect.size.x,
                 _rect.size.y,
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
    
    
//    glGenRenderbuffers(1, &_depthBuffer);
//    glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
//    glRenderbufferStorage(GL_RENDERBUFFER,
//                          GL_DEPTH_COMPONENT32,
//                          _rect.size.x,
//                          _rect.size.y);

    // Does the GPU support current FBO configuration.
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER_EXT);

    switch(status)
    {
        case GL_FRAMEBUFFER_COMPLETE_EXT: break;
            
        default:
            std::cerr << "ERROR GEN FRAME BUFFER : " << status << std::endl;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


//void WinEndDrawing(axWindow* win)
//{
//    if(win->IsBlockDrawing())
//    {
//        glDisable(GL_SCISSOR_TEST);
//    }
//}
//
//void WinBeforeDrawing(axWindow* win)
//{
//    if(win->IsBlockDrawing())
//    {
//        axMatrix4 mview;
//        mview.Identity().Load();
//        
//        axRect abs_rect = win->GetAbsoluteRect();
//        axRect shown_rect = win->GetShownRect();
//        
//        double delta_size_x = shown_rect.size.x - abs_rect.size.x;
//        double delta_size_y = shown_rect.size.y - abs_rect.size.y;
//        
//        double globalY = axApp::GetInstance()->GetCore()->GetGlobalSize().y;
//        double sumY = (abs_rect.position.y + shown_rect.position.y +
//                       abs_rect.size.y + delta_size_y);
//        
//        glScissor(abs_rect.position.x + shown_rect.position.x - 1,
//                  globalY - sumY,
//                  shown_rect.size.x + 1,
//                  shown_rect.size.y + 1);
//        
//        
//        std::cout << "SHOUWN  " << shown_rect.size.x << std::endl;
//        glEnable(GL_SCISSOR_TEST);
//    }
//}

void axWindow::RenderWindow()
{
    // Check the renderer limits. For example, you might want to call the OpenGL
    // function glGetIntegerv to check the maximum texture size
    // (GL_MAX_TEXTURE_SIZE) or find out the maximum number of color buffers
    // you can attach to the framebuffer object(GL_MAX_COLOR_ATTACHMENTS_EXT).
    
    int v = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &v);
    std::cout << "Nb max :::::: " << v << std::endl;
    
    
    if(_needUpdate)
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
        
            glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
            glPushAttrib(GL_DEPTH_BUFFER_BIT);
            glClearColor(0.0, 0.0, 0.0, 0.0);
            glClearDepth(1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glBlendFuncSeparate(GL_SRC_ALPHA,
                                GL_ONE_MINUS_SRC_ALPHA,
                                GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        
            glViewport(0, 0, GetRect().size.x, GetRect().size.y);
        
            glMatrixMode(GL_PROJECTION);
            axMatrix4 proj;
//        axOrtho2D(proj.Identity().GetData(), GetRect().size);
        
            glLoadIdentity();
            glOrtho(0.0, GetRect().size.x,
                    0.0, GetRect().size.y,
                    0.0, 1.0);
        
            glMatrixMode(GL_MODELVIEW);
            axMatrix4 mv_matrix;
            mv_matrix.Identity().Load();
            glTranslated(1.0, 1.0, 0.0);
        #endif // _axBackBufferWindow_
        
//        glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
//        glDisable(GL_BLEND);
        
//        glBlendFunc(<#GLenum sfactor#>, <#GLenum dfactor#>);
        OnPaint();
//        glEnable(GL_BLEND);
        
        #if _axBackBufferWindow_ == 1
            _needUpdate = false;

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
            axSize gSize(axApp::GetInstance()->GetCore()->GetGlobalSize());
            glViewport(0, 0, gSize.x, gSize.y);
            axOrtho2D(proj.Identity().GetData(), gSize);
        
            glMatrixMode(GL_MODELVIEW);
            modelView.Load();
            glPopAttrib();
        
        
        
            //axGC* gc = GetGC();
        
//        BeforeDrawing(this);
            //gc->DrawWindowBuffer();
//        EndDrawing(this);
        if(need_to_reactive_clip_test)
            glEnable(GL_SCISSOR_TEST);
        
        #endif // _axBackBufferWindow_
        
    }
    
    
    //else
    //{
//        BeforeDrawing(this);
        axGC* gc = GetGC();
        gc->DrawWindowBuffer();
//        EndDrawing(this);
    //}
}