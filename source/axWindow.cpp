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
    
//    _isHidden = true;
//    Update();
}

void axWindow::Reparent(axWindow* parent, const axPoint& position)
{
	_parent = parent;
	SetPosition(position);

	axID temp = _parent->GetId();
	_parent->SetIdForReparenting(GetId());
	SetIdForReparenting(temp);

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

// axID axWindow::GetId() const
// {
// 	return _id;
// }

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
	
	// Constant.
	//return axRect(_absolutePosition, _rect.size);
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
    
    // NULL means reserve texture memory, but texels are undefined.
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
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
    
    
    glGenRenderbuffers(1, &_depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER,
                          GL_DEPTH_COMPONENT32,
                          _rect.size.x,
                          _rect.size.y);

    // Does the GPU support current FBO configuration.
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER_EXT);

    switch(status)
    {
        case GL_FRAMEBUFFER_COMPLETE_EXT:
            cout<<"good";
            break;
        default:
            std::cerr << "ERROR GEN FRAME BUFFER" << std::endl;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void axWindow::RenderWindow()
{
    if(_needUpdate)
    {
//        // Save modelView matrix.
//        glMatrixMode(GL_MODELVIEW);
//        axMatrix4 modelView(GL_MODELVIEW);
//        
//        glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
//        glClearColor(0.0, 0.0, 0.0, 0.0);
//        glClearDepth(1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        
//        glViewport(0, 0, GetRect().size.x, GetRect().size.y);
//        
//        glMatrixMode(GL_PROJECTION);
//        axMatrix4 proj;
////        axOrtho2D(proj.Identity().GetData(), GetRect().size);
//        
//        glLoadIdentity();
//        glOrtho(0.0, GetRect().size.x,
//                0.0, GetRect().size.y,
//                0.0, 1.0);
//        
//        glMatrixMode(GL_MODELVIEW);
//        axMatrix4 mv_matrix;
//        mv_matrix.Identity().Load();
//        
        OnPaint();
        
//        _needUpdate = false;
//
//        glBindFramebuffer(GL_FRAMEBUFFER, 0);
//        
//        axSize gSize(axApp::GetInstance()->GetCore()->GetGlobalSize());
//        glViewport(0, 0, gSize.x, gSize.y);
//        axOrtho2D(proj.Identity().GetData(), gSize);
//        
//        glMatrixMode(GL_MODELVIEW);
//        modelView.Load();
//        
//        axGC* gc = GetGC();
//        gc->DrawWindowBuffer();
    }
    else
    {
        axGC* gc = GetGC();
        gc->DrawWindowBuffer();
    }
}