//
//  DrumMidiVelocity.h
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-06.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __DrumNBass__DrumMidiVelocity__
#define __DrumNBass__DrumMidiVelocity__

#include "axLib.h"
#include "axMultipleSlider.h"

class DrumMidiVelocity: public axPanel
{
public:
    /***************************************************************************
     * DrumMidiVelocity::Msg.
     **************************************************************************/
    class Msg : public axMsg
    {
    public:
        Msg();
        
        Msg(DrumMidiVelocity* sender,
            const int& bar_index,
            const int& slider_index,
            const double& value):
        _sender(sender),
        _barIndex(bar_index),
        _sliderIndex(slider_index),
        _value(value)
        {
            
        }
        
        DrumMidiVelocity* GetSender() const
        {
            return _sender;
        }
        
        double GetValue() const
        {
            return _value;
        }
        
        int GetBarIndex() const
        {
            return _barIndex;
        }
        
        int GetSliderIndex() const
        {
            return _sliderIndex;
        }
        
        axMsg* GetCopy()
        {
            return new DrumMidiVelocity::Msg(*this);
        }
        
    private:
        DrumMidiVelocity* _sender;
        double _value;
        int _sliderIndex;
        int _barIndex;
    };
    
    /***************************************************************************
     * DrumMidiVelocity::Events.
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
     * DrumMidiVelocity::DrumMidiVelocity.
     **************************************************************************/
    DrumMidiVelocity(axWindow* parent,
                     const axRect& rect,
                     const DrumMidiVelocity::Events& events);
    
    void SetNumberOfSlider(const int& nb);
    
//    void SetPreset(TrackInfo* info);
    
//    axEVENT(MultipleSliderMsg, OnChangeVelocity);
    axEVENT_ACCESSOR(axNumberBox::Msg, OnStandardDeviation);
    
private:
    std::vector<axMultipleSlider*> _sliders;
    
//    axEvtFunction(MultipleSliderMsg) _velocity_fct;
//    axEventFunction _standard_deviation_fct;
    
//    void OnChangeVelocity(const MultipleSliderMsg& vel);
    
    
    axEVENT_ACCESSOR(axMultipleSlider::Msg, OnChangeVelocity);
    void OnChangeVelocity(const axMultipleSlider::Msg& msg);
    
    void OnStandardDeviation(const axNumberBox::Msg& msg);
    
    virtual void OnPaint();
};

#endif /* defined(__DrumNBass__DrumMidiVelocity__) */
