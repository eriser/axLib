#include "ModuleFrame.h"

//ModuleFrame::ModuleFrame(axWindow* parent, const axRect& rect):
//axPanel(parent, rect)
//{
//    std::string app_path = axApp::GetInstance()->GetAppDirectory();
////    axNumberBoxInfo box_info(axColor(0.45, 0.45, 0.45, 1.0),
////                             axColor(0.5, 0.5, 0.5, 1.0),
////                             axColor(0.2, 0.2, 0.2, 1.0),
////                             axColor(0.2, 0.2, 0.2, 1.0),
////                             axColor(0.0, 0.0, 0.0, 1.0),
////                             axColor(0.0, 0.0, 0.0, 1.0));
////    
////    axDropMenuInfo drop_info;
////    drop_info.normal = axColor(0.6, 0.6, 0.6);
////    drop_info.hover = axColor(0.65, 0.65, 0.65);
////    drop_info.clicking = axColor(0.5, 0.5, 0.5);
////    drop_info.contour = axColor(0.0, 0.0, 0.0, 0.0);
////    drop_info.selected = drop_info.normal;
////    
////    std::vector<std::string> menu_str = {"OSC 1"};
////    _moduleMenu = new axDropMenu(this, axRect(4, 1, 84, 15),
////                                 axDropMenuEvents(),
////                                 drop_info,
////                                 app_path + std::string("DropMenuArrow.png"),
////                                 "",
////                                 "",
////                                 menu_str,
////                                 axDROP_MENU_SINGLE_IMG);
//}

//void ModuleFrame::AddModule(const std::string& name, const int& index)
//{
//    _nameIndex.insert(std::pair<std::string, int>(name, index));
//}

//int ModuleFrame::GetIndex(const std::string& name)
//{
//    std::map<std::string, int>::iterator it = _nameIndex.find(name);
//    if(it != _nameIndex.end())
//    {
//        return it->second;
//    }
//    
//    return -1;
//}

//void ModuleFrame::OnPaint()
//{
//    axGC* gc = GetGC();
//    axRect rect(axPoint(0, 0), GetRect().size);
//    
//    gc->SetColor(axColor(0.8, 0.4, 0.4), 1.0);
//    gc->DrawRectangle(rect);
//    
//    gc->SetColor(axColor(0.6, 0.6, 0.6), 1.0);
//    gc->DrawRectangle(axRect(0, 0, rect.size.x, 17));
//    
//    gc->SetColor(axColor(0.6, 0.6, 0.6), 1.0);
//    gc->DrawRectangleContour(rect);
//    
//}