//
//  axWaveformNavigator.h
//  MidiSequencer
//
//  Created by Alexandre Arsenault on 2014-11-15.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#ifndef __MidiSequencer__axWaveformNavigator__
#define __MidiSequencer__axWaveformNavigator__

#include "axLib.h"

class axAudioBuffer;

class axWaveformNavigator : public axPanel
{
public:
    axWaveformNavigator(axApp* app,
                        axWindow* parent,
                        const axRect& rect);
    
    void SetAudioBuffer(axAudioBuffer* buffer);
    
//    void SetZoom(const double& zoom);
//
    void SetBorders(const double& left, const double& right);
    void SetBorders(const axFloatRange& borders);
    void SetLeftBorder(const double& pos);
    void SetRightBorder(const double& pos);
    
    void SetValueChangeEvt(axEvtFunction(double) fct);
    
private:
    axAudioBuffer* _audioBuffer;
    
    double _leftBorder, _rightBorder;
    double _fillAlpha;
    
    int _click_pos_delta_x;
    
    axRect GetBorderRect() const;
    axFloatRange GetBorderRangeFromRect(const axRect& rect) const;
    
    axEvtFunction(double) _value_change_evt;
    
    // Events.
    virtual void OnPaint();
    virtual void OnMouseLeftDragging(const axPoint& pos);
    virtual void OnMouseLeftDown(const axPoint& pos);
    virtual void OnMouseLeftUp(const axPoint& pos);
    virtual void OnMouseEnter();
    virtual void OnMouseLeave();
};

#endif /* defined(__MidiSequencer__axWaveformNavigator__) */
