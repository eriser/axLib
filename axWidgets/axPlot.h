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

#ifndef __AX_PLOT__
#define __AX_PLOT__



/*******************************************************************************
 * @file    axPlot.h
 * @author  Alexandre Arsenault <alx.arsenault@gmail.com>
 * @brief   axPlot.
 * @date    19/07/2013
 ******************************************************************************/

/// @defgroup Widgets
/// @{

/// @defgroup NumberBox
/// @{


#include "axEvent.h"
#include "axPanel.h"
#include "axColor.h"
#include "axGC.h"
#include "axImage.h"
#include "axMsg.h"






/*******************************************************************************
 * axPlotFlags.
 ******************************************************************************/

/*******************************************************************************
 * axPlotMsg.
 ******************************************************************************/
class axPlotMsg : public axMsg
{
public:
    axPlotMsg(const double& value):
        _value(value)
    {
    }

    double GetValue() const
    {
        return _value;
    }
    
    axMsg* GetCopy()
    {
        return new axPlotMsg(*this);
    }

private:
    double _value;
};

/**************************************************************************//**
 * axPlotEvents.
******************************************************************************/
struct axPlotEvents
{
    enum : axEventId { VALUE_CHANGE };
    
    axPlotEvents(){}
    axPlotEvents(axEventFunction& fct){ value_change = fct; }
    
    axEventFunction value_change;
};

/**************************************************************************//**
 * axPlotInfo.
******************************************************************************/
struct axPlotInfo
{
    axColor background_color;
    axColor grid_color;
    axColor grid_dark_color;
    axColor contour_color;
    axColor function_color;

    axPlotInfo(){}
    axPlotInfo(
        const axColor& bg_color,
        const axColor& grid_color_,
        const axColor& grid_dark_color_,
        const axColor& contour_color_,
        const axColor& function_color_) :
        background_color(bg_color),
        grid_color(grid_color_),
        grid_dark_color(grid_dark_color_),
        contour_color(contour_color_),
        function_color(function_color_){}
};

class axPlot: public axPanel
{
public:
    axPlot(axWindow* parent,
           const axRect& rect,
           const axPlotEvents& events,
           const axPlotInfo& info,
           const std::string& expr,
           const axFloatRange& xRange,
           const axFloatRange& yRange,
           const std::vector<double> xGridValue = {},
           const std::vector<double> xGridDarkValue = {},
           const std::vector<double> yGridValue = {},
           const std::vector<double> yGridDarkValue = {},
           const axFlag& flags = 0);
    
    void SetExpression(const std::string& expr)
    {
        _expr = expr;
        Update();
    }
    
    int SetPoints(std::vector<axFloatPoint> points)
    {
        _points.push_back(points);
        
        Update();
        
        return (int)_points.size() - 1;
    }
    

private:
    axPlotEvents _events;
    axPlotInfo _info;
    axFlag _flags;
    std::string _expr;
    axFloatRange _xRange, _yRange;
    std::vector<double> _xGridValue, _xGridDarkValue;
    std::vector<double> _yGridValue, _yGridDarkValue;
    
    std::vector<std::vector<axFloatPoint>> _points;

    void OnPaint();
};

#endif // __AX_PLOT__
