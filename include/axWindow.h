/// @defgroup Core 
/// @{

#ifndef __AX_WINDOW__
#define __AX_WINDOW__

#include "axObject.h"
//#include "axID.h"
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

	// virtual axApp* GetApp() = 0;

	axPoint GetBottomLeftPosition() const;
    axPoint GetTopRightPosition() const;
    axPoint GetNextPosRight(const int& delta) const;


	bool IsShown() const
	{
		return !_isHidden;
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

	virtual void OnPaint(){}
	virtual void OnLeftDragging(){}
	virtual void OnRightDragging(){}
	virtual void OnMouseMotion(const axPoint& pos){}

	virtual void OnMouseLeftDown(const axPoint& pos)
	{

	}

	virtual void OnMouseLeftUp(const axPoint& pos)
	{

	}

	virtual void OnMouseRightDown(){}
	virtual void OnMouseRightUp(){}
	virtual void OnMouseEnter(){}
	virtual void OnMouseLeave(){}
	virtual void OnFocusIn(){}
	virtual void Update() = 0;
	virtual void OnMouseLeftDragging(const axPoint& pos){}
	//virtual void Update(){}

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

#endif //__AX_WINDOW__

/// @}