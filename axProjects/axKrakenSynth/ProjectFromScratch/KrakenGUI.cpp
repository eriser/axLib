#include "KrakenGUI.h"
#include "ModuleOsc.h"
#include "ModuleLfo.h"
#include "ModuleFrame.h"
#include "ModuleAudioFile.h"
#include "ModuleFilter.h"
#include "ModuleEnv.h"
#include "ModuleRouting.h"
#include "ModuleDistorsion.h"

/*******************************************************************************
 * KrakenGUI.
 ******************************************************************************/
KrakenGUI::KrakenGUI(axWindow* parent, const axRect& rect):
axPanel(parent, rect)
{
    std::string app_path = axApp::GetInstance()->GetAppDirectory();

    Oscillators* oscs = new Oscillators(this, axPoint(55, 50));
    AudioFiles* waves = new AudioFiles(this, oscs->GetNextPosRight(8));
    Lfos* lfos = new Lfos(this, oscs->GetNextPosDown(8));
    Filters* filters = new Filters(this, axPoint(lfos->GetNextPosDown(8)));
    Envelopes* envs = new Envelopes(this, filters->GetNextPosRight(8));
    
    Distorsions* dist = new Distorsions(this, axPoint(envs->GetNextPosRight(8)));
    Distorsions* dist2 = new Distorsions(this, axPoint(dist->GetNextPosDown(8)));
    
    axRect routRect(filters->GetNextPosDown(8), axSize(547, 106));
    ModuleRouting* routing = new ModuleRouting(this, routRect);
    _woodside = new axImage(app_path + "ressources/images/woodSide.png");
}

void KrakenGUI::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect0(axPoint(0, 0), GetRect().size);
    
    gc->SetColor(axColor(0.1, 0.1, 0.1), 1.0);
    gc->DrawRectangle(rect0);
    
    gc->DrawImage(_woodside, axPoint(0, 0));
    gc->DrawImage(_woodside, axPoint(GetRect().size.x - _woodside->GetWidth(), 0));
    
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    gc->DrawRectangleContour(rect0);
    
}