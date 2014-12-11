//
//  axFileDialog.m
//  MidiSequencer
//
//  Created by Alexandre Arsenault on 2014-11-14.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#include "axCocoaInterfaceMac.h"
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#include <iostream>
#import "AppDelegate.h"



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
//    [[NSApp delegate]setNeedsDisplay:YES];
//     NSLog(@"TestFunctionInterface");
    axAppDelegate* appDelegate = (axAppDelegate*)[[NSApplication sharedApplication] delegate];
    [appDelegate MemberTestFunc];
//    [(axAppDelegate*)[[NSApplication sharedApplication] delegate]setNeedsDisplay:YES];

//    axAppDelegate* appDelegate = (axAppDelegate*)[NSApp delegate];
//    appDelegate.MemberTestFunc();
//    [appDelegate MemberTestFunc];
//    appDelegate->MemberTestFunc();
//    NSLog([(axAppDelegate*)[[NSApplication sharedApplication]delegate]);

//    [[(axAppDelegate*)[[NSApplication sharedApplication]delegate]MemberTestFunc ];
    
    
//    ((axAppDelegate*)[[NSApplication sharedApplication]delegate]).MemberTestFunc();
//    axAppDelegate *aD = ((axAppDelegate*)CCApplication::sharedApplication());
//    aD->myAppDelegateMethod();
}

std::string CocoaGetAppDirectory()
{
//    NSString *myPath = [[NSBundle mainBundle] bundlePath];
    NSString *curDir = [[NSFileManager defaultManager] currentDirectoryPath];
    return std::string([curDir UTF8String]);
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
    axAppDelegate* appDelegate = (axAppDelegate*)[[NSApplication sharedApplication] delegate];
    
    NSSize nSize = {static_cast<CGFloat>(size.x), static_cast<CGFloat>(size.y)};
    
    [appDelegate SetFrameSize:nSize];
}