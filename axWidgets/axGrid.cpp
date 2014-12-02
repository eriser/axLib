#include "axGrid.h"

axGrid::axGrid(axWindow* parent,
	const axRect& rect,
	const axGridEvents& events,
	const axGridInfo& info,
	axSize grid_dimension,
	//string label,
	axFlag flags,
	string msg) :
	// Heritage.
	axPanel(parent, rect),
	// Members.
	_events(events),
	_info(info),
	//_label(label),
	_flags(flags),
	_msg(msg),
	_dimension(grid_dimension),
	nGridElement(0),
	_currentElementColor(0.0, 0.8, 0.0)
{
	_gridElements.resize(_dimension.y);

	for(int j = 0; j < _dimension.y; j++)
	{
		_gridElements[j].resize(_dimension.x);
	}

	// _gridElements[4][3].on = true;
	// _gridElements[4][3].color = axColor(0.0, 0.4, 0.0);
	// _gridElements[4][3].position = axPoint((double(4) / _dimension.x) * rect.size.x,
	// 									   (double(3) / _dimension.y) * rect.size.y);

	_selectedElement(0, 0);
}

void axGrid::SetMsg(const string& msg)
{
	//_msg = msg;
}

void axGrid::SetSelected(const bool& selected)
{
	// if (_selected != selected)
	// {
	// 	_selected = selected;

	// 	if (_selected == true)
	// 	{
	// 		if (_currentColor == &_info.normal)
	// 		{
	// 			_currentColor = &_info.selected;
	// 			Update();
	// 		}
	// 	}
	// 	else
	// 	{
	// 		if (_currentColor == &_info.selected)
	// 		{
	// 			_currentColor = &_info.normal;
	// 			Update();
	// 		}
	// 	}
	// }
}

void axGrid::SetBackgroundAlpha(const float& alpha)
{
	// _bgAlpha = alpha;
	// Update();
}

void axGrid::SetElementColor(const axColor& color)
{
	_currentElementColor = color;
}

void axGrid::AddElement(const int& row, const int& col)
{
	axRect rect(GetRect());
	_gridElements[row][col].on = true;
	_gridElements[row][col].color = _currentElementColor;
	_gridElements[row][col].position = axPoint(floor(double(col) / _dimension.x * rect.size.x),
										   (double(row) / _dimension.y) * rect.size.y);
}
 
 axPoint axGrid::GetPositionOfElement(const axArrayIndex& index)
 {
 	axRect rect = GetRect();
 	return axPoint((double(index.x) / _dimension.x) * rect.size.x,
										   (double(index.y) / _dimension.y) * rect.size.y);
 }

 axPoint axGrid::GetElementIndexFromMouse(const axPoint& pos)
 {
 	axRect rect = GetRect();
 	return axSize(floor(((double)pos.x / rect.size.x)  * _dimension.x),
 				  floor(((double)pos.y / rect.size.y)  * _dimension.y));
 }

void axGrid::OnMouseLeftDown(const axPoint& pos)
{
	cout << "LEFT_DOWN" << endl;
	axRect rect = GetAbsoluteRect();
	axPoint position = pos - rect.position;
	axArrayIndex index = GetElementIndexFromMouse(position);

	cout << index.x << " " << index.y << endl;
	

	if(_gridElements[index.y][index.x].on == false)
	{
		AddElement(index.y, index.x);
	}
	_selectedElement = index;

	Update();
}

// void axGrid::OnMouseRightDown()
// {
	
// }

void axGrid::OnMouseLeftUp(const axPoint& pos)
{
}

void axGrid::OnMouseEnter()
{
}

void axGrid::OnMouseLeave()
{
}

void axGrid::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->SetColor(_info.normal, 1.0);
	gc->DrawRectangle(rect0);

	gc->SetColor(_info.contour, 1.0);

	int y = 0;
	glLineWidth(1.0);
	for(int j = 0; j <= _dimension.y; j++)
	{
		int y = (double(j) / _dimension.y) * rect.size.y;
		gc->DrawLine(axPoint(0, y), axPoint(rect.size.x, y));
	}

	int x = 0;
	for(int i = 0; i <= _dimension.x; i++)
	{
		int x = (double(i) / _dimension.x) * rect.size.x;
		gc->DrawLine(axPoint(x, 0), axPoint(x, rect.size.y));
	}

	axSize element_size(1.0 / _dimension.x * rect.size.x - 1, 
						1.0 / _dimension.y * rect.size.y - 1);
	for(int j = 0;j < _dimension.y; j++)
	{
		for(int i = 0; i < _dimension.x; i++)
		{
			if(_gridElements[j][i].on)
			{
				gc->SetColor(_gridElements[j][i].color);

				axSize elem_size(floor((double(i+1)/ _dimension.x * rect.size.x)) - 
								 floor((double(i) / _dimension.x * rect.size.x)) - 1,
								1.0 / _dimension.y * rect.size.y - 1);
				gc->DrawRectangle(axRect(_gridElements[j][i].position, elem_size));
			}
		}
	}



	//_selectedElement
	gc->SetColor(axColor(0.0, 0.0, 1.0));
	//glLineWidth(4.0);
	gc->DrawRectangleContour(axRect(GetPositionOfElement(_selectedElement), element_size), 2);

	// glLineWidth(1.0);

	// gc->DrawCircle(axPoint(50, 50), 10, 500);

}


