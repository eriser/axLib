//
//  DrumInstrumentGUI.cpp
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-05.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "DrumInstrumentGUI.h"
#include "DrumMidiSequencer.h"

DrumInstrumentGUI::DrumInstrumentGUI(axWindow* parent,
                                     const axRect& rect):
// Heritage.
axPanel(parent, rect),
InstrumentGUI()
{
    std::string app_path(axApp::GetAppPath());
    
    _drumSampler = new DrumSampler(this,
                                   axRect(0, 0, rect.size.x, 318));
    
//    _drumMidiSequencer = new DrumMidiSequencer(this,
//                                               axRect(_drumSampler->GetBottomLeftPosition() + axPoint(8, 0),
//                                                      axSize(rect.size.x - 15, 300)));
    
    
//    axRect drumSamplerRect(_drumMidiSequencer->GetBottomLeftPosition() +
//                           axPoint(0, 0),
//                           axSize(rect.size.x, 330));
    

    
//    _drumMidiSequencer = new DrumMidiSequencer(this,
//                                               axRect(0, 0, rect.size.x, 300));
//    
//    
//    axRect drumSamplerRect(_drumMidiSequencer->GetBottomLeftPosition() +
//                           axPoint(0, 0),
//                           axSize(rect.size.x, 330));
//    
//    _drumSampler = new DrumSampler(this,
//                                   drumSamplerRect);
    
//    OnResize();

}

void DrumInstrumentGUI::OnResize()
{
    SetSize(axSize(GetSize().x, _drumSampler->GetSize().y + 5));
    
//    _drumSampler->SetPosition(_drumMidiSequencer->GetBottomLeftPosition());
//    _drumMidiSequencer->SetPosition(_drumSampler->GetBottomLeftPosition() +
//                                    axPoint(8, 0));
    GetParent()->OnResize();
}

void DrumInstrumentGUI::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetDrawingRect());

    gc->SetColor(axColor(0.2));
    gc->DrawRectangle(rect);
    
//    gc->SetColor(axColor::axBlackColor);
//    gc->DrawRectangleContour(rect);
}