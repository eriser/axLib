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
#ifndef __AX_TOGGLE__
#define __AX_TOGGLE__

/// @defgroup Widgets
/// @{

/// @defgroup Toggle
/// @{

#include "axEvent.h"
#include "axPanel.h"
#include "axColor.h"
#include "axGC.h"
#include "axImage.h"
#include "axMsg.h"
#include <fstream>


/**************************************************************************//**
 * axToggleFlags.
******************************************************************************/
#define axTOGGLE_SINGLE_IMG	axFLAG_1
#define axTOGGLE_IMG_RESIZE	axFLAG_2
#define axTOGGLE_CLICK_ON_LEFT_DOWN axFLAG_3
#define axTOGGLE_CANT_UNSELECT_WITH_MOUSE axFLAG_4

class axToggle;

class axToggleMsg : public axMsg
{
public:
	axToggleMsg()
	{
		_sender = nullptr;
	}

	axToggleMsg(axToggle* sender, const bool& selected)
	{
		_sender = sender;
		_selected = selected;
        _msg = std::string("");
	}
    
    axToggleMsg(axToggle* sender, const bool& selected, const std::string& msg)
    {
        _sender = sender;
        _selected = selected;
        _msg = msg;
    }

	axToggle* GetSender() const
	{
		return _sender;
	}
    
    bool GetSelected() const
    {
        return _selected;
    }
    
    string GetMsg() const
    {
        return _msg;
    }
    
    axMsg* GetCopy()
    {
        return new axToggleMsg(*this);
    }

private:
	axToggle* _sender;
    bool _selected;
    std::string _msg;
};

struct axToggleEvents
{
    enum : axEventId { BUTTON_CLICK };
    
	axEventFunction button_click;
	
	axToggleEvents(){}
	axToggleEvents(const axEventFunction& fct){ button_click = fct; }
};

struct axToggleInfo
{
	axColor normal;
	axColor hover;
	axColor clicking;
    
	axColor selected;
    axColor selected_hover;
    axColor selected_clicking;
    
    /// @todo Add select font color to axToggle.
    axColor selected_font_color;
    
	axColor contour;
	axColor font_color;

	axToggleInfo(){}
	axToggleInfo(
		const axColor& normal_color,
		const axColor& hover_color,
		const axColor& clicked_color,
		const axColor& selected_color,
        const axColor& selectedHover_color,
        const axColor& selectedClicking_color,
		const axColor& contour_color,
		const axColor& font_color_) :
		normal(normal_color),
		hover(hover_color),
		clicking(clicked_color),
		selected(selected_color),
        selected_hover(selectedHover_color),
        selected_clicking(selectedClicking_color),
		contour(contour_color),
		font_color(font_color_){}

    void SerializeOutput(const std::string& path)
    {
        fstream file;
        file.open(path, std::fstream::out | std::fstream::binary);
        
        if (file.fail())
        {
            std::cerr << "Problem opening file " << path << std::endl;
        }
        else
        {
            normal.SerializeOutput(file);
            hover.SerializeOutput(file);
            clicking.SerializeOutput(file);
            
            selected.SerializeOutput(file);
            selected_hover.SerializeOutput(file);
            selected_clicking.SerializeOutput(file);
            
            contour.SerializeOutput(file);
            font_color.SerializeOutput(file);
        }
    }
    
    void SerializeInput(const std::string& path)
    {
        fstream file;
        file.open(path, std::fstream::in | std::fstream::binary);
        
        if (file.fail())
        {
            std::cerr << "Problem opening file " << path << std::endl;
        }
        else
        {
            normal.SerializeInput(file);
            hover.SerializeInput(file);
            clicking.SerializeInput(file);
            
            selected.SerializeInput(file);
            selected_hover.SerializeInput(file);
            selected_clicking.SerializeInput(file);
            
            contour.SerializeInput(file);
            font_color.SerializeInput(file);
        }
    }
};

#define axSTANDARD_TOGGLE 	axToggleInfo( \
							axColor(0.5, 0.5, 0.5),\
							axColor(0.6, 0.6, 0.6),\
							axColor(0.4, 0.4, 0.4),\
							axColor(0.5, 0.5, 0.5),\
							axColor(0.0, 0.0, 0.0),\
							axColor(0.0, 0.0, 0.0)) 

#define axTOGGLE_TRANSPARENT 	axToggleInfo( \
axColor(0.5, 0.5, 0.5, 0.0),\
axColor(0.6, 0.6, 0.6, 0.0),\
axColor(0.4, 0.4, 0.4, 0.0),\
axColor(0.5, 0.5, 0.5, 0.0),\
axColor(0.0, 0.0, 0.0, 0.0),\
axColor(0.0, 0.0, 0.0, 0.0),\
axColor(0.0, 0.0, 0.0, 0.0),\
axColor(0.0, 0.0, 0.0, 1.0))

class axToggle : public axPanel
{
public:
	axToggle(axWindow* parent,
             const axRect& rect,
             const axToggleEvents& events,
             const axToggleInfo& info,
             string img_path = "",
             string label = "",
             axFlag flags = 0,
             string msg = "");

	void SetBackgroundAlpha(const float& alpha);

	void SetMsg(const string& msg);
	void SetSelected(const bool& selected);
    
    class axToggleBuilder
    {
    public:
        axToggleBuilder(axPanel* parent,
                        const axSize& size,
                        const axToggleInfo& info,
                        string img_path = "",
                        string label = "",
                        axFlag flags = 0,
                        int nextPositionDelta = 5,
                        axDirection direction = axDIRECTION_RIGHT):
        _parent(parent),
        _size(size),
        _info(info),
        _img(img_path),
        _label(label),
        _flags(flags),
        _direction(direction),
        _nextPositionDelta(nextPositionDelta),
        _past(nullptr)
        {
            
        }
        
        axToggleBuilder(axWindow* win):
        _parent(win),
        _past(nullptr)
        {
            
        }
        
        axToggle* Create(axVectorPairString attributes)
        {
            std::string name;
            axPoint pos;
            axToggleEvents evts;
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
                    _info.SerializeInput(s.second);
                }
                else if(s.first == "label")
                {
                    _label = s.second;
                }
                else if(s.first == "flags")
                {
                    _flags = stoi(s.second);
                }
                else if(s.first == std::string("event"))
                {
                    evts.button_click = _parent->GetEventFunction(s.second);
                }

            }
            
            axToggle* tog = new axToggle(_parent, axRect(pos, _size),
                                         evts,
                                         _info, _img, _label);
            
            _parent->GetResourceManager()->Add(name, tog);
            return tog;

        }
        
        axToggle* Create(const axPoint& pos, const axEventFunction& evt)
        {
//            return _pastKnob = new axKnob(_parent, axRect(pos, _size), evt,
//                                          _info, _flags);
            return nullptr;
        }
        
        axToggle* Create(const axPoint& pos)
        {
//            axKnobEvents evts;
//            return _pastKnob = new axKnob(_parent, axRect(pos, _size), evts,
//                                          _info, _flags);
            return nullptr;

        }
        
        axToggle* Create(const axEventFunction& evt)
        {
//            if(_pastKnob != nullptr)
//            {
//                if(_direction == axDIRECTION_RIGHT)
//                {
//                    axPoint pos(_pastKnob->GetNextPosRight(_nextPositionDelta));
//                    return _pastKnob = new axKnob(_parent, axRect(pos, _size), evt,
//                                                  _info, _flags);
//                }
//                else if(_direction == axDIRECTION_DOWN)
//                {
//                    
//                }
//                else if(_direction == axDIRECTION_LEFT)
//                {
//                    
//                }
//                
//                else //axDIRECTION_UP
//                {
//                    
//                }
//                
//            }
            
            return nullptr;
        }
        
        axToggle* Create()
        {
//            if(_pastKnob != nullptr)
//            {
//                axKnobEvents evts;
//                axPoint pos(_pastKnob->GetNextPosRight(_nextPositionDelta));
//                return _pastKnob = new axKnob(_parent, axRect(pos, _size),
//                                              evts, _info, _flags);
//            }
//            
            return nullptr;
        }
        
    private:
        axWindow* _parent;
        axToggleInfo _info;
        std::string _label;
        std::string _img;
        axFlag _flags;
        axSize _size;
        int _nextPositionDelta;
        axToggle* _past;
        axDirection _direction;
    };
    

private:
	axToggleEvents _events;
	axToggleInfo _info;
	axColor* _currentColor;
	axImage* _btnImg;
	string _label;
	string _msg;
	
	bool _selected;
	axFlag _flags;
	axColor test;
	axFloat _bgAlpha;

	enum axToggleState
	{
		axTOG_NORMAL,
		axTOG_DOWN,
		axTOG_HOVER,
		axTOG_SELECTED
	};

	int _nCurrentImg;

	axEvent OnPaint();
	axEvent OnMouseLeftDown(const axPoint& pos);
	axEvent OnMouseLeftUp(const axPoint& pos);
	axEvent OnMouseEnter();
	axEvent OnMouseLeave();
};


/// @}
/// @}
#endif //__AX_TOGGLE__

