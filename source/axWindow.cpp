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
_needUpdate(true),
_isEditingWidget(false),
_isEditable(true),
_frameBufferObj(rect.size)
{
	if (parent == nullptr)
	{
		_absolutePosition = rect.position;
	}
	else
	{
		_absolutePosition = parent->_absolutePosition + rect.position;
	}
    
	_gc = new axGC(this);
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

//    InitGLWindowBackBufferDrawing();
    _frameBufferObj.Resize(size);
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

void axWindow::SetPopupWindow(const bool& popup)
{
    _isPopup = popup;
}

void axWindow::SetWindowColor(const axColor& color)
{
    _windowColor = color;
}

void axWindow::SetContourColor(const axColor& color)
{
    _contourColor = color;
}

axRect axWindow::GetDrawingRect() const
{
    return axRect(0, 0, _rect.size.x - 1, _rect.size.y - 1);
}

void axWindow::SetNeedUpdate()
{
    _needUpdate = true;
}

bool axWindow::GetIsPopup()
{
    return _isPopup;
}

void axWindow::SetSelectable(const bool& selectable)
{
    _isSelectable = selectable;
}

bool axWindow::IsBlockDrawing() const
{
    return _isBlockDrawing;
}

void axWindow::SetBlockDrawing(const bool& block)
{
    _isBlockDrawing = block;
}

axResourceManager* axWindow::GetResourceManager()
{
    return &_resourceManager;
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

void axWindow::RenderWindow()
{
#if _axBackBufferWindow_ == 1
    if(_needUpdate)
    {
        std::function<void()> draw([this](){ OnPaint(); });
  
        _frameBufferObj.DrawOnFrameBuffer(draw, GetRect().size);
        _needUpdate = false;
    }
    
    _frameBufferObj.DrawFrameBuffer(GetShownRect().size);
    
//    if(_parent == nullptr)
//    {
////        axPrint("RenderWindow parent = nullptr");
//        _frameBufferObj.DrawFrameBuffer(GetShownRect().size);
//    }
//    else
//    {
//        _frameBufferObj.DrawFrameBufferOnParentFrameBuffer(GetShownRect().size);
//    }
    
#else
    OnPaint();
#endif //_axBackBufferWindow_
}