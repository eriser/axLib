//
//  ButtonPanel.h
//  Demo
//
//  Created by Alexandre Arsenault on 2014-11-02.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#ifndef __Demo__ButtonPanel__
#define __Demo__ButtonPanel__

#include "axLib.h"

class ButtonPanel : public axPanel
{
public:
    ButtonPanel(axWindow* parent,
                const axRect& rect);
    
    axEVENT_ACCESOR(axButtonMsg, OnButtonWithEvtManager);
    axEVENT_ACCESOR(axTimerMsg, OnTimerEvent);
    
private:
    virtual void OnPaint();
    
    void OnButtonWithEvtManager(const axButtonMsg& msg);
    
    void OnTimerEvent(const axTimerMsg& msg);
};

#endif /* defined(__Demo__ButtonPanel__) */
