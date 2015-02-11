//
//  LineSelection.cpp
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-05.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "LineSelection.h"

LineSelection::LineSelection(axWindow* parent, const axRect& rect):
axPanel(parent, rect)
{
    _font = new axFont(0);
    _font->SetFontSize(10);
}


void LineSelection::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetRect());
    axRect rect0(axPoint(0, 0), rect.size);
    
    gc->SetColor(axColor(0.1, 0.1, 0.1), 1.0);
    gc->DrawRectangle(rect0);
    
    gc->SetColor(axColor(0.6, 0.6, 0.6));
    //   gc->SetFontSize(10);
    // gc->DrawLine(axPoint(1, rect0.size.y), axPoint(1, rect0.size.y - 10));
    
    for(int i = 0; i < 16; i++)
    {
//        int x_pos = ((i) / double(16)) * (rect0.size.x - 14.0);
        int x_pos = ((i) / double(16)) * (rect0.size.x);
        
        // gc->SetColor(axColor(0.6, 0.6, 0.6), 1.0);
        if(i % 4 == 0)
        {
            gc->DrawLine(axPoint(x_pos, rect0.size.y), axPoint(x_pos, rect0.size.y - 8));
        }
        else
        {
            gc->DrawLine(axPoint(x_pos, rect0.size.y), axPoint(x_pos, rect0.size.y - 6));
        }
        
//        int x_pos2 = ((i+1) / double(16)) * (rect0.size.x - 14.0);
        int x_pos2 = ((i+1) / double(16)) * (rect0.size.x);
        
        // gc->DrawString(_trackName, axPoint(0, 0));
        gc->DrawStringAlignedCenter(*_font, to_string(i+1), axRect(x_pos, 5, x_pos2 - x_pos, rect0.size.y - 5));
        
    }
    
    
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    gc->DrawRectangleContour(rect0);
}