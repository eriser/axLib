/*******************************************************************************
 * Copyright (c) 2013 Alexandre Arsenault.
 *
 * This file is part of axLibrary.
 *
 * axLibrary is free or commercial software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 or any later version of the
 * License or use a commercial axLibrary License.
 *
 * axLibrary is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with axLibrary. If not, see <http://www.gnu.org/licenses/>.
 *
 * To release a closed-source product which uses axLibrary, commercial
 * licenses are available, email alx.arsenault@gmail.com for more information.
 ******************************************************************************/
#include "axVstGuiMac.h"
#include "axApp.h"
#include "axCocoaInterfaceMac.h"
#include "axVst.h"

axVstGuiMac::axVstGuiMac(AudioEffect* effect):
axVstGui(effect)
{

}

bool axVstGuiMac::open(void* ptr)
{
    
    axVst* curVst = static_cast<axVst*>(getEffect());
    
//    pluginOpenOrderMutex.lock();
    if(_isFirstTimeVstGUIOpen == true)
    {
        _isFirstTimeVstGUIOpen = false;
        
//        if(getEffect()->getProgram() == 1)
        if(curVst->GetPluginId() == 1)
        {
            _pluginHasBeenOpenOnHostInit = true;
        }
    }
    
    //    int pluginIndex = getEffect()->getProgram() - 1;
//    int pluginIndex = getEffect()->getProgram();
    int pluginIndex = curVst->GetPluginId();
    
    if(_pluginHasBeenOpenOnHostInit == true)
    {
        pluginIndex = pluginIndex - 1;
    }
//    pluginOpenOrderMutex.unlock();
    
    axApp* app = axApp::GetInstance();
    axVstCoreMac* vstCoreMac = static_cast<axVstCoreMac*>(app->GetCore());
    
    // First instance of vst plugin.
    if(vstCoreMac->GetCurrentManagerIndex() == 0 &&
       vstCoreMac->GetNumberOfManager() == 1 &&
       vstCoreMac->GetVstCoreData()->appDelegate == nullptr)
    {
        //        std::cout << "First vst instance" << std::endl;
        
        vstCoreMac->SetCurrentManagerIndex(0);
        axVstCoreData* coreData = vstCoreMac->GetVstCoreData();
        CreateNSWindowFromApp(ptr, systemWindow, coreData->appDelegate);
        coreData->effect = effect;
        vstCoreMac->InitGL();
        vstCoreMac->UpdateAll();
    }
    
    // Create new instance of vst plugin.
    else if(pluginIndex != vstCoreMac->GetCurrentManagerIndex() &&
            pluginIndex >= vstCoreMac->GetNumberOfManager())
    {
        //        std::cout << "Second vst instance" << std::endl;
        
        vstCoreMac->InitManagers();
        vstCoreMac->SetCurrentManagerIndex(pluginIndex);
        axVstCoreData* coreData = vstCoreMac->GetVstCoreData();
        CreateNSWindowFromApp(ptr, systemWindow, coreData->appDelegate);
        coreData->effect = effect;
        vstCoreMac->InitGL();
        vstCoreMac->UpdateAll();
        //        vstCoreMac->Init(axSize(0, 0));
        //        vstCoreMac->SetCurrentManagerIndex(pluginIndex);
        //        vstCoreMac->ReInitApp();
    }
    
    // Plugin instance already exist.
    else
    {
        //        std::cout << "Reattach vst instance" << std::endl;
        vstCoreMac->SetCurrentManagerIndex(pluginIndex);
        CreateNSWindowFromApp(ptr, systemWindow,
                              vstCoreMac->GetVstCoreData()->appDelegate);
        vstCoreMac->ReInitApp();
        vstCoreMac->InitGL();
        vstCoreMac->UpdateAll();
        //        axVstCoreData* coreData = vstCoreMac->GetVstCoreData();
    }
    
    systemWindow = ptr;
    
    return AEffEditor::open(ptr);
}
