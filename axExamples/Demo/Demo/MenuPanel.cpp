//
//  ButtonPanel.cpp
//  Demo
//
//  Created by Alexandre Arsenault on 2014-11-02.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#include "MenuPanel.h"

MenuPanel::MenuPanel(axWindow* parent,
                         const axRect& rect):
// Parent.
axPanel(parent, rect)
{
    std::string app_path(axApp::GetInstance()->GetAppDirectory());
	//std::string app_path("C:/Users/Alexandre Arsenault/Desktop/axLib/axExamples/Demo/VisualStudio2013/axGL/");
    
    axButtonInfo btn1_info;
    btn1_info.normal = axColor(0.8, 0.8, 0.8);
    btn1_info.hover = axColor(0.9, 0.9, 0.9);
    btn1_info.clicking = axColor(0.7, 0.7, 0.7);
    btn1_info.contour = axColor(0.0, 0.0, 0.0);
    btn1_info.selected = btn1_info.normal;
    
    axButtonEvents btn1_evts;
    btn1_evts.button_click = GetOnPopupMenu();
    
    _btnPopMenu = new axButton(this,
                               axRect(40, 40, 70, 25),
                               btn1_evts,
                               btn1_info,
                               "", "Test1");
    
    axButtonEvents btn2_evts;
    btn2_evts.button_click = GetOnTestBtnUnder();

    axButton* btn2 = new axButton(this,
                                  axRect(_btnPopMenu->GetBottomLeftPosition(),
                                         axSize(70, 25)),
                                  btn2_evts,
                                  btn1_info,
                                  "", "BTN");
  
    
    axPopupMenuInfo menu_info(axColor(1.0, 0.0, 0.0),
                              axColor(0.2, 0.8, 0.8),
                              axColor(0.2, 0.8, 0.8),
                              axColor(0.2, 0.8, 0.8),
                              axColor(0.2, 0.8, 0.8),
                              axColor(0.0, 0.0, 0.0));
    
    axPopupMenuEvents menu_evts;
    menu_evts.selection_change = GetOnPopupMenuChoice();
    
    std::vector<std::string> menu_str = {"Test1", "Test2", "Test3", "Test4"};
    
    axPoint menu_pos = _btnPopMenu->GetAbsoluteRect().position;
    menu_pos.y += _btnPopMenu->GetSize().y;
    _popMenu = new axPopupMenu(this,
                               axRect(menu_pos, axSize(70, 30)),
                               menu_evts, menu_info, menu_str);
    
    _popMenu->Hide();
    _popMenu->SetSelectedIndex(0);
    
    std::cout << "MenuPanel::_popMenu (id) = " << _popMenu->GetId() << std::endl;
}

void MenuPanel::OnPopupMenu(const axButtonMsg& msg)
{
    if(_popMenu->IsShown())
    {
        _popMenu->Hide();
    }
    else
    {
        _popMenu->Show();
    }
    
}

void MenuPanel::OnPopupMenuChoice(const axPopupMenuMsg& msg)
{
    std::cout << "Pop choice : " << msg.GetMsg() << std::endl;
    _popMenu->Hide();
    _btnPopMenu->SetLabel(msg.GetMsg());
    
    Update();
}

void MenuPanel::OnTestBtnUnder(const axButtonMsg& msg)
{
    std::cout << "Btn under" << std::endl;
}

void MenuPanel::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetRect());
    axRect rect0(axPoint(0, 0), rect.size);
    
    gc->SetColor(axColor(0.4, 0.4, 0.4), 1.0);
    gc->DrawRectangle(rect0);
        
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    gc->DrawRectangleContour(rect0);
}