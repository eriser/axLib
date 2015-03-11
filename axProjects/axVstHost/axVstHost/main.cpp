//
//  main.cpp
//  axVstHost
//
//  Created by Alexandre Arsenault on 2015-03-05.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "main.h"
#include <axLib/axLib.h>
#include <axAudio/axAudio.h>

#include "AudioPlayer.h"
#include "axVstInterface.h"


int main()
{
    axApp* app = axApp::CreateApp(axSize(500, 610));
    
    ax::Audio::Core* audio = new ax::Audio::Core();
    
    app->GetResourceManager()->Add("Audio", audio);
    
    app->AddMainEntry([]()
    {
        axPanel* panel = new axPanel(nullptr,
                                     axRect(0, 0, 655, 600));
        panel->SetWindowColor(axColor(0.4));
                          
        AudioPlayer* player = new AudioPlayer(panel, axRect(0, 0, 655, 180));
        
    });
    
    app->AddAfterGUILoadFunction([audio]()
    {
        audio->InitAudio();
        audio->StartAudio();
        
//        axVstInterface* vst = new axVstInterface(44100, 1024);
//        vst->LoadVstPlugin("/Users/alexarse/Project/axLib/axProjects/axVstHost/Build/Debug/A1StereoControl.vst/Contents/MacOS/A1StereoControl");
        //vst->startPlugin();
        //vst->suspendPlugin();
        
//        axPrint("Vst done loading.");

    });
    
    app->MainLoop();
    
    return 0;
}