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
#ifndef __AX_WINDOW__
#define __AX_WINDOW__

/// @defgroup Core
/// @{

#include "axObject.h"
#include "axC++.h"
#include "axGC.h"
#include "axResourceManager.h"
#include "axConfig.h"
#include "axFrameBuffer.h"

class axApp;



class axWindow : public axObject
{
public:
	axWindow(axWindow* parent, const axRect& rect);

	axWindow* GetParent() const;

	axRect GetRect() const;
	axRect GetAbsoluteRect() const;

	void SetRect(const axRect& rect);
    
	axSize GetSize() const;
	void SetSize(const axSize& size);
	void SetPosition(const axPoint& pos);
    
    void SetShownRect(const axRect& size);
    axRect GetShownRect() const;
    
    axPoint GetBottomLeftPosition() const;
    axPoint GetTopRightPosition() const;
    
    axPoint GetNextPosRight(const int& delta) const;
    axPoint GetNextPosDown(const int& delta) const;
    axPoint GetNextPosLeft(const int& delta) const;
    axPoint GetNextPosUp(const int& delta) const;

	void SetScrollDecay(const axPoint& decay);
	axPoint GetScrollDecay() const;

    bool IsShown() const;
    void Show();
    void Hide();
    
	axGC* GetGC();

	void Reparent(axWindow* parent, const axPoint& position);

    bool GetIsPopup();
    
    void SetPopupWindow(const bool& popup);
    
    void SetSelectable(const bool& selectable);
    
    bool IsSelectable() const;
    
    bool IsBlockDrawing() const;
    
    void SetBlockDrawing(const bool& block);

    axResourceManager* GetResourceManager();
    
    bool IsEditingWidget() const;
    
    void SetEditingWidget(const bool& editing);
    
    void SetEditable(const bool& editable);
    
    bool IsEditable() const;
    
    void SetWindowColor(const axColor& color);
    
    void SetContourColor(const axColor& color);
    
    void RenderWindow();
    
    axRect GetDrawingRect() const;
    
    void SetNeedUpdate();
    
    // Drawing events.
    virtual void OnPaint();
    virtual void OnPaintStatic(){}
    virtual void Update() = 0;
    virtual void OnResize(){}
    
    // Mouse events.
    virtual void OnLeftDragging(){}
    virtual void OnRightDragging(){}
    virtual void OnMouseMotion(const axPoint& pos){}
    virtual void OnMouseLeftDown(const axPoint& pos){}
    virtual void OnMouseLeftDoubleClick(const axPoint& pos){}
    virtual void OnMouseLeftUp(const axPoint& pos){}
    virtual void OnMouseRightDown(const axPoint& pos){}
    virtual void OnMouseRightUp(){}
    virtual void OnMouseEnter(){}
    virtual void OnMouseLeave(){}
    virtual void OnFocusIn(){}
    virtual void OnMouseLeftDragging(const axPoint& pos){}
    
    // Keyboard events.
    virtual void OnKeyDown(const char& key){}
    virtual void OnBackSpaceDown(){}
    virtual void OnEnterDown(){}
    virtual void OnKeyDeleteDown(){}
    virtual void OnLeftArrowDown(){}
    virtual void OnRightArrowDown(){}
    virtual void OnWasKeyUnGrabbed(){}
    virtual void OnWasKeyGrabbed(){}

private:
	axWindow* _parent;
	axRect _rect, _shownRect;
	axPoint _absolutePosition, _scrollDecay;
    axColor _windowColor, _contourColor;
    axFrameBuffer _frameBufferObj;
    axResourceManager _resourceManager;
    
    /// @todo Remove this.
	axGC* _gc;

    bool _isPopup;
    bool _needUpdate;
    bool _isHidden;
    bool _isBlockDrawing;
    bool _isSelectable;
    bool _isEditingWidget;
    bool _isEditable;
};

/// @}
#endif //__AX_WINDOW__