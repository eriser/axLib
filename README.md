#axLib

A C++ cross-platform framework to build highly customizable platform-independent graphical user interface.

## Features
- Easy to setup.
- Currently supports Mac OS X (10.10), Windows 7 and 8 and Linux (X11).
- Extensive use of C++11 and Standard Template Library (STL) syntax. 
- Use of C++11 lambda functions and threads for event management.
- Minimize interaction with operating systems.

## Build in Widgets
- axButton

![axButton image](https://dl.dropboxusercontent.com/u/26931825/axLibWebData/axButton.png)

- axDropMenu
- axGrid
- axKnob
- axMenu
- axNumberBox
- axPopupMenu
- axScrollBar
- axSlider
- axToggle

## Create Project from scratch


### Mac OS 10.10 (Yosemite) - Xcode 6.1
1. First you need to install freetype-2.4.0 library or newer version and libpng15. 
	* Make sure they're located in **/usr/local/include/** and **/usr/local/lib/**

2. Create new Project Cocoa application with **Objective-C** language.

3. Delete test folder in project navigator and click on move to trash.

4. Then go in project option under **Targets List** and delete test target.

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

10. Finally create new **main.cpp** and **main.h** files and add them to project. 
	* Continue to **Minimal implementation** section and copy code below for bare minimum implementation.

### Minimal implementation.
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