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

class MainPanel : public axPanel
{
public:
    MainPanel(axWindow* parent, const axRect& rect);
    
    axEVENT_ACCESSOR(axToggleMsg, OnTabClick);
    
private:
    virtual void OnPaint();
    
    void OnTabClick(const axToggleMsg& msg);
    
    std::vector<std::string> _tabBtnLabels;
    std::vector<axToggle*> _tabBtns;
    std::vector<axPanel*> _panels;
//    ButtonPanel* btn_panel;
    
};


#endif /* defined(__Demo__main__) */
