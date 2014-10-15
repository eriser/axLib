//
//  axAppDelegate.m
//  test
//
//  Created by Caroline Ross on 2014-04-24.
//  Copyright (c) 2014 alexarse. All rights reserved.
//

#import "axAppDelegate.h"

#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>

#include <iostream>
#include "axLib.h"
//#include "Main.h"

axApp* GlobalApp = nullptr;

//class Desktop : public axPanel
//{
//public:
//    Desktop(axApp* app, axWindow* parent, const axRect& rect):
//    axPanel(app, parent, rect)
//    {
//        
//        axButtonInfo btn_info(axColor(0.8, 0.8, 0.8),
//                              axColor(0.9, 0.9, 0.9),
//                              axColor(1.0, 1.0, 1.0),
//                              axColor(0.8, 0.8, 0.8),
//                              axColor(0.0, 0.0, 0.0),
//                              axColor(0.8, 0.8, 0.8));
//        
//        axEvtFunction(axButtonMsg) evt(GetOnBtn());
//        axButton* btn = new axButton(app, this, axRect(30, 30, 48, 48),
//                                axButtonEvents(evt),
//                                btn_info,"/Users/alexarse/Project/axLib/ressources/axImages/calc.png", "");
//        
//        axEvtFunction(axButtonMsg) evt2(GetOnBtn2());
//        axButton* btn2 = new axButton(app, this, axRect(100, 30, 48, 48),
//                                     axButtonEvents(evt2),
//                                     btn_info);
//
//    }
//    
//    axEVENT(axButtonMsg, OnBtn);
//    axEVENT(axButtonMsg, OnBtn2);
//    
//    void OnBtn(const axButtonMsg& msg)
//    {
//        cout << "Btn 1 msg." << endl;
//    }
//    
//    
//    void OnBtn2(const axButtonMsg& msg)
//    {
//        cout << "Btn 2 msg." << endl;
//    }
//    
//    void OnPaint()
//    {
//        axGC* gc = GetGC();
//        axRect rect(GetRect());
//        axRect rect0(axPoint(0, 0), rect.size);
//        
//        gc->SetColor(axColor(0.0, 1.0, 0.0), 1.0);
//        gc->DrawRectangle(rect0);
//    }
//};
//-----------------------------------------------------------

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
    
    int y = axApp::MainInstance->GetCore()->GetGlobalSize().y;
    axPoint pos(locationInView.x, y - locationInView.y);
    axApp::MainInstance->GetWindowManager()->OnMouseLeftDown(pos);
    
    [self setNeedsDisplay:YES];
}

// Working.
- (void)mouseUp:(NSEvent *)anEvent
{
    NSPoint locationInView = [self convertPoint:[anEvent locationInWindow]
                                       fromView:nil];
    
    int y = axApp::MainInstance->GetCore()->GetGlobalSize().y;
    axPoint pos(locationInView.x, y - locationInView.y);
    axApp::MainInstance->GetWindowManager()->OnMouseLeftUp(pos);
    
    [self setNeedsDisplay:YES];
}

// Working.
- (void)mouseDragged:(NSEvent *)theEvent
{
    NSPoint locationInView = [self convertPoint:[theEvent locationInWindow]
                                       fromView:nil];

    int y = axApp::MainInstance->GetCore()->GetGlobalSize().y;
    axPoint pos(locationInView.x, y - locationInView.y);
    axApp::MainInstance->GetWindowManager()->OnMouseLeftDragging(pos);
    [self setNeedsDisplay:YES];
}

// Working.
- (void)mouseMoved:(NSEvent *)MyMouseMouse
{
    NSPoint locationInView = [self convertPoint:[MyMouseMouse locationInWindow]
                                       fromView:nil];
    
    int y = axApp::MainInstance->GetCore()->GetGlobalSize().y;
    axPoint pos(locationInView.x, y - locationInView.y);
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
//    bool need_to_draw = GlobalApp->GetCore()->DrawGLScene();
//    if(need_to_draw)
//    {
//        [self setNeedsDisplay:YES];
//    }
    
}

// Each time window has to be redrawn, this method is called
- (void)drawRect:(NSRect)bounds
{
    //below code sets the viewport of Open GL context into
    //correct size (assuming resize, fullscreen operations may trigger change)
    NSRect backingBounds = [self convertRectToBacking:[self bounds]];
    //glViewport(0,0, backingBounds.size.width, backingBounds.size.height);

    axApp::MainInstance->GetCore()->ResizeGLScene(backingBounds.size.width, backingBounds.size.height);
    axApp::MainInstance->GetCore()->DrawGLScene();
    glFlush();
}


@end