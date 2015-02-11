//
//  MasterModuleGUI.h
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-05.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __DrumNBass__MasterModuleGUI__
#define __DrumNBass__MasterModuleGUI__

#include "axLib.h"

class MasterModuleGUI : public axPanel
{
public:
    MasterModuleGUI(axWindow* parent, const axPoint& pos);
    
private:
    
    // axEvents.
    void OnPaint();
};

#endif /* defined(__DrumNBass__MasterModuleGUI__) */
