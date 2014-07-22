#include "axSlider.h"

axSlider::axSlider(axApp* app,
	axWindow* parent,
	const axRect& rect,
	const axSliderEvents& events,
	const axSliderInfo& info,
	//string img_path,
	//string label,
	axFlag flags) :
	// Heritage.
	axPanel(app, parent, rect),
	// Members.
	_events(events),
	_info(info),
	_currentBgColor(info.bgColorNormal),
	_currentSliderColor(info.sliderColorNormal),
	_btnImg(info.img_path.c_str()),
	_nCurrentImg(axBTN_NORMAL),
	_delta_click(0),
	_sliderValue(0.0),
	_flags(flags),
	_bg_alpha(1.0)
{
	_sliderPosition = 0;

	if (axFlag_exist(axSLIDER_FLAG_VERTICAL, _flags))
	{
		_sliderYPos = (GetSize().x - _info.slider_width) * 0.5;
		_btnYPos = (GetSize().x - _info.btn_size.x) * 0.5;

		if (axFlag_exist(axSLIDER_FLAG_RIGHT_ALIGN, _flags))
		{
			_sliderPosition = GetSize().y - _info.btn_size.y;
		}
	}
	else
	{
		_sliderYPos = (GetSize().y - _info.slider_width) * 0.5;
		_btnYPos = (GetSize().y - _info.btn_size.y) * 0.5;

		if (axFlag_exist(axSLIDER_FLAG_RIGHT_ALIGN, _flags))
		{
			_sliderPosition = GetSize().x - _info.btn_size.x;
		}
	}
	//SendPaintEvent();
	Update();
}

void axSlider::SetValue(const double& value)
{
	_sliderValue = value;

	if (axFlag_exist(axSLIDER_FLAG_VERTICAL, _flags))
	{
		_sliderPosition = double(GetSize().y) * (1.0 - _sliderValue);// - _info.btn_size.y - 2) + 1;
	}
	// else
	// {
	// 	_sliderValue = (_sliderPosition - 1) /
	// 		double(GetSize().x - _info.btn_size.x - 2);
	// }
	Update();	
}

void axSlider::OnMouseLeftDown(const axPoint& mousePos)
{
	axPoint pos = mousePos - GetAbsoluteRect().position;

	axRect sliderBtnRect;
	if (axFlag_exist(axSLIDER_FLAG_VERTICAL, _flags))
	{
		sliderBtnRect = axRect(axPoint(_btnYPos, _sliderPosition),
			_info.btn_size);
	}
	else
	{
		sliderBtnRect = axRect(axPoint(_sliderPosition, _btnYPos),
			_info.btn_size);
	}


	if (axFlag_exist(axSLIDER_FLAG_CLICK_ANYWHERE, _flags))
	{
		_nCurrentImg = axBTN_DOWN;
		_currentSliderColor = _info.sliderColorClicked;
		
		if (axFlag_exist(axSLIDER_FLAG_VERTICAL, _flags))//IsMouseHoverRect( sliderBtnRect ) && m_nCurrentImg != axBTN_DOWN )
		{
			_delta_click = -_info.btn_size.y * 0.5;
		}
		else
		{
			_delta_click = -_info.btn_size.x * 0.5;
		}

		blockSliderPosition(pos);
		GrabMouse();
		//SendPaintEvent();

		if (_events.slider_value_change)
		{
			_events.slider_value_change(axSliderMsg(_sliderValue));
		}
		Update();
	}
	// Click on boutton to move.
	else
	{

		cout << "ELSE" << endl;
		if (sliderBtnRect.IsPointInside(pos) && _nCurrentImg != axBTN_DOWN)
		{
			cout << "ELSE IN" << endl;
			_nCurrentImg = axBTN_DOWN;
			_currentSliderColor = _info.sliderColorClicked;

			if (axFlag_exist(axSLIDER_FLAG_VERTICAL, _flags))
			{
				_delta_click = sliderBtnRect.position.y - pos.y;
			}
			else
			{
				_delta_click = sliderBtnRect.position.x - pos.x;
			}

			cout << "MOUSE GRAB before" << endl;
			GrabMouse();

			cout << "MOUSE GRAB" << endl;

			// Send value change event.
			if (_events.slider_value_change)
			{
				_events.slider_value_change(axSliderMsg(_sliderValue));
			}
			Update();
		}
	}
}

void axSlider::OnMouseLeftUp(const axPoint& p)
{
	
	axPoint pos = p - GetAbsoluteRect().position;

	if (IsGrabbed())
	{
		UnGrabMouse();
		blockSliderPosition(pos);
		_nCurrentImg = axBTN_NORMAL;
		_currentSliderColor = _info.sliderColorNormal;

		Update();
	}
}

void axSlider::OnMouseLeftDragging(const axPoint& p)
{
	cout << "Drag" << endl;
	//DSTREAM << "DRAG" << endl;
	axPoint pos = p - GetAbsoluteRect().position;

	blockSliderPosition(pos);

	if (_events.slider_value_change)
	{
		_events.slider_value_change(axSliderMsg(_sliderValue));
	}
	
	Update();
}

void axSlider::blockSliderPosition(const axPoint& pos)
{
	//axPoint pos = p - GetRect().position;

	if (axFlag_exist(axSLIDER_FLAG_VERTICAL, _flags))
	{
		int pos_y = pos.y + _delta_click;

		axCLIP(pos_y, 1, GetSize().y - _info.btn_size.y - 1);

		_sliderPosition = pos_y;
	}
	else
	{
		int pos_x = pos.x + _delta_click;

		axCLIP(pos_x, 1, GetSize().x - _info.btn_size.x - 1);

		_sliderPosition = pos_x;
	}

	updateSliderValue();
}

void axSlider::updateSliderValue()
{
	if (axFlag_exist(axSLIDER_FLAG_VERTICAL, _flags))
	{
		_sliderValue = (_sliderPosition - 1) /
			double(GetSize().y - _info.btn_size.y - 2);
	}
	else
	{
		_sliderValue = (_sliderPosition - 1) /
			double(GetSize().x - _info.btn_size.x - 2);
	}
}

void axSlider::OnMouseMotion(const axPoint& p)
{
	axPoint pos = p - GetAbsoluteRect().position;

	axRect sliderBtnRect;
	if (axFlag_exist(axSLIDER_FLAG_VERTICAL, _flags))
	{
		sliderBtnRect = axRect(axPoint(_btnYPos, _sliderPosition),
			_info.btn_size);
	}
	else
	{
		sliderBtnRect = axRect(axPoint(_sliderPosition, _btnYPos),
			_info.btn_size);
	}

	if (sliderBtnRect.IsPointInside(pos))
	{
		if (_nCurrentImg != axBTN_HOVER)
		{
			_nCurrentImg = axBTN_HOVER;
			Update();
		}
	}
	else // Mouse is not hover slider button.
	{
		if (_nCurrentImg != axBTN_NORMAL)
		{
			_nCurrentImg = axBTN_NORMAL;
			Update();
		}
	}
}

void axSlider::OnMouseEnter(const axPoint& p)
{
	// cout << "Enter" << endl;
	axPoint pos = p - GetAbsoluteRect().position;

	if (axFlag_exist(axSLIDER_FLAG_LEFT_CLICK_ENTER, _flags))
	{
		//if (GetParent()->LeftIsDown())
		//{
			axRect sliderBtnRect(axPoint(_sliderPosition, _btnYPos),
				_info.btn_size);

			_nCurrentImg = axBTN_DOWN;
			_currentSliderColor = _info.sliderColorClicked;
			if (axFlag_exist(axSLIDER_FLAG_VERTICAL, _flags))
			{
				_delta_click = sliderBtnRect.position.y - pos.y;
			}
			else
			{
				_delta_click = sliderBtnRect.position.x - pos.x;
			}
			GrabMouse();
			//SendPaintEvent();
			Update();
		//}
	}
}

void axSlider::OnMouseLeave(const axPoint& p)
{
	axPoint pos = p - GetAbsoluteRect().position;

	if (axFlag_exist(axSLIDER_FLAG_RELEASE_ON_LEAVE, _flags))
	{
		if (IsGrabbed())
		{
			UnGrabMouse();
			blockSliderPosition(pos);
			_nCurrentImg = axBTN_NORMAL;
			_currentSliderColor = _info.sliderColorNormal;
			//SendPaintEvent();
			Update();
		}
	}
}

void axSlider::DrawLineBehindSlider_Vertical(axGC* gc, const axRect& rect0)
{
	int half_btn_size = _info.btn_size.y * 0.5;

	axRect slider_rect;
	if (axFlag_exist(axSLIDER_FLAG_RIGHT_ALIGN, _flags))
	{
		// slider_rect = axRect(_sliderYPos, _sliderPosition + half_btn_size,
		// 					 12,// _info.slider_width,
		// 					 GetSize().y - _sliderPosition - half_btn_size);

		slider_rect = axRect(_sliderYPos, _sliderPosition + half_btn_size,
							 _info.slider_width,
							 GetSize().y - _sliderPosition - half_btn_size);
	}
	else
	{
		slider_rect = axRect(_sliderYPos, 0,
							 _info.slider_width,
							 _sliderPosition + half_btn_size);
	}

	gc->SetColor(_currentSliderColor);
	gc->DrawRectangle(slider_rect);

	gc->SetColor(_info.sliderContourColor);
	// gc->DrawRectangleContour(slider_rect);
}

void axSlider::DrawVerticalSlider(axGC* gc, const axRect& rect0)
{
	axSize size(rect0.size);
	int half_btn_size = _info.btn_size.y * 0.5;

	// if (axFlag_exist(axSLIDER_FLAG_BACK_SLIDER, _flags))
	// {
	// 	// Back slider.
	// 	axRect back_slider(_sliderYPos, 0, _info.slider_width, size.y);

	// 	gc->SetColor(_info.backSliderColor, 1.0);
	// 	gc->DrawRectangle(back_slider);

	// 	gc->SetColor(_info.backSliderContourColor);
	// 	gc->DrawRectangleContour(back_slider);
	// }

	// Draw line behind the slider.
	if (!axFlag_exist(axSLIDER_FLAG_NO_SLIDER_LINE, _flags))
	{
		DrawLineBehindSlider_Vertical(gc, rect0);
	}


	gc->SetColor(_info.contourColor);
	gc->DrawRectangleContour(rect0);

	if (_btnImg.IsImageReady())
	{
		gc->DrawPartOfImage(&_btnImg,
			axPoint(0, _nCurrentImg * _info.btn_size.y),
			_info.btn_size,
			axPoint(_btnYPos, _sliderPosition));
	}
}

void axSlider::OnPaint()
{
	axGC* gc = GetGC();
	axSize size = GetSize();
	axRect rect0(0, 0, size.x, size.y);

	gc->SetColor(_currentBgColor, _bg_alpha);
	gc->DrawRectangle(rect0);

	// VERTICAL SLIDER.
	if (axFlag_exist(axSLIDER_FLAG_VERTICAL, _flags))
	{
		DrawVerticalSlider(gc, rect0);
	}

	// HORIZONTAL SLIDER.
	else
	{
		if (axFlag_exist(axSLIDER_FLAG_BACK_SLIDER, _flags))
		{
			// Back slider.
			axRect back_slider(0, _sliderYPos, size.x,
				_info.slider_width);

			gc->SetColor(_info.backSliderColor);
			gc->DrawRectangle(back_slider);

			gc->SetColor(_info.backSliderContourColor);
			gc->DrawRectangleContour(back_slider);
		}

		int half_btn_size = _info.btn_size.x * 0.5;

		if (!axFlag_exist(axSLIDER_FLAG_NO_SLIDER_LINE, _flags))
		{
			axRect slider_rect(0, _sliderYPos,
				_sliderPosition + half_btn_size,
				_info.slider_width);

			if (axFlag_exist(axSLIDER_FLAG_RIGHT_ALIGN, _flags))
			{
				slider_rect = axRect(_sliderPosition,
					_sliderYPos,
					GetSize().x - _sliderPosition + half_btn_size,
					_info.slider_width);
			}
			else
			{
				slider_rect = axRect(0, _sliderYPos,
					_sliderPosition + half_btn_size,
					_info.slider_width);
			}
			gc->SetColor(_currentSliderColor);
			gc->DrawRectangle(slider_rect);

			gc->SetColor(_info.sliderContourColor);
			gc->DrawRectangleContour(slider_rect);
		}


		gc->SetColor(_info.contourColor);
		gc->DrawRectangleContour(axRect(0, 0, size.x, size.y));

		if (_btnImg.IsImageReady())
		{
			gc->DrawPartOfImage(&_btnImg,
				axPoint(0, _nCurrentImg * _info.btn_size.y),
				_info.btn_size,
				axPoint(_sliderPosition, _btnYPos));
		}
	}

}

