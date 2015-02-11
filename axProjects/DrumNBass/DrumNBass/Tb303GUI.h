//
//  Tb303GUI.h
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-06.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __DrumNBass__Tb303GUI__
#define __DrumNBass__Tb303GUI__

#include "axLib.h"

class MyLED : public axPanel
{
public:
    MyLED(axWindow* parent,
          const axRect& rect);
    
    void SetActive(const bool& on);
    void SetOff();
    
    bool IsActive() const
    {
        return (bool)_imgIndex;
    }
    
private:
    void OnPaint();
    int _imgIndex;
    
    
    axImage* _ledImg;
};

class MyNumberPanel : public axPanel
{
public:
    MyNumberPanel(axWindow* parent,
                  const axPoint& pos);
    
    void SetNumber(const int& num);
    int GetNumber() const;
    
private:
    void OnPaint();
    int _number;
};

class MyButton : public axButton
{
public:
    MyButton(axWindow* parent,
             const axRect& rect,
             const axButton::Events& events,
             const axButton::Info& info,
             const axPoint& led_delta_pos);
    
    void SetLed(MyLED* led);
    
    void SetActive(const bool& on);
    void SetOff();
    
    bool IsActive() const
    {
        return _led->IsActive();
    }
    
    enum MyButtonType
    {
        BUTTON_PALE,
        BUTTON_DARK
    };
    
    axEVENT_ACCESSOR(axButton::Msg, OnClickButton);
    
    class MyButtonBuilder
    {
    public:
        MyButtonBuilder(axWindow* parent,
                        const axSize& size,
                        const axButton::Info& info,
                        const axPoint& led_delta_pos,
                        const int& x_delta);
        
        MyButton* Create(const axPoint& pos, const axEventFunction& evt);
        MyButton* Create(const axEventFunction& evt);
        
        void SetDelta(const axPoint& delta);
        
        void SetXDelta(const int& x)
        {
            _xDelta = x;
        }
        
        void SetInfo(const axButton::Info& info)
        {
            _info = info;
        }
        
    private:
        axWindow* _parent;
        axSize _size;
        axButton::Info  _info;
        axPoint _delta;
        int _xDelta;
        MyButton* _pastBtn;
    };
    
private:
    MyLED* _led;
    bool _active;
    
    void OnClickButton(const axButton::Msg& msg);
};

class MyPreference : public axPanel
{
public:
    MyPreference(const axRect& rect);
    
private:
    // Events.
    virtual void OnPaint();
};

class MyProject: public axPanel
{
public:
    MyProject(axWindow* parent,
              const axRect& rect);
    
    axEVENT_ACCESSOR(axButton::Msg, OnRunClick);
    axEVENT_ACCESSOR(axButton::Msg, OnNoteClick);
    axEVENT_ACCESSOR(axButton::Msg, OnButtonClick);
    axEVENT_ACCESSOR(axDropMenuMsg, OnWaveChoice);
    
    axEVENT_ACCESSOR(axButton::Msg, OnDownClick);
    axEVENT_ACCESSOR(axButton::Msg, OnUpClick);
    
    axEVENT_ACCESSOR(axKnob::Msg, OnTuningChange);
    axEVENT_ACCESSOR(axKnob::Msg, OnVolumeChange);
    axEVENT_ACCESSOR(axKnob::Msg, OnFreqChange);
    axEVENT_ACCESSOR(axKnob::Msg, OnResChange);
    
    axEVENT_ACCESSOR(axKnob::Msg, OnDecayChange);
    
    axEVENT_ACCESSOR(axButton::Msg, OnNextEditPattern);
    axEVENT_ACCESSOR(axButton::Msg, OnBackEditPattern);
    
    axEVENT_ACCESSOR(axButton::Msg, OnPreference);
    
    enum MyButtonId
    {
        NOTE_C0,
        NOTE_D,
        NOTE_E,
        NOTE_F,
        NOTE_G,
        NOTE_A,
        NOTE_B,
        NOTE_C1,
        
        DOWN,
        UP,
        ACCENT,
        SLIDE,
        
        NOTE_C0_S,
        NOTE_D_S,
        NOTE_F_S,
        NOTE_G_S,
        NOTE_A_S,
        NUM_OF_BUTTONS
    };
    
private:
    
    void UpdateParameters(const int& index);
    
    // Events.
    virtual void OnPaint();
    
    void OnRunClick(const axButton::Msg& msg);
    void OnNoteClick(const axButton::Msg& msg);
    void OnButtonClick(const axButton::Msg& msg);
    void OnWaveChoice(const axDropMenuMsg& msg);
    
    void OnDownClick(const axButton::Msg& msg);
    void OnUpClick(const axButton::Msg& msg);
    
    void OnVolumeChange(const axKnob::Msg& msg);
    void OnTuningChange(const axKnob::Msg& msg);
    void OnFreqChange(const axKnob::Msg& msg);
    void OnResChange(const axKnob::Msg& msg);
    
    void OnDecayChange(const axKnob::Msg& msg);
    
    void OnPreference(const axButton::Msg& msg);
    
    
    void OnNextEditPattern(const axButton::Msg& msg);
    void OnBackEditPattern(const axButton::Msg& msg);
    
    axImage* _bgImg;
    MyNumberPanel* _numberPanel;
    MyPreference* _pref;
    std::vector<MyButton*> _btns;
};

#endif /* defined(__DrumNBass__Tb303GUI__) */
