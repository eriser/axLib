#include "ModuleDistorsion.h"

/*******************************************************************************
 * ModuleDistorsion.
 ******************************************************************************/
ModuleDistorsion::ModuleDistorsion(axWindow* parent, const axRect& rect):
axPanel(parent, rect)
{
    std::string app_path = axApp::GetAppPath();
    axObjectLoader loader(this, app_path + "ressources/objects/Distorsion.xml");
}

/*******************************************************************************
 * Distorsions.
 ******************************************************************************/
Distorsions::Distorsions(axWindow* parent, const axPoint& pos):
ModuleFrame<ModuleDistorsion>(parent, axRect(pos, axSize(165, 55 + 17)))
{
    std::string app_path = axApp::GetInstance()->GetAppDirectory();
    app_path += std::string("ressources/");
    
    axDropMenuInfo drop_info;
    drop_info.normal = axColor(0.6, 0.6, 0.6, 0.0);
    drop_info.hover = axColor(0.65, 0.65, 0.65, 0.1);
    drop_info.clicking = axColor(0.5, 0.5, 0.5, 0.0);
    drop_info.contour = axColor(0.0, 0.0, 0.0, 0.0);
    drop_info.selected = drop_info.normal;
    drop_info.font_color = axColor(0.5, 0.5, 0.5);
    
    std::vector<std::string> menu_str = {"DISTORSION 1", "DISTORSION 2", "DISTORSION 3", "DISTORSION 4", "DISTORSION 5"};
    _moduleMenu = new axDropMenu(this, axRect(4, 1, 120, 15),
                                 axDropMenuEvents(GetOnModuleChange()),
                                 drop_info,
                                 app_path + std::string("images/DropMenuArrow.png"),
                                 "",
                                 "",
                                 menu_str,
                                 axDROP_MENU_SINGLE_IMG);
    
    AddModule(new ModuleDistorsion(this, axRect(0, 17, 158, 65)), "DISTORSION 1");
    AddModule(new ModuleDistorsion(this, axRect(0, 17, 158, 65)), "DISTORSION 2");
    AddModule(new ModuleDistorsion(this, axRect(0, 17, 158, 65)), "DISTORSION 3");
    AddModule(new ModuleDistorsion(this, axRect(0, 17, 158, 65)), "DISTORSION 4");
    AddModule(new ModuleDistorsion(this, axRect(0, 17, 158, 65)), "DISTORSION 5");
}