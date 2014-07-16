#include "axPopupMenu.h"

axPopupMenu::axPopupMenu(axApp* app,
	axWindow* parent,
	const axRect& rect,
	const axPopupMenuEvents& events,
	const axPopupMenuInfo& info,
	const vector<string>& labels,
	axFlag flag) :
	axPanel(app, parent, rect),
	_events(events),
	_info(info),
	_flag(flag),
	_index(-1),
	_labels(labels),
	_lastSelected(nullptr)
{
	axSize btn_size(rect.size.x, LABEL_HEIGHT);
	//axRect rect(0, i * LABEL_HEIGHT, GetSize().x, LABEL_HEIGHT);

	axButtonEvents btn_event;//(GetOnButtonClick());
	axButtonInfo btn_info("../../../ressources/axObjects/axStandardButton.axobj");
	btn_info.contour = axColor(0.4, 0.4, 0.4);

	for (int i = 0; i < _labels.size(); i++)
	{
		_btns.push_back(new axButton(app, this, 
						axRect(axPoint(0, i * LABEL_HEIGHT), btn_size), 
						btn_event, btn_info, "", _labels[i]));
	}

	SetSize(axSize(btn_size.x, _btns.size() * LABEL_HEIGHT));
}

void axPopupMenu::OnButtonClick(const axButtonMsg& msg)
{
	DSTREAM << "KK" << endl;
	if (_lastSelected == nullptr)
		_lastSelected = msg.GetSender();

	else if (msg.GetSender() != _lastSelected)
	{
		_lastSelected->SetSelected(false);
		_lastSelected = msg.GetSender();
	}
}

void axPopupMenu::OnMouseMotion(const axPoint& pos)
{
	/*if (!IsGrabbed())
	{
		GrabMouse();
	}*/

	//if ()

	/*for (unsigned int i = 0; i < _labels.size(); ++i)
	{
		axRect rect(0, i * LABEL_HEIGHT, GetSize().x, LABEL_HEIGHT);
		axRect r(rect + GetAbsoluteRect().position);

		if (r.IsPointInside(pos))
		{
			if (_index != i)
			{
				_index = i;
				Update();
			}
			break;
		}
	}*/
}

void axPopupMenu::OnMouseLeftUp(const axPoint& pos)
{
	//UnGrabMouse();
	////m_parent->TriggerEvent(m_eventID.valueChange);
	//if (_events.selection_change)
	//{
	//	_events.selection_change(axPopupMenuMsg(_labels[_index]));
	//}
}

void axPopupMenu::OnPaint()
{
	//Resize(axSize(GetSize().x, m_labels.size() * LABEL_HEIGHT));

	///axGC gc(GetBackBuffer());
	axGC* gc = GetGC();
	//axSize size = GetSize();
	axRect rect0(axPoint(0, 0), GetRect().size);

	gc->SetColor(_info.bgColorNormal);
	gc->DrawRectangle(rect0);

	gc->SetColor(_info.contourColor);
	gc->DrawRectangleContour(rect0);

	//axSize label_size(size.x, LABEL_HEIGHT);
	//for (unsigned int i = 0, pos_y = 0; i < _labels.size(); ++i, pos_y += LABEL_HEIGHT)
	//{
	//	if (i == _index)
	//	{
	//		gc->SetColor(_info.bgColorSelected);
	//		axRect selected_rect(axPoint(2, pos_y + 2), label_size - axSize(4, 4));

	//		gc->DrawRectangle(selected_rect);

	//		gc->SetColor(_info.fontColor);
	//		gc->DrawStringAlignedCenter(_labels[i], selected_rect);
	//	}

	//	else
	//	{
	//		gc->SetColor(_info.bgColorNormal);
	//		axRect selected_rect(axPoint(2, pos_y + 2), label_size - axSize(4, 4));

	//		gc->DrawRectangle(selected_rect);

	//		gc->SetColor(_info.fontColor);
	//		gc->DrawStringAlignedCenter(_labels[i], 
	//			axRect(axPoint(0, pos_y), label_size));

	///*		gc.DrawTextAligned(m_labels[i], axTEXT_CENTER,
	//			m_info.bgColorNormal.GetColorRGB(),
	//			"8",
	//			axRect(axPoint(0, pos_y), label_size));*/
	//	}
	//}

	////FlipScreen(gc);
}
