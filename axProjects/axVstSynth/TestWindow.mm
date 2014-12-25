//
//  TestWindow.m
//  axVstSynth
//
//  Created by Alexandre Arsenault on 2014-12-23.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//
#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
#include "TestWindow.h"
#include "AppDelegate.h"
#include <iostream>
//#import <Carbon/Carbon.h>

void InitMacApp()
{
    NSApplicationLoad();
}

void SetWindow(void*& win)
{
    NSRect rect = {0, 0, 200, 200};
    axAppDelegate* hostWindow = [[axAppDelegate alloc] initWithFrame: rect ];
//    NSWindowPtr ptr = &hostWindow;
    win = (__bridge void*) hostWindow;
//    win = static_cast<void*>(hostWindow);
}

void* CreateNSWindow(void* ptr, void*& child)
{
    std::cout << "CreateNSWindow(void* ptr, void*& child) in TestWindow.mm" << std::endl;

    NSView* parentView = (__bridge NSView*)ptr;
//    [[parentView window] setBackgroundColor:[NSColor orangeColor]];
    if(GlobalAppDelegate == nullptr)
    {
        std::cout << "CreateNSWindow GlobalAppDelegate in TestWindow.mm" << std::endl;
        axAppDelegate* app = [[axAppDelegate alloc] initWithFrame: NSMakeRect(0, 0, 200, 200)];
        //    [[parentView window] addChildWindow:[app window] ordered:NSWindowBelow];
        
        [parentView addSubview: app];
    }

//    NSRect rect = NSMakeRect(0, 0, 200, 200);

    return (__bridge void*)parentView;
}

//- (void)addChildWindow:(NSWindow *)childWindow
//ordered:(NSWindowOrderingMode)orderingMode
//Parameters
//childWindow
//The child window to order.
//orderingMode
//NSWindowAbove: childWindow is ordered immediately in front of the window.
//NSWindowBelow: childWindow is ordered immediately behind the window.