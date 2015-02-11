//
//  main.cpp
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-05.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "main.h"
#include "axLib.h"
#include "DrumNBassGUI.h"

int main()
{
    axApp* app = axApp::CreateApp();
    
    app->AddMainEntry([](){new DrumNBassGUI();});
    
    app->MainLoop();
}