//
//  axAppDelegate.h
//  test
//
//  Created by Caroline Ross on 2014-04-24.
//  Copyright (c) 2014 alexarse. All rights reserved.
//

#import <Cocoa/Cocoa.h>
//#include "axApp.h"
//#include "axTrempoline.h"
//#include "main.h"


@interface axAppDelegate : NSOpenGLView
{
    //system timer, needed to synchronize the frame rate
    NSTimer* renderTimer;
    
    //our C++ renderer as I aim to minimize
    //ObjectiveC footprint and use clean C/C++ only, if possible
    //    axApp app;
    
    
}
//it's analogical to WM_PAINT event in Windows
- (void) drawRect: (NSRect)bounds;
- (void) mouseMoved:(NSEvent *)MyMouseMouse;
- (void)windowDidResize:(NSEvent *)event;
- (void)windowDidMove:(NSNotification *)notification;
- (void) setFrameSize:(NSSize)newSize;
- (void) SetFrameSize:(NSSize)newSize;
- (id) MemberTestFunc;
//+ (void) MemberTestFunc;
//-(void)mouseMoved:(NSEvent *)event;

@end




