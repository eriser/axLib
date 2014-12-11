#include "axTextBox.h"

axTextBox::axTextBox(axWindow* parent,
                             const axRect& rect,
                             const axTextBoxEvents& events,
                             const axTextBoxInfo& info,
                             string img_path,
                             string label,
                             axFlag flags) :
// Heritage.
axPanel(parent, rect),
// Members.
_events(events),
_info(info),
_label(label),
_flags(flags),
_nCurrentImg(axBTN_NORMAL),
_cursorBarXPosition(5),
_flashingCursor(nullptr),
_cursorFlashActive(true)
{
	_currentColor = &_info.normal;

    _btnImg = new axImage(img_path);
    
    if(_events.button_click)
    {
        AddConnection(axTextBoxEvents::BUTTON_CLICK, _events.button_click);
    }
    
    if(axFlag_exist(axTEXT_BOX_FLASHING_CURSOR, _flags))
    {
        _flashingCursor = new axTimer();
        _flashingCursor->AddConnection(0, GetOnFlashingCursorTimer());
    }
}

void axTextBox::SetLabel(const std::string& label)
{
    _label = label;
    Update();
}

void axTextBox::OnMouseLeftDown(const axPoint& pos)
{
    GrabKey();
    Update();
}

void axTextBox::OnFlashingCursorTimer(const axTimerMsg& msg)
{
    _cursorFlashActive = !_cursorFlashActive;
    Update();
}

void axTextBox::OnMouseLeftUp(const axPoint& pos)
{

}

void axTextBox::OnMouseEnter()
{

}

void axTextBox::OnMouseLeave()
{

}

void axTextBox::OnWasKeyUnGrabbed()
{
    _flashingCursor->StopTimer();
    Update();
}

void axTextBox::OnWasKeyGrabbed()
{
    if(axFlag_exist(axTEXT_BOX_FLASHING_CURSOR, _flags))
    {
        _flashingCursor->StartTimer(500);
    }
    
    Update();
}

void axTextBox::OnKeyDown(const char& key)
{
    if(_cursorBarXPosition < GetRect().size.x - 10)
    {
        _label.push_back(key);
        Update();
    }
}

void axTextBox::OnBackSpaceDown()
{
    _label.pop_back();
    Update();
}

void axTextBox::OnLeftArrowDown()
{
    
}

void axTextBox::OnRightArrowDown()
{
    
}

void axTextBox::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->SetColor(*_currentColor);
	gc->DrawRectangle(rect0);
    
    gc->SetColor(_info.font_color);
    axPoint next_pos(5, 5);
    
    gc->BlockDrawing(rect0);
    
    if_not_empty(_label)
    {
        for(const auto& c : _label)
        {
            next_pos = gc->DrawChar(c, next_pos);
        }
        
        _cursorBarXPosition = next_pos.x;
    }
    else
    {
        _cursorBarXPosition = 5;
    }

    if(IsKeyGrab() && _cursorFlashActive)
    {
        gc->SetColor(_info.cursor);
  
        gc->DrawLine(axPoint(_cursorBarXPosition, 5),
                     axPoint(_cursorBarXPosition, rect0.size.y - 5));
    }
    
    gc->UnBlockDrawing();
    

	gc->SetColor(_info.contour);
	gc->DrawRectangleContour(axRect(axPoint(0, 0), rect.size));
}


