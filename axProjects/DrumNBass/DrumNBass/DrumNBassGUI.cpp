//
//  DrumNBassGUI.cpp
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-05.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "DrumNBassGUI.h"
#include "DrumNBaseConstant.h"
#include "DrumNBassScrollPanel.h"
//#include "MainMidiSequencer.h
#include "MidiPanel.h"


DrumNBassGUI::DrumNBassGUI():
axPanel(nullptr, axRect(axPoint(0, 0), DNBConstant::drumNBassFixAppSize))
{
    std::string app_path(axApp::GetAppPath());
    
    // Woodside image.
    _woodSideImg = new axImage(app_path + "resources/images/woodSide.png");
    
    
    axButton* midiPartBtn = new axButton(this,
                                         axRect(axPoint(DNBConstant::drumNBassFixAppSize.x - 38, 20),
                                                axSize(30, 30)),
                                         axButton::Events(GetOnSynthPartClick()),
                                         axButton::Info(axColor(0.0, 0.8, 0.0, 0.0),
                                                        axColor(0.7, 0.7, 0.7, 0.5),
                                                        axColor(0.8, 0.8, 0.8, 0.5),
                                                        axColor(0.0, 0.0, 0.0, 0.0),
                                                        axColor(0.0, 0.0, 0.0),
                                                        axColor(0.0, 0.0, 0.0)),
                                         "resources/images/synth.png",
                                         "");
    
    axButton* synthPartBtn = new axButton(this,
                                         axRect(axPoint(DNBConstant::drumNBassFixAppSize.x - 38, 60),
                                                axSize(30, 30)),
                                         axButton::Events(GetOnMidiPartClick()),
                                         axButton::Info(axColor(0.0, 0.8, 0.0, 0.0),
                                                        axColor(0.7, 0.7, 0.7, 0.5),
                                                        axColor(0.8, 0.8, 0.8, 0.5),
                                                        axColor(0.0, 0.0, 0.0, 0.0),
                                                        axColor(0.0, 0.0, 0.0),
                                                        axColor(0.0, 0.0, 0.0)),
                                         "resources/images/piano.png",
                                         "");
    
    // Create main scroll panel.
    _mainScrollPanel =
        new DrumNBassScrollPanel(this, DNBConstant::scrollPanelFixRect);
    
    axRect midiRect(DNBConstant::scrollPanelFixRect);
    midiRect.size.x += 8;
    midiRect.size.y -= 50;
    _midiPanel = new MidiPanel(this, midiRect);
    _midiPanel->Hide();
    
    // Create Scroll bar.
    _scrollBar = new axScrollBar(this, _mainScrollPanel,
                                 axRect(DNBConstant::drumNBassFixAppSize.x -
                                        (DNBConstant::scrollBarFixWidth - 1) -
                                        DNBConstant::woodSideFixSize.x, 0,
                                        DNBConstant::scrollBarFixWidth,
                                        DNBConstant::drumNBassFixAppSize.y -
                                        DNBConstant::bottom_bar_height),
                                 axScrollBarEvents(),
                                 axScrollBarInfo(axColor(0.4, 0.4, 0.4),
                                                 axColor(0.45, 0.45, 0.45),
                                                 axColor(0.42, 0.42, 0.42),
                                                 axColor(0.0, 0.0, 0.0),
                                                 axColor(0.0, 0.0, 0.0),
                                                 axColor(0.25, 0.25, 0.25),
                                                 axColor(0.15, 0.15, 0.15)));
    
    // Assign main scroll panel to scroll bar.
    _scrollBar->SetPanelSize(_mainScrollPanel->GetSize());
    
    _mainScrollPanel->OnResize();
    
    
    
    axButton::Info btn_info(axColor(0.9, 0.17, 0.17, 0.0),
                            axColor(0.3, 0.3, 0.3, 0.4),
                            axColor(0.2, 0.2, 0.2, 0.4),
                            axColor(0.2, 0.2, 0.2, 0.0),
                            axColor(0.0, 0.0, 0.0, 1.0),
                            axColor(0.4, 0.4, 0.4));

    axButton* play = new axButton(this,
                                  axRect(GetRect().size.x * 0.5 - 50,
                                         GetRect().size.y - 45,
                                         40, 40),
                                  axButton::Events(),
                                  btn_info,
                                  app_path + "resources/images/play.png",
                                  "");
    
    axButton* stop = new axButton(this,
                                  axRect(play->GetNextPosRight(5),
                                         axSize(40, 40)),
                                  axButton::Events(),
                                  btn_info,
                                  app_path + "resources/images/stop.png",
                                  "");

}

void DrumNBassGUI::OnResize()
{
    _scrollBar->SetPanelSize(_mainScrollPanel->GetSize());
}

void DrumNBassGUI::OnMidiPartClick(const axButton::Msg& msg)
{
    _mainScrollPanel->Hide();
    _scrollBar->Hide();
    _midiPanel->Show();
}

void DrumNBassGUI::OnSynthPartClick(const axButton::Msg& msg)
{
    _midiPanel->Hide();
    _mainScrollPanel->Show();
    _scrollBar->Show();
}

void DrumNBassGUI::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetDrawingRect());
    
    gc->SetColor(axColor(0.8, 0.8, 0.8));
    gc->DrawRectangle(rect);
    
    gc->DrawImage(_woodSideImg, axPoint(0, 0));
    gc->DrawImage(_woodSideImg, axPoint(rect.size.x -
                                        DNBConstant::woodSideFixSize.x, 0));
    
//    gc->SetColor(axColor(0.2));
    gc->DrawRectangleColorFade(axRect(45, rect.size.y -
                             DNBConstant::bottom_bar_height, rect.size.x - 90,
                             DNBConstant::bottom_bar_height),
                                axColor(0.5), axColor(0.6));
    
//    gc->SetLineWidth(2);
    gc->SetColor(axColor::axBlackColor);
    gc->DrawRectangleContour(axRect(45, rect.size.y -
                                    DNBConstant::bottom_bar_height+1,
                                    rect.size.x - 88,
                                    DNBConstant::bottom_bar_height - 1), 2);
    
//    gc->SetLineWidth(1);
    
}