#ifndef DEF_AX_KNOB_H
#define DEF_AX_KNOB_H

#include "axWindow.h"
#include "axImage.h"
#include "axWidget.h"

/**************************************************************************//**
 * axKnobMsg
******************************************************************************/
class axKnobMsg
{
public:
    axKnobMsg(const double& value):
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
 * axKnobEvents.
******************************************************************************/
struct axKnobEvents
{
    axEvtFunction(axKnobMsg) value_change;

    axKnobEvents(){}
    axKnobEvents(axEvtFunction(axKnobMsg)& fct){ value_change = fct; }
};

/********************************************************************************//**
 * axKnobInfo.
 ***********************************************************************************/
struct axKnobInfo
{
    string img_path, selected_img_path;

    axSize knob_size;

    unsigned int n_knob;

    axColor bgColorNormal,
            bgColorHover,
            bgColorClicked;

    axKnobInfo( const axColor& bg_normalColor,
                const axColor& bg_hoverColor,
                const axColor& bg_clickingColor,
                const unsigned int& numberKnob,
                const axSize& size,
                const string& imgPath,
                const string& sImgPath ):
                // Members.
                img_path( imgPath ),
                selected_img_path( sImgPath ),
                knob_size( size ),
                n_knob( numberKnob ),
                bgColorNormal( bg_normalColor ),
                bgColorHover( bg_hoverColor ),
                bgColorClicked( bg_clickingColor ){}
};

/********************************************************************************//**
 * axKnob.
 ***********************************************************************************/
class axKnob: public axPanel
{
public:
    axKnob(axWindow* parent,
           const axRect& rect,
           const axKnobEvents& events,
           const axKnobInfo& info,
           axFlag flags = 0,
           double value = 0.0);

    double GetValue() { return m_knobValue; }

	void SetValue(const axFloat& value);

	void SetBackgroundAlpha(const float& alpha)
	{
		_bgAlpha = alpha;
		Update();
	}

private:
    axKnobEvents _events;
    axKnobInfo _info;
    axFlag _flags;
    axFloatRange _range;

    axImage* m_knobImg;
    axColor m_currentBgColor;
    unsigned int m_nCurrentImg;
    double m_knobValue;
    int _clickPosY;
	float _bgAlpha;

    void OnPaint();
    void OnMouseLeftDragging(const axPoint& pos);
    void OnMouseLeftDown(const axPoint& pos);
    void OnMouseLeftUp(const axPoint& pos);
};

/********************************************************************************//**
 * axKnobControl.
 ***********************************************************************************/
// class axKnobControl: public axWindow
// {
// public:
//     axKnobControl( axApp* app,
//                    axWindow* parent,
//                    const axID& id,
//                    const axKnobEvents& events,
//                    const axKnobInfo& info,
//                    const axPoint& position );
// private:
//     axKnob* m_knob;
//     axColor m_currentBgColor;

//     void OnPaint();
//     void OnKnobChange();

//     EVENT( axKnobControl, OnKnobChange )
// };

/********************************************************************************//**
 * Knob Templates.
 ***********************************************************************************/
#define axKNOB_INFO_SMALL_RED axKnobInfo( axColor("#777777"),            \
                                          axColor("#999999"),            \
                                          axColor("#999999"),            \
                                          128,                           \
                                          axSize( 32, 32 ),              \
                                          "knob.png",         \
                                          "knobSelected.png")

#define axKNOB_INFO_SIMPLE_BLUE axKnobInfo( axColor("#888888"),            \
                                            axColor("#AAAAAA"),            \
                                            axColor("#BBBBBB"),            \
                                            128,                           \
                                            axSize( 32, 32 ),              \
                                            "ressources/images/knobs/simple_blue_knob.png", \
                                            "ressources/images/knobs/simple_blue_knob.png")
#endif // DEF_AX_KNOB_H.
