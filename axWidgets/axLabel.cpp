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
#include "axLabel.h"

/*******************************************************************************
 * axLabel::Info.
 ******************************************************************************/
axLabel::Info::Info()
{
    
}

axLabel::Info::Info(const axColor& normal_color,
                    const axColor& contour_color,
                    const axColor& fontColor,
                    const int fontSize):
normal(normal_color),
contour(contour_color),
font_color(fontColor),
font_size(fontSize)
{
    
}

axLabel::Info::Info(const std::string& path)
{
    axWidgetLoader loader;
    axVectorPairString att = loader.GetAttributes(path);
    
    for(auto& n : att)
    {
        if(n.first == "normal")
        {
            normal.LoadFromString(n.second);
        }
        else if(n.first == "contour")
        {
            contour.LoadFromString(n.second);
        }
        else if(n.first == "font_color")
        {
            font_color.LoadFromString(n.second);
        }
        else if(n.first == "font_size")
        {
            font_size = stoi(n.second);
        }
        else if(n.first == "font_name")
        {
            font_name = n.second;
        }
    }
}

/*******************************************************************************
 * axLabel::Builder.
 ******************************************************************************/
axLabel::Builder::Builder(axWindow* win):
_parent(win)
{
    
}

axLabel* axLabel::Builder::Create(axVectorPairString attributes)
{
    std::string name;
    axPoint pos;
    for(auto& s : attributes)
    {
        if(s.first == "name")
        {
            name = s.second;
        }
        else if(s.first == "rect")
        {
            axStringVector strVec;
            strVec = GetVectorFromStringDelimiter(s.second, ",");
            
            pos = axPoint(stoi(strVec[0]),
                          stoi(strVec[1]));
            
            _size = axSize(stoi(strVec[2]),
                           stoi(strVec[3]));
        }
        else if(s.first == "info")
        {
            _info = axLabel::Info(s.second);
        }
        else if(s.first == "label")
        {
            _label = s.second;
        }
    }
    
    axLabel* label = new axLabel(_parent, axRect(pos, _size),
                                 _info, _label);
    
    _parent->GetResourceManager()->Add(name, label);
    return label;
}

/*******************************************************************************
 * axLabel.
 ******************************************************************************/
axLabel::axLabel(axWindow* parent,
                 const axRect& rect,
                 const axLabel::Info& info,
                 const std::string& label):
axPanel(parent, rect),
_info(info),
_label(label)
{
    SetSelectable(false);
}

void axLabel::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(axPoint(0, 0), GetRect().size);
    
    gc->SetColor(_info.normal);
    gc->DrawRectangle(rect);
    
    gc->SetColor(_info.font_color);
    gc->SetFontSize(_info.font_size);
    gc->DrawStringAlignedCenter(_label, rect);
    
    gc->SetColor(_info.contour);
    gc->DrawRectangleContour(rect);
}
