//
//  axToolBar.cpp
//  MidiSequencer
//
//  Created by Alexandre Arsenault on 2014-11-16.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#include "axToolBar.h"

axToolBar::axToolBar(axApp* app,
                     axWindow* parent,
                     const axRect& rect,
                     const axToolBarEvents& events,
                     const axToolBarInfo& info,
                     axFlag flags) :
// Heritage.
axPanel(app, parent, rect),
// Members.
_events(events),
_info(info),
_flags(flags),
_bgAlpha(1.0)
{
    _currentColor = &_info.bgColorNormal;
}

void axToolBar::OnMouseEnter()
{

}

void axToolBar::OnMouseLeave()
{

}

void axToolBar::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetRect());
    axRect rect0(axPoint(0, 0), rect.size);
    
    gc->SetColor(*_currentColor, _bgAlpha);
    gc->DrawRectangle(rect0);
    
    gc->SetColor(_info.contourColor, _bgAlpha);
    gc->DrawRectangleContour(rect0);
}


