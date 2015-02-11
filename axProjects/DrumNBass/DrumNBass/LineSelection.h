//
//  LineSelection.h
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-05.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __DrumNBass__LineSelection__
#define __DrumNBass__LineSelection__

#include "axLib.h"

class LineSelection: public axPanel
{
public:
    LineSelection(axWindow* parent, const axRect& rect);
    
private:
    
    axFont* _font;
    // Events.
    virtual void OnPaint();
};

#endif /* defined(__DrumNBass__LineSelection__) */
