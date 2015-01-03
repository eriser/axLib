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
#ifndef __AX_VST_GUI__
#define __AX_VST_GUI__

//#include <thread>
//#include <mutex>

#include "public.sdk/source/vst2.x/audioeffectx.h"
#include "public.sdk/source/vst2.x/aeffeditor.h"

class axVstGui : public AEffEditor
{
public:
    axVstGui(AudioEffect* effect);
    
    virtual bool open(void* ptr) = 0;
    virtual bool getRect(ERect** rect);
    
protected:
    ERect _rect;
    static bool _pluginHasBeenOpenOnHostInit;
    static bool _isFirstTimeVstGUIOpen;
//    static std::mutex pluginOpenOrderMutex;
};

#endif // __AX_VST_GUI__
