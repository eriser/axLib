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
#include "axWidgetSelector.h"

/*******************************************************************************
 * axWidgetSelector::Info.
 ******************************************************************************/
axWidgetSelector::Info::Info()
{
    
}

axWidgetSelector::Info::Info(const axColor& color):
_color(color)
{
    
}

/*******************************************************************************
 * axWidgetSelector.
 ******************************************************************************/
axWidgetSelector::axWidgetSelector(axWindow* parent):
axWidget(parent, axRect(0, 0, 20, 20),
         new_ Info(axColor(1.0, 0.33, 0.0))),
_selectedWidget(nullptr)
{
    SetSelectable(false);
    Hide();
}

void axWidgetSelector::SetSelectedWidget(axWindow* win)
{
    _selectedWidget = win;
    axRect rect = win->GetRect();
    
    SetPosition(axPoint(rect.position.x - 3, rect.position.y - 3));
    SetSize(axSize(rect.size.x + 6, rect.size.y + 6));
    
    Show();
}

void axWidgetSelector::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetDrawingRect());

//    gc->SetColor(static_cast<Info*>(_info)->_color, 0.2);
//    gc->DrawRectangleContour(rect.GetInteriorRect(axPoint(1, 1)));
    
    gc->SetColor(static_cast<Info*>(_info)->_color, 0.1);
    gc->DrawRectangleContour(rect);
    
    gc->SetColor(static_cast<Info*>(_info)->_color, 0.2);
    gc->DrawRectangleContour(rect.GetInteriorRect(axPoint(1, 1)));
    
    gc->SetColor(static_cast<Info*>(_info)->_color, 0.4);
    gc->DrawRectangleContour(rect.GetInteriorRect(axPoint(2, 2)));
    
    gc->SetColor(static_cast<Info*>(_info)->_color, 0.5);
    gc->DrawRectangleContour(rect.GetInteriorRect(axPoint(3, 3)));
//
//    gc->SetColor(static_cast<Info*>(_info)->_color, 0.6);
//    gc->DrawRectangleContour(rect.GetInteriorRect(axPoint(3, 3)));
//    
//    gc->SetColor(static_cast<Info*>(_info)->_color, 1.0);
//    gc->DrawRectangleContour(rect.GetInteriorRect(axPoint(4, 4)));
}
