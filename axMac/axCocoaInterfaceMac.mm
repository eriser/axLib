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
#include "axCocoaInterfaceMac.h"
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#include <iostream>

#ifdef _AX_VST_APP_
#include "axVstAppDelegate.h"
#else
#import "AppDelegate.h"
#endif // _AX_VST_APP_

#include "axLib.h"




std::string axOpenFileDialog()
{
    NSOpenPanel* openDlg = [NSOpenPanel openPanel];
    [openDlg setCanChooseFiles:YES];
    [openDlg setCanChooseDirectories:YES];
    [openDlg setPrompt:@"Select"];
    
    std::string file_path;
    
    if ([openDlg runModalForDirectory:nil file:nil] == NSOKButton )
    {
        NSArray* files = [openDlg filenames];
        for(NSString* filePath in [openDlg filenames])
        {
            file_path = std::string([filePath UTF8String]);
        }
    }
    
    return file_path;
}

void TestFunctionInterface()
{
//    axAppDelegate* appDelegate = (axAppDelegate*)[[NSApplication sharedApplication] delegate];
//    [appDelegate MemberTestFunc];
    
    std::cout << "TestFunctionInterface" << std::endl;
    
    
#ifdef _AX_VST_APP_
    axApp* app = axApp::CreateApp();
    axVstCoreMac* vstCoreMac = static_cast<axVstCoreMac*>(app->GetCore());
    axVstAppDelegate* delegate = (__bridge axVstAppDelegate*)
                                 vstCoreMac->GetCurrentAppDelegate();
    
    
//    std::vector<axVstCoreData>* data = vstCoreMac->GetManagerVector();
//    for(auto& n : *data)
//    {
//        if(n.appDelegate != nullptr)
//        {
//            axAppDelegate* d = (__bridge axAppDelegate*)n.appDelegate;
//            [d MemberTestFunc];
//        }
//    }
    
    [delegate MemberTestFunc];

#else
    
    [GlobalAppDelegate MemberTestFunc];
#endif // _AX_VST_APP_
}

std::string CocoaGetAppDirectory()
{
#ifdef DEBUG
    NSString *curDir = [[NSFileManager defaultManager] currentDirectoryPath];
    return std::string([curDir UTF8String]);
#else // RELEASE.
    
    NSString *path = [[NSBundle mainBundle] executablePath];
    std::string app_path([path UTF8String]);
    app_path = app_path.substr(0, app_path.find_last_of("/"));
    app_path = app_path.substr(0, app_path.find_last_of("/"));
    return app_path + std::string("/Resources");
    
#endif // DEBUG.
}

void AddEventToDispatchQueue()
{
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0ul);
    dispatch_async(queue, ^{
        // Perform async operation
        dispatch_sync(dispatch_get_main_queue(), ^{
            // Update UI
        });
    });
}

void axCocoaResizeFrame(const axSize& size)
{
#ifdef _AX_VST_APP_
#else
    axAppDelegate* appDelegate = (axAppDelegate*)[[NSApplication sharedApplication] delegate];
    
    NSSize nSize = {static_cast<CGFloat>(size.x), static_cast<CGFloat>(size.y)};
    
    [appDelegate SetFrameSize:nSize];
#endif

}

static axPoint hide_mouse_position;

void axCocoaHideMouse()
{
    [NSCursor hide];
    NSPoint mPos = [NSEvent mouseLocation];
    hide_mouse_position.x = mPos.x;
    hide_mouse_position.y = mPos.y;
}

void axCocoaShowMouse()
{
    NSRect e = [[NSScreen mainScreen] frame];
    
    CGPoint pt = CGPointMake(hide_mouse_position.x,
                           e.size.height - hide_mouse_position.y);
    CGDisplayErr err;
    if ((err = CGWarpMouseCursorPosition(pt)) != CGEventNoErr)
    {
        NSLog(@"CGWarpMouseCursorPosition returned: \"%d\"", err);
    }
    
    [NSCursor unhide];
}

//------------------------------------------------------------------------------
// Use for vst interface from host given parent.
//void* CreateNSWindow(void* ptr, void*& child)
//{
//    std::cout << "CreateNSWindow(void* ptr, void*& child) in TestWindow.mm" << std::endl;
    
//#ifdef _AX_VST_APP_
//    NSView* parentView = (__bridge NSView*)ptr;
//
//    if(GlobalAppDelegate == nullptr)
//    {
////        std::cout << "CreateNSWindow GlobalAppDelegate in TestWindow.mm" << std::endl;
//        axVstAppDelegate* app = [[axVstAppDelegate alloc] initWithFrame: NSMakeRect(0, 0, 200, 200)];
//        GlobalAppDelegate = app;
//        [parentView addSubview: app];
//        
//        return (__bridge void*)app;
//    }
//    else
//    {
////        std::cout << "CreateNSWindow GlobalAppDelegate ELSE in TestWindow.mm" << std::endl;
//        //axAppDelegate* app = [[axAppDelegate alloc] initWithFrame: NSMakeRect(0, 0, 200, 200)];
//        [parentView addSubview: GlobalAppDelegate];
//
//    }
//    
//    //    return (__bridge void*)parentView;
//    return (__bridge void*)GlobalAppDelegate;
//
//#else
//    return nullptr;
//#endif
//}

void* CreateNSWindowFromApp(void* parent, void*& child, void*& appDelegate)
{
    
#ifdef _AX_VST_APP_
    NSView* parentView = (__bridge NSView*)parent;
    
    if(appDelegate == nullptr)
    {
        std::cout << "CREATE axVstAppDelegate WINDOW" << std::endl;
        axVstAppDelegate* app = [[axVstAppDelegate alloc] initWithFrame: NSMakeRect(0, 0, 200, 200)];
        appDelegate = (__bridge void*)app;
        [appDelegate retain];
        [parentView addSubview: app];
    }
    else
    {
        std::cout << "ATTACH WINDOW" << std::endl;
        [parentView addSubview: (__bridge axVstAppDelegate*)appDelegate];
    }
    
    return (__bridge void*)parentView;
    
#else
    
#endif // _AX_VST_APP_
}


void axReInitApp(void* appDelegate)
{
#ifdef _AX_VST_APP_
    axVstAppDelegate* app = (__bridge axVstAppDelegate*)appDelegate;
    [app ReInit];
#endif
}