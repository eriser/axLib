//
//  axAppDelegate.m
//  test
//
//  Created by Caroline Ross on 2014-04-24.
//  Copyright (c) 2014 alexarse. All rights reserved.
//

#import "AppDelegate.h"

#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>

#include <iostream>
#include "axLib.h"
//#include "Main.h"

axApp* GlobalApp = nullptr;



@implementation axAppDelegate

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    
    // below code helps optimize Open GL context
    // initialization for the best available resolution
    // important for Retina screens for example
    if (self)
    {
        [self wantsBestResolutionOpenGLSurface];
    }
    
//    cout << "TEST" << endl;
    return self;
}


- (void)viewDidMoveToWindow
{
    //[[self window] setAcceptsMouseMovedEvents:YES];
    [self addTrackingRect:[self bounds] owner:self userData:NULL assumeInside:YES];
    [[self window] makeFirstResponder:self];
}

- (void)prepareOpenGL
{
    // Synchronize buffer swaps with vertical refresh rate
    GLint swapInt = 1;
    [[self window] setAcceptsMouseMovedEvents:YES];
    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
    
    axApp::MainInstance = new axApp();
    axMain::MainEntryPoint(axApp::MainInstance);
    
//    cout << "TEST2" << endl;
    //    axApp::MainInstance->CallMainEntryPoint(axApp::MainInstance);
    //    Desktop* desktop = new Desktop(axApp::MainInstance, nullptr, axRect(0, 0, 200, 200));
}

-(void)awakeFromNib
{
    //when UI is created and properly initialized,
    // we set the timer to continual, real-time rendering
    //a 1ms time interval
    //    renderTimer = [NSTimer timerWithTimeInterval:0.001
    //                                          target:self
    //                                        selector:@selector(timerFired:)
    //                                        userInfo:nil
    //                                         repeats:YES];
    //
    //    [[NSRunLoop currentRunLoop] addTimer:renderTimer
    //                                 forMode:NSDefaultRunLoopMode];
    //
    //    //Ensure timer fires during resize
    //    [[NSRunLoop currentRunLoop]
    //     addTimer:renderTimer
    //     forMode:NSEventTrackingRunLoopMode];
}

// Working.
-(void)mouseDown:(NSEvent *)event
{
    NSPoint locationInView = [self convertPoint:[event locationInWindow]
                                       fromView:nil];
    
//    int y = axApp::MainInstance->GetCore()->GetGlobalSize().y;
//    axPoint pos(locationInView.x, y - locationInView.y);
    axPoint pos(locationInView.x, locationInView.y);
    axApp::MainInstance->GetWindowManager()->OnMouseLeftDown(pos);
    
    [self setNeedsDisplay:YES];
}

// Working.
- (void)mouseUp:(NSEvent *)anEvent
{
    NSPoint locationInView = [self convertPoint:[anEvent locationInWindow]
                                       fromView:nil];
    
//    int y = axApp::MainInstance->GetCore()->GetGlobalSize().y;
//    axPoint pos(locationInView.x, y - locationInView.y);
    axPoint pos(locationInView.x, locationInView.y);
    axApp::MainInstance->GetWindowManager()->OnMouseLeftUp(pos);
    
    [self setNeedsDisplay:YES];
}

// Working.
- (void)mouseDragged:(NSEvent *)theEvent
{
    NSPoint locationInView = [self convertPoint:[theEvent locationInWindow]
                                       fromView:nil];
    
//    int y = axApp::MainInstance->GetCore()->GetGlobalSize().y;
//    axPoint pos(locationInView.x, y - locationInView.y);
    axPoint pos(locationInView.x, locationInView.y);
    axApp::MainInstance->GetWindowManager()->OnMouseLeftDragging(pos);
    [self setNeedsDisplay:YES];
}

// Working.
- (void)mouseMoved:(NSEvent *)MyMouseMouse
{
    NSPoint locationInView = [self convertPoint:[MyMouseMouse locationInWindow]
                                       fromView:nil];
    
//    int y = axApp::MainInstance->GetCore()->GetGlobalSize().y;
//    axPoint pos(locationInView.x, y - locationInView.y);
    axPoint pos(locationInView.x, locationInView.y);
    axApp::MainInstance->GetWindowManager()->OnMouseMotion(pos);
    
    [self setNeedsDisplay:YES];
    
}

// Working.
- (void)mouseEntered:(NSEvent *)theEvent
{
    NSLog(@"Mouse enter");
    //    [[self window] setAcceptsMouseMovedEvents:YES];
}

- (void)mouseExited:(NSEvent *)theEvent
{
    NSLog(@"Mouse leave");
    //    [[self window] setAcceptsMouseMovedEvents:NO];
}


// Timer callback method
- (void)timerFired:(id)sender
{

}

// Set origin at top left position.
- (BOOL)isFlipped
{
    return YES;
}


// Each time window has to be redrawn, this method is called
- (void)drawRect:(NSRect)bounds
{
    int frame_height = bounds.size.height;
//    cout << "gg : " << bounds.origin.y << endl;
//    CGPoint position = CGPointMake(0,0);
//    [self setFrame:(CGRect){.origin = position,.size = self.frame.size}];
    
//    bounds.origin = CGPointMake(50,0);
//    NSRect frame_rect = [self convertRectToBacking:[self drawRect]];
    NSRect backingBounds = [self convertRectToBacking:[self bounds]];

    axApp::MainInstance->GetCore()->ResizeGLScene(backingBounds.size.width,
                                                  backingBounds.size.height,
                                                  frame_height - backingBounds.size.height);
//    NSRect frame = [self frame];
//    frame.origin.y = 0;
//    [self setFrame:frame];
//    NSPoint pt;
//    pt.x = 0;
//    pt.y = 0;
//    [self setFrameTopLeftPoint:pt];
//    cout << "POSITION : " << backingBounds.origin.y << endl;
//    cout << "HEIGHT : " << backingBounds.size.height << endl;
//    cout << "FHEIGHT : " << frame_rect.size.height << endl;
//    if ( [self frame].size.height < [[self superview] frame].size.height )
//    {
//        [self frame].size.height = [[self superview] frame].size.height;
//    }
    

    // Is only gonna draw if necessary.
    axApp::MainInstance->GetCore()->DrawGLScene();
    glFlush();
}


@end