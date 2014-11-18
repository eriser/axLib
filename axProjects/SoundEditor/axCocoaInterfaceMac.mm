//
//  axFileDialog.m
//  MidiSequencer
//
//  Created by Alexandre Arsenault on 2014-11-14.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

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