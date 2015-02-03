#include "ModuleRouting.h"
#include "axObjectLoader.h"

ModuleRouting::ModuleRouting(axWindow* parent, const axRect& rect):
axPanel(parent, rect)
{
    std::string objPath(axApp::GetAppPath() + "ressources/objects/Routing.xml");
    axObjectLoader loader(this, objPath);
    
    axDropMenuInfo drop_info;
    drop_info.normal = axColor(0.6, 0.6, 0.6, 0.0);
    drop_info.hover = axColor(0.65, 0.65, 0.65, 0.1);
    drop_info.clicking = axColor(0.5, 0.5, 0.5, 0.0);
    drop_info.contour = axColor(0.0, 0.0, 0.0, 0.0);
    drop_info.selected = drop_info.normal;
    drop_info.font_color = axColor(0.5, 0.5, 0.5);
    
    std::vector<std::string> menu_str = {"None", "None", "None"};
    
    axSize srcSize(37, 15);
    axDropMenu* menu = new axDropMenu(this, axRect(axPoint(3, 22), srcSize),
                                 axDropMenuEvents(),
                                 drop_info,
                                 "",
                                 "",
                                 "",
                                 menu_str,
                                 axDROP_MENU_SINGLE_IMG);
    
    
    
    axDropMenu* menu2 = new axDropMenu(this, axRect(menu->GetNextPosDown(6),
                                                    srcSize),
                                      axDropMenuEvents(),
                                      drop_info,
                                      "",
                                      "",
                                      "",
                                      menu_str,
                                      axDROP_MENU_SINGLE_IMG);
    
    axDropMenu* menu3 = new axDropMenu(this, axRect(menu2->GetNextPosDown(6),
                                                    srcSize),
                                       axDropMenuEvents(),
                                       drop_info,
                                       "",
                                       "",
                                       "",
                                       menu_str,
                                       axDROP_MENU_SINGLE_IMG);
    
    axDropMenu* menu4 = new axDropMenu(this, axRect(menu3->GetNextPosDown(6),
                                                    srcSize),
                                       axDropMenuEvents(),
                                       drop_info,
                                       "",
                                       "",
                                       "",
                                       menu_str,
                                       axDROP_MENU_SINGLE_IMG);
    
    axDropMenu* menu5 = new axDropMenu(this, axRect(axPoint(278, 22), srcSize),
                                      axDropMenuEvents(),
                                      drop_info,
                                      "",
                                      "",
                                      "",
                                      menu_str,
                                      axDROP_MENU_SINGLE_IMG);
    
    
    
    axDropMenu* menu6 = new axDropMenu(this, axRect(menu5->GetNextPosDown(6),
                                                    srcSize),
                                       axDropMenuEvents(),
                                       drop_info,
                                       "",
                                       "",
                                       "",
                                       menu_str,
                                       axDROP_MENU_SINGLE_IMG);
    
    axDropMenu* menu7 = new axDropMenu(this, axRect(menu6->GetNextPosDown(6),
                                                    srcSize),
                                       axDropMenuEvents(),
                                       drop_info,
                                       "",
                                       "",
                                       "",
                                       menu_str,
                                       axDROP_MENU_SINGLE_IMG);
    
    axDropMenu* menu8 = new axDropMenu(this, axRect(menu7->GetNextPosDown(6),
                                                    srcSize),
                                       axDropMenuEvents(),
                                       drop_info,
                                       "",
                                       "",
                                       "",
                                       menu_str,
                                       axDROP_MENU_SINGLE_IMG);
}

void ModuleRouting::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect0(GetDrawingRect());
    
    gc->SetColor(axColor(0.15, 0.15, 0.15), 1.0);
    gc->DrawRectangle(axRect(0, 0, rect0.size.x, 17));
    
    gc->SetColor(axColor(0.0, 0.0, 0.0));
    int middleX = rect0.size.x * 0.5;
    gc->DrawLine(axPoint(middleX, 0), axPoint(middleX, rect0.size.y));
    
    gc->SetColor(axColor(0.6, 0.6, 0.6), 1.0);
    gc->DrawRectangleContour(rect0);
}