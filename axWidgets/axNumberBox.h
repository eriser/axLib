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
#define axNUMBER_BOX_FLAG_LABEL    axFLAG_1
#define axNUMBER_BOX_FLAG_SLIDER   axFLAG_2
#define axNUMBER_BOX_FLAG_NO_MOUSE axFLAG_3

/**************************************************************************//**
 * axNumberBoxMsg
******************************************************************************/
class axNumberBoxMsg
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

private:
    double _value;
};

/**************************************************************************//**
 * axNumberBoxEvents.
******************************************************************************/
struct axNumberBoxEvents
{
    axEvtFunction(axNumberBoxMsg) value_change;

    axNumberBoxEvents(){}
    axNumberBoxEvents(axEvtFunction(axNumberBoxMsg)& fct){ value_change = fct; }
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
                axFlag flags = 0,
                double value = 0.0,
                axFloatRange range = axFloatRange(0.0, 1.0),
                axControlType type = axCTRL_FLOAT,
                axControlUnit m_unit = axCTRL_NO_UNIT,
                axControlInterpolation interpolation = axCTRL_LINEAR,
                string label = "");

    double GetValue();

private:
    axNumberBoxEvents _events;
    axNumberBoxInfo _info;
    axFlag _flags;
    // axSTANDARD_WIDGET_MEMBERS( axNumberBox );
    axControlType _type;
    axControlUnit _unit;
    axFloatRange _range;
    axControlInterpolation _interpolation;
    // axFlag m_flags;

    axColor _currentColor;
    double _value;
    double _zeroToOneValue;
    string _label;

    int _clickPosY;


    void OnPaint();
    void OnMouseEnter();
    void OnMouseLeftDown(const axPoint& pos);
    void OnMouseLeftUp(const axPoint& pos);
    // void OnMouseMotion(const axPoint& pos);
    void OnMouseLeftDragging(const axPoint& pos);
    void OnMouseLeave();

    // void OnPaint();
    // void OnMouseEnter();
    // void OnMouseLeave();
    // void OnMouseLeftDown();
    // void OnMouseLeftUp();
    // void OnLeftDragging();
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

///********************************************************************************//**
//***********************************************************************************/
//class axNumberBoxControl: public axWindow
//{
//public:
//    axNumberBoxControl( axApp* app,
//                        axWindow* parent,
//                        const axID& id,
//                        const string& label,
//                        const axNumberBoxEvents& events,
//                        const axNumberBoxInfo& box_info,
//                        const axPoint& pos );

//    double GetValue();

//private:
//    axWindow* m_parent;
//    axNumberBox* m_numBox;
//    axColor m_currentColor;
//    axNumberBoxEvents m_eventID;
//    string m_label;

//    void OnPaint();

//    void OnValueChange();

//    EVENT( axNumberBoxControl, OnValueChange )
//};

#endif // __AX_NUMBER_BOX__
