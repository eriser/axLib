#include "ModuleEnv.h"

/*******************************************************************************
 * ModuleEnv.
 ******************************************************************************/
ModuleEnv::ModuleEnv(axWindow* parent, const axRect& rect):
axPanel(parent, rect)
{
    std::string app_path = axApp::GetInstance()->GetAppDirectory();
    axObjectLoader loader(this, app_path + "ressources/objects/Envelope.xml");
}

/*******************************************************************************
 * Envelopes.
 ******************************************************************************/
Envelopes::Envelopes(axWindow* parent, const axPoint& pos):
ModuleFrame<ModuleEnv>(parent, axRect(pos, axSize(208, 135 + 17)))
{
    std::string app_path = axApp::GetInstance()->GetAppDirectory();
    app_path += std::string("ressources/");
    
    axNumberBox::Info box_info(axColor(0.45, 0.45, 0.45, 1.0),
                               axColor(0.5, 0.5, 0.5, 1.0),
                               axColor(0.2, 0.2, 0.2, 1.0),
                               axColor(0.2, 0.2, 0.2, 1.0),
                               axColor(0.0, 0.0, 0.0, 1.0),
                               axColor(0.0, 0.0, 0.0, 1.0));
    
    axDropMenuInfo drop_info;
    drop_info.normal = axColor(0.6, 0.6, 0.6, 0.0);
    drop_info.hover = axColor(0.65, 0.65, 0.65, 0.1);
    drop_info.clicking = axColor(0.5, 0.5, 0.5, 0.0);
    drop_info.contour = axColor(0.0, 0.0, 0.0, 0.0);
    drop_info.selected = drop_info.normal;
    drop_info.font_color = axColor(0.5, 0.5, 0.5);
    
    std::vector<std::string> menu_str = {"ENV 1", "ENV 2", "ENV 3"};
    _moduleMenu = new axDropMenu(this, axRect(4, 1, 84, 15),
                                 axDropMenuEvents(GetOnModuleChange()),
                                 drop_info,
                                 app_path + std::string("images/DropMenuArrow.png"),
                                 "",
                                 "",
                                 menu_str,
                                 axDROP_MENU_SINGLE_IMG);
    
    AddModule(new ModuleEnv(this, axRect(0, 17, 208, 130)), "ENV 1");
    AddModule(new ModuleEnv(this, axRect(0, 17, 208, 130)), "ENV 2");
    AddModule(new ModuleEnv(this, axRect(0, 17, 208, 130)), "ENV 3");
}