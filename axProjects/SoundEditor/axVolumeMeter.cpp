//
//  axVolumeMeter.cpp
//  MidiSequencer
//
//  Created by Alexandre Arsenault on 2014-11-17.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#include "axVolumeMeter.h"

axVolumeMeter::axVolumeMeter(axApp* app,
                     axWindow* parent,
                     const axRect& rect,
//                     const axToolBarEvents& events,
                     const axVolumeMeterInfo& info,
                     axFlag flags) :
// Heritage.
axPanel(app, parent, rect),
// Members.
//_events(events),
_info(info),
_flags(flags),
_bgAlpha(1.0)
{
    _value = 0.0;
    _currentColor = &_info.bgColorNormal;
}


void axVolumeMeter::SetValue(const double& value)
{
    _value = value;
}

void axVolumeMeter::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetRect());
    axRect rect0(axPoint(0, 0), rect.size);
    
    gc->SetColor(*_currentColor, _bgAlpha);
    gc->DrawRectangle(rect0);
    
    double nSeparation = 25.0;
    

    for(int i = 0; i < int(nSeparation); i++)
    {
        if(i > (1.0 - _value) * nSeparation)
        {
            // Red part.
            if(i < 7)
            {
                gc->SetColor(axColor(1.0 - (i-5.0) / 7.0, i / 7.0, 0.0, 0.6));
                gc->DrawRectangle(axRect(axPoint(1, i * rect.size.y / nSeparation), axSize(rect.size.x - 2, rect.size.y / nSeparation)));
            }
            // Green part.
            else
            {
                gc->SetColor(axColor(0.0, 1.0, 0.0, 0.6));
                gc->DrawRectangle(axRect(axPoint(1, i * rect.size.y / nSeparation), axSize(rect.size.x - 2, rect.size.y / nSeparation)));
            }
        }
    }
    
    gc->SetColor(_info.contourColor, _bgAlpha);
    gc->DrawRectangleContour(rect0);
}