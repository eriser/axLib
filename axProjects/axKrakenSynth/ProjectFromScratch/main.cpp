#include "main.h"
#include <random>
#include "axVar.h"

#include "KrakenGUI.h"
#include "KrakenMidi.h"
#include "KrakenAudio.h"

#include "axObjectLoader.h"
/*******************************************************************************
 * MyLED.
 ******************************************************************************/
MyLED::MyLED(axWindow* parent,
             const axRect& rect) :
axPanel(parent, rect)
{
    
    _ledImg = new axImage(axApp::GetInstance()->GetAppDirectory() +
                          std::string("ressources/images/axLED_9x9.png"));
    _imgIndex = 0;
}

void MyLED::SetActive(const bool& on)
{
    if(on)
    {
        _imgIndex = 1;
    }
    else
    {
        _imgIndex = 0;
    }
    
    Update();
}

void MyLED::SetOff()
{
    _imgIndex = 0;
    Update();
}

void MyLED::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect = axRect(axPoint(0, 0), GetRect().size);
    

    gc->DrawPartOfImage(_ledImg,
                        axPoint(0, _imgIndex * 9),
                        axSize(9, 9),
                        axPoint(0, 0));
}

/*******************************************************************************
 * MyNumberPanel.
 ******************************************************************************/
MyNumberPanel::MyNumberPanel(axWindow* parent,
                             const axPoint& pos) :
axPanel(parent, axRect(pos, axSize(28, 15))),
_number(1)
{
    _font = new axFont(1);
    _font->SetFontSize(16);
}

void MyNumberPanel::SetNumber(const int& num)
{
    _number = axClamp<int>(num, 0, 99);
    Update();
}

int MyNumberPanel::GetNumber() const
{
    return _number;
}

void MyNumberPanel::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect = axRect(axPoint(0, 0), GetRect().size);
    
    gc->SetColor(axColor(0.3, 0.0, 0.0, 1.0));
    gc->DrawRectangle(rect);
    
    gc->SetColor(axColor(0.4, 0.0, 0.0, 1.0));
//    gc->SetFontType(std::string("digital-7 (mono).ttf"));
//    gc->SetFontSize(16);
    gc->DrawChar(*_font, '0', axPoint(5, -4));
    gc->DrawChar(*_font, '0', axPoint(13, -4));
    
    
    gc->SetColor(axColor(0.95, 0.0, 0.0, 1.0));
    
    if(_number > 9)
    {
        gc->DrawChar(*_font, std::to_string(_number)[0], axPoint(5, -4));

        gc->DrawChar(*_font, std::to_string(_number)[1], axPoint(13, -4));
    }
    else
    {
        gc->DrawChar(*_font, std::to_string(_number)[0], axPoint(13, -4));
    }
    
    gc->SetColor(axColor(0.4, 0.0, 0.0, 1.0));
    gc->DrawRectangleContour(rect.GetInteriorRect(axPoint(1, 1)));
    
    gc->SetColor(axColor(0.0, 0.0, 0.0, 1.0));
    gc->DrawRectangleContour(rect);
    
}

//void axMain::MainEntryPoint(axApp* app)
//{
//    
//    
//    KrakenAudio* audio = KrakenAudio::GetInstance();
//    audio->InitAudio();
//    
//    KrakenMidi* midi = new KrakenMidi();
//    KrakenGUI* gui = new KrakenGUI(nullptr, axRect(0, 0, 655, 600));
//    
//    
//    audio->StartAudio();
//}

//void axMain::MainEntryPoint(axApp* app)
//{
//
//}

int main()
{
    axEventManager::GetInstance();
    axApp* app = axApp::CreateApp();
	
    app->AddMainEntry([]()
    {
        KrakenGUI* gui = new KrakenGUI(nullptr, axRect(0, 0, 655, 600));
    });
    
    // Init audio.
    KrakenAudio* audio = KrakenAudio::GetInstance();
    audio->InitAudio();
    
    app->AddAfterGUILoadFunction([&]()
    {
        audio->StartAudio();
    });
    
    // Init Midi.
    KrakenMidi* midi = new KrakenMidi();
    
	app->MainLoop();

	return 0;
}



