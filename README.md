#axLib

A C++ cross-platform framework to build highly customizable platform-independent graphical user interface.

## Features
- Easy to setup.
- Currently supports Mac OS X (10.10), Windows 7 and 8 and Linux (X11).
- Extensive use of C++11 and Standard Template Library (STL) syntax. 
- Use of C++11 lambda functions and threads for event management.
- Minimize interaction with operating systems.

## Build in Widgets

| First Header  | Second Header |
| ------------- | ------------- |
| Content Cell  | Content Cell  |
| Content Cell  | Content Cell  |

- axButton

	![axButton image](https://dl.dropboxusercontent.com/u/26931825/axLibWebData/axButton.png)
- axDropMenu
- axGrid
- axKnob
- axMenu
- axNumberBox

	![axNumberBox](https://dl.dropboxusercontent.com/u/26931825/axLibWebData/axNumberBox.png)
- axPopupMenu
- axScrollBar
- axSlider
- axToggle

## Code examples

[Create axButton](https://github.com/alexarse/axLib/wiki/axButton) 
- Use the build-in axButton with and without custom skins. 
- Basic example of simple axEvent.

[Custom Animated Button](https://github.com/alexarse/axLib/wiki/Custom-animated-button.)
- Overload axButton virtual function.
- Use of axTimer object.

## Project examples

![MidiSequencer]
(https://dl.dropboxusercontent.com/u/26931825/axLibWebData/MidiSequencer.png) ![Calculator]
(https://dl.dropboxusercontent.com/u/26931825/axLibWebData/Calculator.png)

![SoundEditor]
(https://dl.dropboxusercontent.com/u/26931825/axLibWebData/SoundEditor.png)



## Create Project


### Mac OS 10.10 (Yosemite) - Xcode 6.1
1. First you need to install freetype-2.4.0 library or newer version and libpng15. 
	* Make sure they're located in **/usr/local/include/** and **/usr/local/lib/**

2. Create new Project Cocoa application with **Objective-C** language.

3. Delete test folder in project navigator and click on move to trash.

	 ![Delete test folder](https://dl.dropboxusercontent.com/u/26931825/axLibWebData/ProjectFromScratch_Test.png)

4. Then go in project option under **Targets List** right click and delete test target.

	![Delete test](https://dl.dropboxusercontent.com/u/26931825/axLibWebData/ProjectFromScratch_Tst2.png)

5. Delete these files in project navigator and move to trash.
	* **AppDelegate.h** 
	* **AppDelegate.m** 
	* **MainMenu.xib**

6. Drag or include these folders in project navigator.
	* **axLib/include/**
	* **axLib/source/**
	* **axLib/axWidgets/**
	* **axLib/axMac/**

7. Add these folder to **Search Paths** tab under **Header Search Paths** in project configuration.
	* **/usr/local/include/**
	* **/usr/local/include/freetype2/**

8. Add this folder to **Search Paths** tab under **Library Search Paths** in project configuration.
	* **/usr/local/lib/**

9. Add these flags to **Linking** tab under **Other Linker Flags** still in project configuration.
	* **-lfreetype**
	* **-lpng15**

10. Change **Per-configuration Build Products Path** in **Build Locations** tab to :
	* **$(SRCROOT)/Debug/**
	* **$(SRCROOT)/Release/**

11. Then copy **axLib/ressources/axFonts/FreeSans.ttf** into each folder created (Debug and Release).

10. Finally create new **main.cpp** and **main.h** files and add them to project. 
	* Continue to **Minimal implementation** section and copy code below for bare minimum implementation.

### Windows 7 and 8 - Visual Studio 2013
1. Create a **AX_LIB** path variable to axLib main folder in **Control Panel** under **System**.

	![System](https://dl.dropboxusercontent.com/u/26931825/axLibWebData/sysprop.PNG)

2. Compile **FreeType2** in **axLib/libs/freetype-2.5.3/builds/windows/vc2010/**.

3. Compile **LibPng** in **axLib/libs/lpng1610/projects/vstudio/**.

4. Create a C++ **Emtpy Project** in visual studio.

	![Empty](https://dl.dropboxusercontent.com/u/26931825/axLibWebData/console.PNG)

5. Add these folders to project.
	* **axLib/include/**
	* **axLib/source/**
	* **axLib/axWidgets/**
	* **axLib/axWin32/**

6. Add these folders in **Configuration Properties** under C/C++ General.

	![Include](https://dl.dropboxusercontent.com/u/26931825/axLibWebData/include.PNG)
	
	* **$(AX_LIB)/include/**
	* **$(AX_LIB)/axWin32/**
	* **$(AX_LIB)/axWidgets/**
	* **$(AX_LIB)/libs/freetype-2.5.3/include/**
	* **$(AX_LIB)/libs/lpng1610/**

6. Add these folders in **Configuration Properties** under Linker General in **Additional Library Directories**.

	![Lib directory](https://dl.dropboxusercontent.com/u/26931825/axLibWebData/lib_dir.PNG)
	
	* **$(AX_LIB)/libs/lpng1610/projects/vstudio/Debug/**
	* **$(AX_LIB)/libs/freetype-2.5.3/objs/win32/vc2010/**

7. Then add these lines to **Linker Input** in **Additional Dependencies**.

	![Lib directory](https://dl.dropboxusercontent.com/u/26931825/axLibWebData/link_folder.PNG)

	* **opengl32.lib**
	* **glu32.lib**
	* **libpng16.lib**
	* **zlib.lib**
	* **freetype253_D.lib**

8. Add this macro in **Configuration Properties** under **C/C++ Preprocessor**.

	![Lib directory](https://dl.dropboxusercontent.com/u/26931825/axLibWebData/prepro.PNG)
	
	* **_CRT_SECURE_NO_WARNINGS**

9. Finally create new main.cpp and main.h files and add them to project.

	* Continue to Minimal implementation section and copy code below for bare minimum implementation.



## Minimal implementation.
main.h
``` cpp
#ifndef __MINIMAL_PROJECT__
#define __MINIMAL_PROJECT__

#include "axLib.h"

class MyProject: public axPanel
{
public:
    MyProject(axWindow* parent,
              const axRect& rect);
private:
    // axEvents.
    virtual void OnPaint();
};

#endif // __MINIMAL_PROJECT__
```

main.cpp
``` cpp
#include "main.h"

MyProject::MyProject(axWindow* parent, const axRect& rect):
                     axPanel(parent, rect)
{
    
}

void MyProject::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect0(axPoint(0, 0), GetRect().size);
    
    gc->SetColor(axColor(0.3, 0.3, 0.3, 1.0));
    gc->DrawRectangle(rect0);

    gc->SetColor(axColor(0.0, 0.0, 0.0, 1.0));
    gc->DrawRectangleContour(rect0);
}

void axMain::MainEntryPoint(axApp* app)
{
    MyProject* myProject = new MyProject(nullptr, axRect(0, 0, 500, 500));
}
```

## Git instructions.
For now, all development is made on **dev** branch. But Release-xx.xx.xx branches will be use when new release approaches.

To add a new feature, you just need to create a new branch from dev.

```
git checkout -b myfeature dev
```

Then to incorporate a finished feature on dev.
```
git checkout dev
git merge --no-ff myfeature
git branch -d myfeature
git push origin dev
```






