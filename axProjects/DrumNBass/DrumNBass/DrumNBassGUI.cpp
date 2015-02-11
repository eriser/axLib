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
#include "MainMidiSequencer.h"

//const axSize DrumNBassGUI::drumNBassFixAppSize = axSize(655, 686);
//const int DrumNBassGUI::scrollBarFixWidth = 8;
//const axSize DrumNBassGUI::woodSideFixSize = axSize(45, 686);
//const axRect DrumNBassGUI::scrollPanelFixRect = axRect(woodSideFixSize.x, 0,
//                                                       drumNBassFixAppSize.x - 2
//                                                       * woodSideFixSize.x -
//                                                       (scrollBarFixWidth - 1),
//                                                       drumNBassFixAppSize.y);

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
    
    axRect mainMidiRect(DNBConstant::scrollPanelFixRect);
    mainMidiRect.size.y -= 50;
    _mainMidiSequencer = new MainMidiSequencer(this,
                                               mainMidiRect);
    
    _mainMidiSequencer->Hide();
    
    
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
}

void DrumNBassGUI::OnResize()
{
    _scrollBar->SetPanelSize(_mainScrollPanel->GetSize());
}

void DrumNBassGUI::OnMidiPartClick(const axButton::Msg& msg)
{
    _mainScrollPanel->Hide();
    _mainMidiSequencer->Show();
}

void DrumNBassGUI::OnSynthPartClick(const axButton::Msg& msg)
{
    _mainMidiSequencer->Hide();
    _mainScrollPanel->Show();
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
    
    gc->SetColor(axColor(0.6));
    gc->DrawRectangle(axRect(45, rect.size.y -
                             DNBConstant::bottom_bar_height, rect.size.x - 90,
                             DNBConstant::bottom_bar_height));
    
    gc->SetColor(axColor::axBlackColor);
    gc->DrawRectangleContour(axRect(45, rect.size.y -
                                    DNBConstant::bottom_bar_height+1,
                                    rect.size.x - 89,
                                    DNBConstant::bottom_bar_height - 1));
                                        
}