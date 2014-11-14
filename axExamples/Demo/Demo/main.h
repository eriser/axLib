//
//  main.h
//  Demo
//
//  Created by Alexandre Arsenault on 2014-11-02.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#ifndef __Demo__main__
#define __Demo__main__

#include "axLib.h"
//#include "ButtonPanel.h"

class MainPanel : public axPanel
{
public:
    MainPanel(axApp* app, axWindow* parent, const axRect& rect);
    
    axEVENT(axButtonMsg, OnTabClick);
    
private:
    virtual void OnPaint();
    
    void OnTabClick(const axButtonMsg& msg);
    
    std::vector<std::string> _tabBtnLabels;
    std::vector<axButton*> _tabBtns;
    std::vector<axPanel*> _panels;
//    ButtonPanel* btn_panel;
    
};

class axMain
{
public:
    axMain();
    
    static void MainEntryPoint(axApp* app);
};

#endif /* defined(__Demo__main__) */
