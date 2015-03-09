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

axLabel::Info::Info(const std::string& path):
// Heritage.
axInfo(path)
{
    axWidgetLoader loader;
    axVectorPairString att = loader.GetAttributes(path);
    
    SetAttributes(att);
}

axLabel::Info::Info(const axVectorPairString& attributes)
{
    SetAttributes(attributes);
}

axStringVector axLabel::Info::GetParamNameList() const
{
    return axStringVector{"normal",
        "contour", "font_color", "font_name", "font_size", "align"};
}

std::string axLabel::Info::GetAttributeValue(const std::string& name)
{
    if(name == "normal")
    {
        return normal.ToString();
    }
    else if(name == "contour")
    {
        return contour.ToString();
    }
    else if(name == "font_color")
    {
        return font_color.ToString();
    }
    else if(name == "font_name")
    {
        return font_name;
    }
    else if(name == "font_size")
    {
        return to_string(font_size);
    }
    else if(name == "align")
    {
        if(_alignement == axALIGN_LEFT)
        {
            return "left";
        }
        else if(_alignement == axALIGN_CENTER)
        {
            return "center";
        }
        else if(_alignement == axALIGN_RIGHT)
        {
            return "right";
        }
    }

    return "";
}

void axLabel::Info::SetAttribute(const axStringPair& attribute)
{
    if(attribute.first == "normal")
    {
        normal.LoadFromString(attribute.second);
    }
    else if(attribute.first == "contour")
    {
        contour.LoadFromString(attribute.second);
    }
    else if(attribute.first == "font_color")
    {
        font_color.LoadFromString(attribute.second);
    }
    else if(attribute.first == "font_name")
    {
        font_name = attribute.second;
    }
    else if(attribute.first == "font_size")
    {
        font_size = stoi(attribute.second);
    }
    else if(attribute.first == "align")
    {
        if(attribute.second == "left")
        {
            _alignement = axALIGN_LEFT;
        }
        else if(attribute.second == "center")
        {
            _alignement = axALIGN_CENTER;
        }
        else if(attribute.second == "right")
        {
            _alignement = axALIGN_RIGHT;
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
axWidget(parent, rect, new Info(info)),
//_info(info),
_label(label),
_font(nullptr)
{
    SetSelectable(false);
    
    if(static_cast<Info*>(_info)->font_name != "")
    {
        _font = std::unique_ptr<axFont>(new axFont(static_cast<Info*>(_info)->font_name));
        
    }
    else
    {
		_font = std::unique_ptr<axFont>(new axFont(0));
        axPrint("axLabel :: Default font.");
    }
    
    _font->SetFontSize(static_cast<Info*>(_info)->font_size);
}

void axLabel::SetLabel(const std::string& label)
{
    _label = label;
    Update();
}

void axLabel::OnPaint()
{
    axGC* gc = GetGC();
//    axRect rect(axPoint(0, 0), GetRect().size);
    axRect rect(GetDrawingRect());
    
    gc->SetColor(static_cast<Info*>(_info)->normal);
    gc->DrawRectangle(rect);
    
    gc->SetColor(static_cast<Info*>(_info)->font_color);
    
    if(static_cast<Info*>(_info)->_alignement == axALIGN_CENTER)
    {
        gc->DrawStringAlignedCenter(*_font, _label, rect);
    }
    else if(static_cast<Info*>(_info)->_alignement == axALIGN_LEFT)
    {
        gc->DrawString(*_font, _label, axPoint(5, 2));
    }
    
    
    gc->SetColor(static_cast<Info*>(_info)->contour);
    gc->DrawRectangleContour(rect);
}
