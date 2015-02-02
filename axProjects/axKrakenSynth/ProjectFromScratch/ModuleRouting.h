//
//  ModuleRouting.h
//  ProjectFromScratch
//
//  Created by Alexandre Arsenault on 2015-01-06.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __ProjectFromScratch__ModuleRouting__
#define __ProjectFromScratch__ModuleRouting__

#include "axLib.h"

class ModuleRouting: public axPanel
{
public:
    ModuleRouting(axWindow* parent, const axRect& rect);
                  
private:
    // Events.
    virtual void OnPaint();
};

#endif /* defined(__ProjectFromScratch__ModuleRouting__) */
