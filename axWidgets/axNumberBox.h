#ifndef __AX_NUMBER_BOX__
#define __AX_NUMBER_BOX__

/**************************************************************************//**
 * @file    axNumberBox.h
 * @author  Alexandre Arsenault <alexandre.arsenault@polymtl.ca>
 * @brief   axNumberBox.
 * @date    19/07/2013
 *****************************************************************************/
#include "axEvent.h"
#include "axPanel.h"
#include "axColor.h"
#include "axGC.h"
#include "axImage.h"
#include "axMsg.h"

 enum axControlType
{
    axCTRL_NO_TYPE,
    axCTRL_FLOAT,
    axCTRL_INT,
    axCTRL_PAN
};

enum axControlUnit
{
    axCTRL_NO_UNIT,
    axCTRL_HERTZ,
    axCTRL_DECIBEL,
    axCTRL_TIME_MS,
    axCTRL_TIME_SEC
};

enum axControlInterpolation
{
    axCTRL_LINEAR,
    axCTRL_EXPONENTIAL,
    axCTRL_LOGARITHME
};

 /*************************************************************************//**
 * axNumberBoxFlags.
******************************************************************************/
#define axNUMBER_BOX_SINGLE_IMG    axFLAG_1
#define axNUMBER_BOX_FLAG_LABEL    axFLAG_2
#define axNUMBER_BOX_FLAG_SLIDER   axFLAG_3
#define axNUMBER_BOX_FLAG_NO_MOUSE axFLAG_4

/**************************************************************************//**
 * axNumberBoxMsg
******************************************************************************/
class axNumberBoxMsg : public axMsg
{
public:
    axNumberBoxMsg(const double& value):
        _value(value)
    {
    }

    double GetValue() const
    {
        return _value;
    }
    
    axMsg* GetCopy()
    {
        return new axNumberBoxMsg(*this);
    }

private:
    double _value;
};

/**************************************************************************//**
 * axNumberBoxEvents.
******************************************************************************/
struct axNumberBoxEvents
{
    enum : axEventId { VALUE_CHANGE };
    
    axNumberBoxEvents(){}
    axNumberBoxEvents(axEventFunction& fct){ value_change = fct; }
    
    axEventFunction value_change;
};

/**************************************************************************//**
 * axNumberBoxInfo.
******************************************************************************/
struct axNumberBoxInfo
{
    axColor normal;
    axColor hover;
    axColor clicking;
    axColor selected;
    axColor contour;
    axColor font_color;

    axNumberBoxInfo(){}
    axNumberBoxInfo(
        const axColor& normal_color,
        const axColor& hover_color,
        const axColor& clicked_color,
        const axColor& selected_color,
        const axColor& contour_color,
        const axColor& font_color_) :
        normal(normal_color),
        hover(hover_color),
        clicking(clicked_color),
        selected(selected_color),
        contour(contour_color),
        font_color(font_color_){}
};



/********************************************************************************//**
 * axNumberBox.
 * @todo Need to find a way to hide mouse when clicking.
 * @todo Figure out what to do with Label and axXXXControl class.
************************************************************************************/
class axNumberBox: public axPanel
{
public:
    axNumberBox(axWindow* parent,
                const axRect& rect,
                const axNumberBoxEvents& events,
                const axNumberBoxInfo& info,
                std::string img_path = "",
                axFlag flags = 0,
                double value = 0.0,
                axFloatRange range = axFloatRange(0.0, 1.0),
                axControlType type = axCTRL_FLOAT,
                axControlUnit m_unit = axCTRL_NO_UNIT,
                axControlInterpolation interpolation = axCTRL_LINEAR,
                std::string label = "");

    double GetValue();

private:
    axNumberBoxEvents _events;
    axNumberBoxInfo _info;
    axFlag _flags;
    axImage* _bgImg;

    axControlType _type;
    axControlUnit _unit;
    axFloatRange _range;
    axControlInterpolation _interpolation;

    axColor _currentColor;
    int _nCurrentImg;
    double _value;
    double _zeroToOneValue;
    std::string _label;

    int _clickPosY;

    enum axNumberBoxState
    {
        axNUM_BOX_NORMAL,
        axNUM_BOX_DOWN,
        axNUM_BOX_HOVER
    };

    void OnPaint();
    void OnMouseEnter();
    void OnMouseLeftDown(const axPoint& pos);
    void OnMouseLeftUp(const axPoint& pos);
    void OnMouseLeftDragging(const axPoint& pos);
    void OnMouseLeave();

};

#define axNUMBER_BOX_STANDARD axNumberBoxInfo( axColor("#AA1111"),          \
                                               axColor("#CC1111"),          \
                                               axColor("#FF1111"),          \
                                               axColor("#FF0000"),          \
                                               axColor("#000000") )

#define axNUMBER_BOX_WHITE axNumberBoxInfo( axColor("#FFFFFF"),          \
                                            axColor("#DDDDDD"),          \
                                            axColor("#AAAAAA"),          \
                                            axColor("#FF0000"),          \
                                            axColor("#000000") )

#endif // __AX_NUMBER_BOX__
