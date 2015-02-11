//
//  MasterModuleGUI.cpp
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-05.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "MasterModuleGUI.h"
#include "DrumNBaseConstant.h"

MasterModuleGUI::MasterModuleGUI(axWindow* parent, const axPoint& pos):
axPanel(parent, axRect(pos,
                       axSize(DNBConstant::scrollPanelFixRect.size.x, 70)))
{
    std::string app_path(axApp::GetAppPath());
}

void MasterModuleGUI::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetDrawingRect());
    
//    gc->SetColor(0.5);
//    gc->DrawRectangle(rect);
    
    gc->DrawRectangleColorFade(rect, axColor(0.5), axColor(0.6));
    
    axFont font(0);
    gc->SetColor(0.0, 0.0, 0.0);
    gc->DrawString(font, "Audio Interface", axPoint(10, 10));
    
    gc->SetColor(0.7, 0.7, 0.7);
    gc->DrawString(font, "CoreAudio", axPoint(100, 10));
    gc->SetColor(0.0, 0.0, 0.0);
    
    gc->SetColor(0.0, 0.0, 0.0);
    gc->DrawString(font, "Midi Interface", axPoint(10, 30));
    
    gc->SetColor(0.7, 0.7, 0.7);
    gc->DrawString(font, "CoreMidi", axPoint(100, 30));
    gc->SetColor(0.0, 0.0, 0.0);
    
    gc->DrawRectangleContour(rect);
}