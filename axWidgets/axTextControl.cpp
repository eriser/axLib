#include "axTextControl.h"

axTextControl::axTextControl(axWindow* parent,
                             const axRect& rect,
                             const axTextControlEvents& events,
                             const axTextControlInfo& info,
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
_cursorBarXPosition(5)
{
	_currentColor = &_info.normal;

    _btnImg = new axImage(img_path);
    
    if(_events.button_click)
    {
        AddConnection(axTextControlEvents::BUTTON_CLICK, _events.button_click);
    }
	
}

void axTextControl::SetLabel(const std::string& label)
{
    _label = label;
    Update();
}

void axTextControl::OnMouseLeftDown(const axPoint& pos)
{
    GrabKey();
    Update();
}

void axTextControl::OnMouseLeftUp(const axPoint& pos)
{

}

void axTextControl::OnMouseEnter()
{

}

void axTextControl::OnMouseLeave()
{

}

void axTextControl::OnKeyDown(const char& key)
{
    if(_cursorBarXPosition < GetRect().size.x - 10)
    {
        _label.push_back(key);
        Update();
    }
    
    
}

void axTextControl::OnBackSpaceDown()
{
    _label.pop_back();
    Update();
}

void axTextControl::OnPaint()
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

    if(IsKeyGrab())
    {
        gc->SetColor(_info.cursor);
  
        gc->DrawLine(axPoint(_cursorBarXPosition, 5),
                     axPoint(_cursorBarXPosition, rect0.size.y - 5));
    }
    
    gc->UnBlockDrawing();
    

	gc->SetColor(_info.contour);
	gc->DrawRectangleContour(axRect(axPoint(0, 0), rect.size));
}


