//
//  KrakenSynthInstrumentGUI.cpp
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-05.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "KrakenSynthInstrumentGUI.h"
#include "ModuleOsc.h"
#include "ModuleLfo.h"
#include "ModuleFrame.h"
#include "ModuleAudioFile.h"
#include "ModuleFilter.h"
#include "ModuleEnv.h"
#include "ModuleRouting.h"
#include "ModuleDistorsion.h"

KrakenSynthInstrumentGUI::KrakenSynthInstrumentGUI(axWindow* parent,
                                                   const axRect& rect):
// Heritage.
axPanel(parent, rect),
InstrumentGUI()
{
    std::string app_path(axApp::GetAppPath());
    
    Oscillators* oscs = new Oscillators(this, axPoint(9, 30));
    AudioFiles* waves = new AudioFiles(this, oscs->GetNextPosRight(8));
    Lfos* lfos = new Lfos(this, oscs->GetNextPosDown(8));
    Filters* filters = new Filters(this, axPoint(lfos->GetNextPosDown(8)));
    Envelopes* envs = new Envelopes(this, filters->GetNextPosRight(8));
    
    Distorsions* dist = new Distorsions(this, axPoint(envs->GetNextPosRight(8)));
    Distorsions* dist2 = new Distorsions(this, axPoint(dist->GetNextPosDown(8)));
    
    axRect routRect(filters->GetNextPosDown(8), axSize(547, 106));
    ModuleRouting* routing = new ModuleRouting(this, routRect);

}

void KrakenSynthInstrumentGUI::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetDrawingRect());
    
    gc->SetColor(axColor(0.15));
    gc->DrawRectangle(rect);
    
    gc->DrawRectangleColorFade(axRect(0, 0, rect.size.x, 20),
                               axColor(0.15, 0.15, 0.15),
                               axColor(0.1, 0.1, 0.1));
    
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    gc->DrawRectangleContour(axRect(0, 0, rect.size.x, 20));
    
    gc->SetColor(axColor(0.65, 0.65, 0.65));
    axFont font(0);
    font.SetFontSize(10);
    gc->DrawString(font, "KRAKEN SYNTH", axPoint(10, 2));
    
    
    
//    gc->SetColor(axColor(0.1));
//    gc->DrawRectangle(axRect(8, 20, rect.size.x - 8, rect.size.y - 25));
    
    gc->SetColor(axColor::axBlackColor);
    gc->DrawRectangleContour(rect);
}