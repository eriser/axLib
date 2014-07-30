#include "axWindow.h"

axWindow::axWindow(axWindow* parent, const axRect& rect):
	//axObject(),
	// Members.
	_parent(parent),
	//_id(NEXT_ID),
	_rect(rect),
	_isHidden(false)
	// _scrollDecay(0, 0)
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

axPoint axWindow::GetScrollDecay()
{
	return _scrollDecay;
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

axRect axWindow::GetAbsoluteRect() const
{
	axPoint pos = _rect.position;
	const axWindow* win = this;

	// Too slow.
	while (win->GetParent() != nullptr)
	{
		pos += win->GetParent()->GetRect().position;
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
