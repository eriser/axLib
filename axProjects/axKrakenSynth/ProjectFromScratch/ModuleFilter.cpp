#include "ModuleFilter.h"
#include "KrakenAudio.h"

/*******************************************************************************
 * ModuleFilter.
 ******************************************************************************/
ModuleFilter::ModuleFilter(axWindow* parent,
                           const axRect& rect,
                           const int& index):
axPanel(parent, rect),
_index(index)
{
    axObject::AddEventFunction("OnFreqChange", GetOnFreqChange());
    axObject::AddEventFunction("OnResChange", GetOnResChange());
    axObject::AddEventFunction("OnGainChange", GetOnGainChange());

    std::string app_path = axApp::GetAppPath();
    axObjectLoader loader(this, app_path + "ressources/objects/Filter.xml");
    
    
    _freqKnob = axWindow::GetResourceManager()->GetResource("freq");
    _resKnob = axWindow::GetResourceManager()->GetResource("res");
    _gainKnob = axWindow::GetResourceManager()->GetResource("gain");
}

void ModuleFilter::OnFreqChange(const axKnob::Msg& msg)
{
    double freq = axLineairInterpole<double>(20.0, 20000.0, msg.GetValue());
    KrakenAudio::GetInstance()->SetFilterFreq(_index, freq);
}

void ModuleFilter::OnResChange(const axKnob::Msg& msg)
{
    double res = axLineairInterpole<double>(0.1, 10.0, msg.GetValue());
    KrakenAudio::GetInstance()->SetFilterRes(_index, res);
}

void ModuleFilter::OnGainChange(const axKnob::Msg& msg)
{
    KrakenAudio::GetInstance()->SetFilterGain(_index, msg.GetValue());
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
    
    AddModule(new ModuleFilter(this, axRect(0, 17, 158, 130), 0), "FILTER 1");
    AddModule(new ModuleFilter(this, axRect(0, 17, 158, 130), 1), "FILTER 2");
    AddModule(new ModuleFilter(this, axRect(0, 17, 158, 130), 2), "FILTER 3");
}