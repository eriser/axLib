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

#ifndef __AX_DEBUG_BUTTON__
#define __AX_DEBUG_BUTTON__

/*******************************************************************************
 * @file    axDebugButton.h
 * @author  Alexandre Arsenault <alx.arsenault@gmail.com>
 * @brief   axDebugButton.
 * @date    14/01/2015
 ******************************************************************************/

#include "axPanel.h"
#include "axButton.h"
#include "axLabel.h"
#include "axTextBox.h"

class axDebugButton : public axPanel
{
public:
    axDebugButton(axWindow* parent);

    void SetMsg(const std::string& msg);
    
    void SetSelected(const bool& selected);
    
    void SetLabel(const std::string& label);
    
    axEVENT_ACCESSOR(axTextBox::Msg, OnAttributeEdit);
    
protected:
    axButton::Events _events;
    axButton::Info _info;
    std::string _label, _msg;
//    axImage* _btnImg;
    axFlag _flags;
    std::unique_ptr<axFont> _font;
    
    axColor* _currentColor;
    bool _selected;
//    int _nCurrentImg;
    
    axStringVector _infoVector;
    
    typedef std::tuple<std::string, axLabel*, axTextBox*> axEditorTuple;
    std::vector<axEditorTuple> _infoEditor;
    
//    enum axButtonState
//    {
//        axBTN_NORMAL,
//        axBTN_HOVER,
//        axBTN_DOWN,
//        axBTN_SELECTED
//    };
    
    virtual void OnPaint();
    virtual void OnMouseLeftDown(const axPoint& pos);
    virtual void OnMouseLeftDragging(const axPoint &pos);
    virtual void OnMouseLeftUp(const axPoint& pos);
    
    virtual void OnMouseRightDown(const axPoint& pos);
    
    virtual void OnMouseEnter();
    virtual void OnMouseLeave();
    
    void OnAttributeEdit(const axTextBox::Msg& msg);
    
    
    bool _isEditing;
};

#endif //__AX_DEBUG_BUTTON__




