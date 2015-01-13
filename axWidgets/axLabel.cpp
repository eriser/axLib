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
                    const int fontSize,
                    const std::string& fontName,
                    const axAlignement& alignement):
normal(normal_color),
contour(contour_color),
font_color(fontColor),
font_size(fontSize),
_alignement(alignement),
font_name(fontName)
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
        else if(n.first == "align")
        {
            if(n.second == "left")
            {
                _alignement = axALIGN_LEFT;
            }
            else if(n.second == "center")
            {
                _alignement = axALIGN_CENTER;
            }
            else if(n.second == "right")
            {
                _alignement = axALIGN_RIGHT;
            }
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

void axLabel::SetLabel(const std::string& label)
{
    _label = label;
    Update();
}

void axLabel::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(axPoint(0, 0), GetRect().size);
    
    gc->SetColor(_info.normal);
    gc->DrawRectangle(rect);
    
    gc->SetColor(_info.font_color);
    axFont font(_info.font_name);
    font.SetFontSize(_info.font_size);
//    gc->SetFontType(_info.font_name);
//    gc->SetFontSize(_info.font_size);
    
    if(_info._alignement == axALIGN_CENTER)
    {
        gc->DrawStringAlignedCenter(font, _label, rect);
    }
    else if(_info._alignement == axALIGN_LEFT)
    {
        gc->DrawString(font, _label, axPoint(5, 2));
    }
    
    
    gc->SetColor(_info.contour);
    gc->DrawRectangleContour(rect);
}
