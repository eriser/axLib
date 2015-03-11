//
//  axLoader.h
//  axVstHost
//
//  Created by Alexandre Arsenault on 2015-03-05.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __axVstHost__axLoader__
#define __axVstHost__axLoader__


#include <iostream>
#include <dlfcn.h>
#include <functional>

//_fctHandle = reinterpret_cast<void(*)(void)>

template<typename RETURN, typename PARAM>
class axModuleLoader
{
public:
    axModuleLoader(const std::string& path,
                   const std::string& entry_fct)
    {
        //RTLD_LOCAL
        _binHandle = dlopen(path.c_str(), RTLD_NOW | RTLD_GLOBAL);
        
        if(_binHandle != nullptr)
        {
            _fctHandle = reinterpret_cast<RETURN(*)(PARAM)>
            (dlsym(_binHandle, entry_fct.c_str()));
        }
        else
        {
            std::cerr << "Error : failed to load library." << std::endl;
        }
    }
    
    
    std::function<RETURN(PARAM)> GetHandle()
    {
        return _fctHandle;
    }
    
    ~axModuleLoader()
    {
        dlclose(_binHandle);
    }
    
//    std::function<void()> GetFunctionHandle()
//    {
//        return _fctHandle;
//    }
    
private:
    void* _binHandle;
    std::function<RETURN(PARAM)> _fctHandle;
};

//int main(int argc, const char * argv[])
//{
//    const std::string binaryFilePath = "/Users/alexarse/Desktop/ModuleTest/Debug/ModuleTest.bundle/Contents/MacOS/ModuleTest";
//    
//    axModuleLoader loader(binaryFilePath);
//    std::function<void()> fct = loader.GetFunctionHandle();
//    if(fct)
//    {
//        fct();
//    }
//    
//    fct();
//    
//    return 0;
//}


#endif /* defined(__axVstHost__axLoader__) */
