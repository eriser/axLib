//
//  PianoRoll.cpp
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-08.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "PianoRoll.h"

PianoRoll::PianoRoll(axWindow* parent,
                     const axRect& rect,
                     const PianoNote::Events& events):
axPanel(parent, rect)
{
    int note_height = 10;
    axRect drawRect(GetDrawingRect());
    
    for(int i = 0; i * note_height < drawRect.size.y; i++)
    {
        bool black_note = false;
        
        int i_mod_12 = i % 12;
        switch(i_mod_12)
        {
            case 0:
            case 2:
            case 4:
            case 5:
            case 7:
            case 9:
            case 11:

                break;
                
            default:
                black_note = true;
        }
        axRect noteRect(0, i * note_height, rect.size.x, note_height);
        _notes.push_back(new PianoNote(this, noteRect, events, i, black_note));
    }
}

void PianoRoll::OnResize()
{
    for(int i = 1; i < _notes.size(); i++)
    {
        _notes[i]->SetPosition(_notes[i-1]->GetNextPosDown(0));
    }
}

//void PianoRoll::OnNoteChangeSize(const PianoNote::Msg& msg)
//{
//    std::cout << "Note click." << std::endl;
//}

void PianoRoll::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetDrawingRect());
    
    gc->SetColor(axColor(0.8, 0.8, 0.8), 1.0);
    gc->DrawRectangle(rect);
    
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    gc->DrawRectangleContour(rect);
}