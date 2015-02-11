//
//  DrumNBaseConstant.cpp
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-05.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "DrumNBaseConstant.h"

namespace DNBConstant
{
    const axSize drumNBassFixAppSize = axSize(663, 686);
    const int scrollBarFixWidth = 8;
    const axSize woodSideFixSize = axSize(45, 686);
    
    const axRect scrollPanelFixRect = axRect(woodSideFixSize.x, 0,
                                             drumNBassFixAppSize.x - 2
                                             * woodSideFixSize.x -
                                             (scrollBarFixWidth - 1),
                                             drumNBassFixAppSize.y);
    
    const int bottom_bar_height = 50;
}
