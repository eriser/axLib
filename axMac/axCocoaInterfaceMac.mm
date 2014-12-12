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