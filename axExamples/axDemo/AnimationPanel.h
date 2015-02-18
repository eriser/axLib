//
//  AnimationPanel.h
//  axDemo
//
//  Created by Alexandre Arsenault on 2014-12-10.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#ifndef __axDemo__AnimationPanel__
#define __axDemo__AnimationPanel__

#include <axLib/axLib.h>
//#include "axLib.h"

class AnimationPanel : public axPanel
{
public:
    AnimationPanel(axWindow* parent,
                   const axRect& rect);
    
    
    axEVENT_ACCESSOR(axTimerMsg, OnTimerAnimation);
    axEVENT_ACCESSOR(axTimerMsg, OnClickAnimation);
    axEVENT_ACCESSOR(axTimerMsg, OnReleaseAnimation);
    
private:
    virtual void OnPaint();
    virtual void OnMouseMotion(const axPoint& pos);
    virtual void OnMouseLeftDown(const axPoint& pos);
    virtual void OnMouseLeftUp(const axPoint& pos);
    void OnTimerAnimation(const axTimerMsg& msg);
    void OnClickAnimation(const axTimerMsg& msg);
    void OnReleaseAnimation(const axTimerMsg& msg);
    
    virtual void OnMouseEnter();
    virtual void OnMouseLeave();
    
    axPoint _mousePosition;
    int _nParticules;
    axTimer* _animationTimer, *_clickTimer, *_releaseTimer;
    
    double _radius;
};

#endif /* defined(__axDemo__AnimationPanel__) */
