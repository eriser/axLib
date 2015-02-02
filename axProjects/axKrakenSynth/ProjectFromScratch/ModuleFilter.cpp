#include "ModuleFilter.h"

/*******************************************************************************
 * ModuleFilter.
 ******************************************************************************/
ModuleFilter::ModuleFilter(axWindow* parent, const axRect& rect):
axPanel(parent, rect)
{
    std::string app_path = axApp::GetAppPath();
    axObjectLoader loader(this, app_path + "ressources/objects/Filter.xml");
}

/*******************************************************************************
 * Filters.
 ******************************************************************************/
Filters::Filters(axWindow* parent, const axPoint& pos):
ModuleFrame<ModuleFilter>(parent, axRect(pos, axSize(158, 135 + 17)))
{
    std::string app_path = axApp::GetInstance()->GetAppDirectory();
    app_path += "ressources/";
    
    axDropMenuInfo drop_info;
    drop_info.normal = axColor(0.6, 0.6, 0.6, 0.0);
    drop_info.hover = axColor(0.65, 0.65, 0.65, 0.1);
    drop_info.clicking = axColor(0.5, 0.5, 0.5, 0.0);
    drop_info.contour = axColor(0.0, 0.0, 0.0, 0.0);
    drop_info.selected = drop_info.normal;
    drop_info.font_color = axColor(0.5, 0.5, 0.5);
    
    std::vector<std::string> menu_str = {"FILTER 1", "FILTER 2", "FILTER 3"};
    _moduleMenu = new axDropMenu(this, axRect(4, 1, 84, 15),
                                 axDropMenuEvents(GetOnModuleChange()),
                                 drop_info,
                                 app_path + std::string("images/DropMenuArrow.png"),
                                 "",
                                 "",
                                 menu_str,
                                 axDROP_MENU_SINGLE_IMG);
    
    AddModule(new ModuleFilter(this, axRect(0, 17, 158, 130)), "FILTER 1");
    AddModule(new ModuleFilter(this, axRect(0, 17, 158, 130)), "FILTER 2");
    AddModule(new ModuleFilter(this, axRect(0, 17, 158, 130)), "FILTER 3");
}