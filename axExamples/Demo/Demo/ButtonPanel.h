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
    ButtonPanel(axApp* app,
                axWindow* parent,
                const axRect& rect);
    
    
private:
    virtual void OnPaint();
};

#endif /* defined(__Demo__ButtonPanel__) */
