//
//  NumberBoxPanel.h
//  Demo
//
//  Created by Alexandre Arsenault on 2014-11-03.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#ifndef __Demo__NumberBoxPanel__
#define __Demo__NumberBoxPanel__

#include "axLib.h"

class NumberBoxPanel : public axPanel
{
public:
    NumberBoxPanel(axApp* app,
              axWindow* parent,
              const axRect& rect);
    
    
private:
    virtual void OnPaint();
};

#endif /* defined(__Demo__NumberBoxPanel__) */
