//
//  axMultipleSlider.h
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-06.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __DrumNBass__axMultipleSlider__
#define __DrumNBass__axMultipleSlider__

#include "axEvent.h"
#include "axPanel.h"
#include "axColor.h"
#include "axGC.h"
#include "axImage.h"
#include "axMsg.h"
#include "axSlider.h"

class axMultipleSlider: public axPanel
{
public:
    /***************************************************************************
     * axMultipleSlider::Msg.
     **************************************************************************/
    class Msg : public axMsg
    {
    public:
        Msg();
        
        Msg(axMultipleSlider* sender, const int& index, const double& value);
        
        axMultipleSlider* GetSender() const;
        
        double GetValue() const;
        
        int GetIndex() const;
        
        axMsg* GetCopy();
        
    private:
        axMultipleSlider* _sender;
        double _value;
        int _index;
    };
    
    /***************************************************************************
     * axMultipleSlider::Events.
     **************************************************************************/
    class Events
    {
    public:
        enum : axEventId { VALUE_CHANGE };
        
        Events(){}
        Events(const axEventFunction& fct){ value_change = fct; }
        
        axEventFunction value_change;
    };
    
    /***************************************************************************
     * axMultipleSlider::axMultipleSlider.
     **************************************************************************/
    axMultipleSlider(axWindow* parent,
                     const axRect& rect,
                     const axColor& bgColor,
                     const int& barIndex);
    
    void SetNumberOfSlider(const int& nb);
    
    void SetValue(const int& index, const double& value);
    
private:
    axMultipleSlider::Events _events;
    int _nSlider;
    axColor _bgColor;
    axSlider* sliders[3];
    int _barIndex;

    // Events.
    virtual void OnPaint();
    
    axEVENT_ACCESSOR(axSliderMsg, OnSlider1Move);
    void OnSlider1Move(const axSliderMsg& msg);
    
    axEVENT_ACCESSOR(axSliderMsg, OnSlider2Move);
    void OnSlider2Move(const axSliderMsg& msg);
    
    axEVENT_ACCESSOR(axSliderMsg, OnSlider3Move);
    void OnSlider3Move(const axSliderMsg& msg);
};

#endif /* defined(__DrumNBass__axMultipleSlider__) */
