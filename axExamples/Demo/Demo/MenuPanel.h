//
//  MenuPanel.h
//  Demo
//
//  Created by Alexandre Arsenault on 2014-11-02.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#ifndef __Demo__MenuPanel__
#define __Demo__MenuPanel__

#include "axLib.h"

class MenuPanel : public axPanel
{
public:
    MenuPanel(axWindow* parent,
                const axRect& rect);
    
    axEVENT_ACCESOR(axButtonMsg, OnPopupMenu);
    axEVENT_ACCESOR(axButtonMsg, OnTestBtnUnder);
    axEVENT(axPopupMenuMsg, OnPopupMenuChoice);
private:
    virtual void OnPaint();
    
    void OnPopupMenu(const axButtonMsg& msg);
    void OnPopupMenuChoice(const axPopupMenuMsg& msg);
    
    void OnTestBtnUnder(const axButtonMsg& msg);
    
    axButton* _btnPopMenu;
    axPopupMenu* _popMenu;
};

#endif /* defined(__Demo__MenuPanel__) */
