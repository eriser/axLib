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

class axApp;


class axWindow : public axObject
{
public:
	axWindow(axWindow* parent, const axRect& rect);

	axWindow* GetParent() const;

	//axID GetId() const;
	axRect GetRect() const;
	axRect GetAbsoluteRect() const;
	axSize GetSize() const;
	void SetSize(const axSize& size);
	void SetPosition(const axPoint& pos);

	void SetScrollDecay(const axPoint& decay);
	axPoint GetScrollDecay();

	axPoint GetBottomLeftPosition() const;
    axPoint GetTopRightPosition() const;
    axPoint GetNextPosRight(const int& delta) const;


	bool IsShown() const
	{
        const axWindow* win = this;
        
        while(win != nullptr)
        {
            if(win->_isHidden)
            {
                return false;
            }
            
            win = win->GetParent();
            
        }
        return true;
//		return !_isHidden;
        
	}

	void Show()
	{
		if (_isHidden != false)
		{
			_isHidden = false;
			Update();
		}
	}

	void Hide()
	{
		if (_isHidden != true)
		{
			_isHidden = true;
			Update();
		}
		
	}

//protected:
	friend class axManager;

    // Drawing events.
	virtual void OnPaint(){}
    virtual void Update() = 0;
    
    // Mouse events.
	virtual void OnLeftDragging(){}
	virtual void OnRightDragging(){}
	virtual void OnMouseMotion(const axPoint& pos){}
    virtual void OnMouseLeftDown(const axPoint& pos){}
    virtual void OnMouseLeftDoubleClick(const axPoint& pos){}
    virtual void OnMouseLeftUp(const axPoint& pos){}
	virtual void OnMouseRightDown(){}
	virtual void OnMouseRightUp(){}
	virtual void OnMouseEnter(){}
	virtual void OnMouseLeave(){}
	virtual void OnFocusIn(){}
	virtual void OnMouseLeftDragging(const axPoint& pos){}

    // Keyboard events.
    virtual void OnKeyDown(const char& key){}
    virtual void OnBackSpaceDown(){}
    virtual void OnKeyDeleteDown(){}
    virtual void OnLeftArrowDown(){}
    virtual void OnRightArrowDown(){}
    virtual void OnWasKeyUnGrabbed(){}
    virtual void OnWasKeyGrabbed(){}
    

    virtual void OnResize(){}
    
	axGC* GetGC();

	void Reparent(axWindow* parent, const axPoint& position);

	bool& GetIsPopup()
	{
		return _isPopup;
	}

protected:
	bool _isPopup;
    
private:
	axWindow* _parent;
	axRect _rect;
	axPoint _absolutePosition;
	axGC* _gc;
	bool _isHidden;
	axPoint _scrollDecay;
	
	GLuint _texture;
};

/// @}
#endif //__AX_WINDOW__