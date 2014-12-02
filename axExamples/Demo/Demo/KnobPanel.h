//
//  KnobPanel.h
//  Demo
//
//  Created by Alexandre Arsenault on 2014-11-03.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#ifndef __Demo__KnobPanel__
#define __Demo__KnobPanel__

#include "axLib.h"

class KnobPanel : public axPanel
{
public:
    KnobPanel(axWindow* parent,
                const axRect& rect);
    
    
private:
    virtual void OnPaint();
};

#endif /* defined(__Demo__KnobPanel__) */
