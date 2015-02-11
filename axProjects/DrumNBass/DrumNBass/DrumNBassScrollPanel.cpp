//
//  DrumNBassScrollPanel.cpp
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-05.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "DrumNBassScrollPanel.h"
#include "MasterModuleGUI.h"
#include "DrumInstrumentGUI.h"
#include "Tb303GUI.h"
#include "KrakenSynthInstrumentGUI.h"
#include "DrumNBaseConstant.h"

DrumNBassScrollPanel::DrumNBassScrollPanel(axWindow* parent,
                                           const axRect& rect):
axPanel(parent, rect)
{
    SetBlockDrawing(true);
    
    std::string app_path(axApp::GetAppPath());
    
    _masterModule = new MasterModuleGUI(this, axPoint(0, 1));
    
    _drumInstrument = new DrumInstrumentGUI(this,
                              axRect(_masterModule->GetNextPosDown(0),
                                 axSize(rect.size.x, 313)));
    
    _tb303GUI = new MyProject(this, axRect(_drumInstrument->GetNextPosDown(0),
                                           axSize(rect.size.x, 273)));
    
    
    _krakenSynth =
    new KrakenSynthInstrumentGUI(this, axRect(_tb303GUI->GetNextPosDown(0),
                                              axSize(rect.size.x, 540)));
    
//    OnResize();
    
//    SetShownRect(axRect(0, 0, DNBConstant::scrollPanelFixRect.size.x,
//                        300));
}

void DrumNBassScrollPanel::OnResize()
{
    SetSize(axSize(GetSize().x,
                   _masterModule->GetSize().y + _drumInstrument->GetSize().y +
                   _tb303GUI->GetSize().y + _krakenSynth->GetSize().y));
    
    _tb303GUI->SetPosition(_drumInstrument->GetNextPosDown(0));
    _krakenSynth->SetPosition(_tb303GUI->GetNextPosDown(0));
    
    
//    std::cout << GetShownRect().position.x << " " <<
//    GetShownRect().position.y << " " <<
//    GetShownRect().size.x << " " <<
//    GetShownRect().size.x << " " << endl;
    
    SetShownRect(axRect(0, 0, DNBConstant::scrollPanelFixRect.size.x,
                        DNBConstant::drumNBassFixAppSize.y -
                        DNBConstant::bottom_bar_height));
    
//    SetShownRect(axRect(0, 0, DNBConstant::scrollPanelFixRect.size.x,
//                        300));
    
//    std::cout << GetShownRect().position.x << " " <<
//    GetShownRect().position.y << " " <<
//    GetShownRect().size.x << " " <<
//    GetShownRect().size.y << " " << endl;
    
    GetParent()->OnResize();
}

void DrumNBassScrollPanel::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetDrawingRect());

//    gc->DrawRectangleColorFade(rect,
//                               axColor(0.15, 0.15, 0.15),
//                               axColor(0.1, 0.1, 0.1));
    
    gc->SetColor(axColor(1.0, 0.0, 0.0));
    gc->DrawRectangle(rect);
    
    gc->SetColor(axColor(0.0, 0.0, 0.0));
    gc->DrawRectangleContour(rect);
}