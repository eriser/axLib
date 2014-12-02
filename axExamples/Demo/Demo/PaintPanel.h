//
//  PaintPanel.h
//  Demo
//
//  Created by Alexandre Arsenault on 2014-11-03.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#ifndef __Demo__PaintPanel__
#define __Demo__PaintPanel__

#include "axLib.h"

class PaintPanel : public axPanel
{
public:
    PaintPanel(axWindow* parent,
                const axRect& rect);
    
    
private:
    virtual void OnPaint();
    
    axImage* dog_cat_img;
};

#endif /* defined(__Demo__PaintPanel__) */
