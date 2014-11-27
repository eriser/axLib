//
//  main.cpp
//  Demo
//
//  Created by Alexandre Arsenault on 2014-11-02.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#include "main.h"
#include "ButtonPanel.h"
#include "SliderPanel.h"
#include "KnobPanel.h"
#include "NumberBoxPanel.h"
#include "PaintPanel.h"

MainPanel::MainPanel(axApp* app, axWindow* parent, const axRect& rect):
// Parent.
axPanel(app, parent, rect)
{
    _tabBtnLabels.push_back("Button");
    _tabBtnLabels.push_back("Slider");
    _tabBtnLabels.push_back("Knob");
    _tabBtnLabels.push_back("NumBox");
    _tabBtnLabels.push_back("Menu");
    _tabBtnLabels.push_back("Text");
    _tabBtnLabels.push_back("Toolbar");
    _tabBtnLabels.push_back("Paint");
    _tabBtnLabels.push_back("Anim");
    _tabBtnLabels.push_back("Custom");
    
    axSize tabBtnSize(55, 25);
    
    // This should't be needed.
    // @todo Find a way to do the automatic cast.
    std::function<void (axButtonMsg)> fct = GetOnTabClick();
    
    // Tab button event.
    axButtonEvents btn_evts(fct);
    
    // Tab buttons info.
    axButtonInfo btn_info(axSTANDARD_BUTTON);
    btn_info.selected = axColor(0.3, 0.8, 0.8);
    
    _tabBtns.push_back(new axButton(app, this,
                                    axRect(axPoint(0, 0), tabBtnSize),
                                    btn_evts,
                                    btn_info,
                                    "",
                                    _tabBtnLabels[0],
                                    0,
                                    "0"));
    
    for(unsigned int i = 1; i < _tabBtnLabels.size(); i++)
    {
        _tabBtns.push_back(new axButton(app, this,
                                     axRect(_tabBtns[i-1]->GetTopRightPosition(),
                                            tabBtnSize),
                                     btn_evts,
                                     btn_info,
                                     "",
                                     _tabBtnLabels[i],
                                        0,
                                        std::to_string(i)));
    }

    _panels.resize(_tabBtnLabels.size(), nullptr);
    axRect panel_rect(1, tabBtnSize.y + 1, 548, 499 - tabBtnSize.y - 2);
    
    _panels[0] = new ButtonPanel(app, this, panel_rect);
    _panels[1] = new SliderPanel(app, this, panel_rect);
    _panels[2] = new KnobPanel(app, this, panel_rect);
    _panels[3] = new NumberBoxPanel(app, this, panel_rect);
    _panels[7] = new PaintPanel(app, this, panel_rect);
    
    _tabBtns[0]->SetSelected(true);
    _panels[0]->Show();
    _panels[1]->Hide();
    _panels[2]->Hide();
    _panels[3]->Hide();
    _panels[7]->Hide();
}

void MainPanel::OnTabClick(const axButtonMsg& msg)
{
    int panel_index = std::stoi(msg.GetMsg());
    
    for(auto& x : _panels)
    {
        if(x != nullptr)
        {
            x->Hide();
        }
    }
    
    if(_panels[panel_index] != nullptr)
    {
        _panels[panel_index]->Show();
    }
    
    for(auto& x : _tabBtns)
    {
        if(x != nullptr)
        {
            x->SetSelected(false);
        }
    }

    _tabBtns[panel_index]->SetSelected(true);
}

void MainPanel::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetRect());
    axRect rect0(axPoint(0, 0), rect.size);
    
    gc->SetColor(axColor(0.4, 0.4, 0.4), 1.0);
    gc->DrawRectangle(rect0);
    
    
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    gc->DrawRectangleContour(rect0);
    
}

axMain::axMain()
{
}

void axMain::MainEntryPoint(axApp* app)
{
    MainPanel* mainPanel = new MainPanel(app, nullptr,
                                            axRect(1, 1, 550, 499));
}

int main(int argc, char* argv[])
{
	axApp app(axSize(550, 499));
	axMain* main = new axMain();
	main->MainEntryPoint(&app);

	app.MainLoop();
	return 0;
}