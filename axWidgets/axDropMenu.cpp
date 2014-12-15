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
#include "axDropMenu.h"

axDropMenu::axDropMenu(axWindow* parent,
                       const axRect& rect,
                       const axDropMenuEvents& events,
                       const axDropMenuInfo& info,
                       std::string img_path,
                       std::string bg_img_path,
                       std::string toogle_img_path,
                       const std::vector<std::string>& menu_str,
                       axFlag flags,
                       std::string msg) :
// Heritage.
axPanel(parent, rect),
// Members.
_events(events),
_info(info),
_flags(flags),
_nCurrentImg(axDROP_NORMAL),
_selected(false),
test(1.0, 1.0, 0.0),
_msg(msg)
{
    _currentColor = &_info.normal;
    
    _btnImg = new axImage(img_path);
    _bgImg = new axImage(bg_img_path);
    
    if(_events.selection_change)
    {
        AddConnection(axDropMenuEvents::SELECTION_CHANGE,
                      _events.selection_change);
    }
    
    axPopupMenuInfo menu_info(axColor(1.0, 1.0, 0.0, 0.0),
                              axColor(0.2, 0.8, 0.8, 0.0),
                              axColor(0.2, 0.8, 0.8, 0.0),
                              axColor(0.2, 0.8, 0.8, 0.0),
                              axColor(0.2, 0.8, 0.8, 0.0),
                              axColor(0.0, 0.0, 0.0, 0.0));
    
    axPopupMenuEvents menu_evts;
    menu_evts.selection_change = GetOnPopupMenuChoice();
    
    // Popup menu position.
    axPoint menu_pos = GetAbsoluteRect().position;
    menu_pos.y += rect.size.y;
    
    _popMenu = new axPopupMenu(this,
                               axRect(menu_pos, axSize(rect.size.x, 30)),
                               menu_evts,
                               menu_info,
                               menu_str,
                               toogle_img_path);
    
    _popMenu->Hide();
    _popMenu->SetSelectedIndex(0);
    
    _label = menu_str[0];
}

void axDropMenu::SetMsg(const string& msg)
{
    _msg = msg;
}

void axDropMenu::SetSelected(const bool& selected)
{
    if (_selected != selected)
    {
        _selected = selected;
        
        if (_selected == true)
        {
            if (_currentColor == &_info.normal)
            {
                _currentColor = &_info.selected;
                Update();
            }
        }
        else
        {
            if (_currentColor == &_info.selected)
            {
                _currentColor = &_info.normal;
                Update();
            }
        }
    }
}

void axDropMenu::OnPopupMenuChoice(const axPopupMenuMsg& msg)
{
    _popMenu->Hide();
    _label = msg.GetMsg();
    
    PushEvent(axDropMenuEvents::SELECTION_CHANGE,
              new axDropMenuMsg(this, _label));
    Update();
}

void axDropMenu::OnMouseLeftDown(const axPoint& pos)
{
    _currentColor = &_info.clicking;
    _nCurrentImg = axDROP_DOWN;
    
    GrabMouse();
    
    if(_popMenu->IsShown())
    {
        _popMenu->Hide();
    }
    else
    {
        _popMenu->Show();
    }

    
    Update();
}

void axDropMenu::OnMouseLeftUp(const axPoint& pos)
{
    if (IsGrabbed())
    {
        UnGrabMouse();
        
        if (IsMouseHoverWindow())
        {
            _currentColor = &_info.hover;
            _nCurrentImg = axDROP_HOVER;
            _selected = true;
        }
        else
        {
            if (_selected)
            {
                _currentColor = &_info.selected;
                _nCurrentImg = axDROP_SELECTED;
            }
            else
            {
                _currentColor = &_info.normal;
                _nCurrentImg = axDROP_NORMAL;
            }
        }
        
        Update();
    }
}

void axDropMenu::OnMouseEnter()
{
    //DSTREAM << "ENTER" << endl;
    if (!IsGrabbed())
    {
        _currentColor = &_info.hover;
        _nCurrentImg = axDROP_HOVER;
        Update();
    }
}

void axDropMenu::OnMouseLeave()
{
    if (!IsGrabbed())
    {
        if (_selected)
        {
            _currentColor = &_info.selected;
            _nCurrentImg = axDROP_SELECTED;
        }
        else
        {
            _currentColor = &_info.normal;
            _nCurrentImg = axDROP_NORMAL;
        }
    }
    
    Update();
}

void axDropMenu::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetRect());
    axRect rect0(axPoint(0, 0), rect.size);
    
    gc->SetColor(*_currentColor);
    gc->DrawRectangle(rect0);
    
    if (_bgImg->IsImageReady())
    {
        if (axFlag_exist(axDROP_MENU_SINGLE_BACKGROUND_IMG, _flags))
        {
            gc->DrawImageResize(_bgImg, axPoint(0, 0), rect.size, 1.0);
        }
        else
        {
            gc->DrawPartOfImageResize(_bgImg,
                                      axPoint(0, _nCurrentImg * _bgImg->GetSize().y / 3),
                                      axSize(_bgImg->GetSize().x, _bgImg->GetSize().y / 3),
                                      axRect(axPoint(0, 0), GetRect().size));
        }
    }
    
    if (_btnImg->IsImageReady())
    {
        if (axFlag_exist(axDROP_MENU_SINGLE_IMG, _flags))
        {
            gc->DrawImageResize(_btnImg,
                                axPoint(rect.size.x - _btnImg->GetSize().x - 5,
                                        (rect.size.y - _btnImg->GetHeight()) * 0.5),
                                _btnImg->GetSize(), 1.0);
        }
        else
        {
            axSize btn_size = _btnImg->GetSize();
            btn_size.y /= 3;
            
            gc->DrawPartOfImage(_btnImg, axPoint(0, _nCurrentImg * rect.size.y),
                                btn_size,
                                axPoint(rect.size.x - _btnImg->GetSize().x - 5,
                                        (rect.size.y - _btnImg->GetHeight()) * 0.5));
        }
        
    }
    
    if_not_empty(_label)
    {
        gc->SetColor(_info.font_color, 1.0);
        gc->SetFontSize(12);
        gc->DrawStringAlignedCenter(_label, rect0);
    }
    
    gc->SetColor(_info.contour);
    gc->DrawRectangleContour(axRect(axPoint(0, 0), rect.size));
}