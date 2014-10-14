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

axApp* GlobalApp = nullptr;

class Desktop : public axPanel
{
public:
    Desktop(axApp* app, axWindow* parent, const axRect& rect):
    axPanel(app, parent, rect)
    {
        
        axButtonInfo btn_info(axColor(0.8, 0.8, 0.8), axColor(0.9, 0.9, 0.9), axColor(1.0, 1.0, 1.0),
                     axColor(0.8, 0.8, 0.8), axColor(0.0, 0.0, 0.0), axColor(0.8, 0.8, 0.8));
        
        axEvtFunction(axButtonMsg) evt(GetOnBtn());
        axButton* btn = new axButton(app, this, axRect(30, 30, 30, 30),
                                axButtonEvents(evt),
                                btn_info);
        
        axButton* btn2 = new axButton(app, this, axRect(65, 30, 30, 30),
                                     axButtonEvents(),
                                     btn_info);

    }
    
    axEVENT(axButtonMsg, OnBtn);
    
    void OnBtn(const axButtonMsg& msg)
    {
        cout << "Btn 1 msg." << endl;
    }
    
    void OnPaint()
    {
        axGC* gc = GetGC();
        axRect rect(GetRect());
        axRect rect0(axPoint(0, 0), rect.size);
        
        gc->SetColor(axColor(0.0, 1.0, 0.0), 1.0);
        gc->DrawRectangle(rect0);
    }
};
//-----------------------------------------------------------

@implementation axAppDelegate

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    
    //below code helps optimize Open GL context
    // initialization for the best available resolution
    // important for Retina screens for example
    if (self)
    {
        [self wantsBestResolutionOpenGLSurface];
    }
    
    //app = axApp(axSize(500, 500));
    axApp* app = new axApp();
    GlobalApp = app;
    Desktop* desktop = new Desktop(app, nullptr, axRect(0, 0, 200, 200));
//    app->GetWindowManager()->Add(desktop);
//    app->GetWindowManager()->Add(desktop);
//    app->GetWindowManager()->Add(desktop);
//    app->GetWindowManager()->Add(desktop);
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
    
    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
    
    
}

-(void)awakeFromNib
{
    //when UI is created and properly initialized,
    // we set the timer to continual, real-time rendering
    //a 1ms time interval
    renderTimer = [NSTimer timerWithTimeInterval:0.001
                                          target:self
                                        selector:@selector(timerFired:)
                                        userInfo:nil
                                         repeats:YES];
    
    [[NSRunLoop currentRunLoop] addTimer:renderTimer
                                 forMode:NSDefaultRunLoopMode];
    
    //Ensure timer fires during resize
    [[NSRunLoop currentRunLoop]
     addTimer:renderTimer
     forMode:NSEventTrackingRunLoopMode];
}

// Working.
-(void)mouseDown:(NSEvent *)event
{
    NSPoint locationInView = [self convertPoint:[event locationInWindow]
                                       fromView:nil];
    
    int y = GlobalApp->GetCore()->GetGlobalSize().y;
    axPoint pos(locationInView.x, y - locationInView.y);
    GlobalApp->GetWindowManager()->OnMouseLeftDown(pos);
    //NSLog(@"Mouse motion");
//    NSLog(@"Mouse motion = %f, %f", locationInView.x, y - locationInView.y);

}

// Working.
- (void)mouseUp:(NSEvent *)anEvent
{
    NSPoint locationInView = [self convertPoint:[anEvent locationInWindow]
                                       fromView:nil];
    
    int y = GlobalApp->GetCore()->GetGlobalSize().y;
    axPoint pos(locationInView.x, y - locationInView.y);
    GlobalApp->GetWindowManager()->OnMouseLeftUp(pos);
}

// Working.
- (void)mouseDragged:(NSEvent *)theEvent
{
    NSLog(@"Mouse drag");
}

// Working.
- (void)mouseMoved:(NSEvent *)MyMouseMouse
{
    NSPoint locationInView = [self convertPoint:[MyMouseMouse locationInWindow]
                                       fromView:nil];
    
    int y = GlobalApp->GetCore()->GetGlobalSize().y;
    axPoint pos(locationInView.x, y - locationInView.y);
    GlobalApp->GetWindowManager()->OnMouseMotion(pos);
}

// Working.
- (void)mouseEntered:(NSEvent *)theEvent
{
//    NSLog(@"Mouse enter");
    [[self window] setAcceptsMouseMovedEvents:YES];
}

- (void)mouseExited:(NSEvent *)theEvent
{
    [[self window] setAcceptsMouseMovedEvents:NO];
}


// Timer callback method
- (void)timerFired:(id)sender
{
    bool need_to_draw = GlobalApp->GetCore()->DrawGLScene();
    if(need_to_draw)
    {
        [self setNeedsDisplay:YES];
    }
    
}

// Each time window has to be redrawn, this method is called
- (void)drawRect:(NSRect)bounds
{
    //below code sets the viewport of Open GL context into
    //correct size (assuming resize, fullscreen operations may trigger change)
    NSRect backingBounds = [self convertRectToBacking:[self bounds]];
    //glViewport(0,0, backingBounds.size.width, backingBounds.size.height);

    GlobalApp->GetCore()->ResizeGLScene(backingBounds.size.width, backingBounds.size.height);
//    GlobalApp->GetCore()->UpdateAll();
    GlobalApp->GetCore()->DrawGLScene();
    glFlush();
}


@end