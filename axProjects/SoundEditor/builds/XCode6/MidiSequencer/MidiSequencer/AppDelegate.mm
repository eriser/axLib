
#import "AppDelegate.h"
#include "main.h"

#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>

#include <iostream>
#include "axLib.h"

axApp* GlobalApp = nullptr;
axAppDelegate* GlobalAppDelegate;

@implementation axAppDelegate

- (id)initWithFrame:(NSRect)frame
{
    [self installRunLoopObserver];
    
    self = [super initWithFrame:frame];
    
    // Helps optimize Open GL context initialization for the best available
    // resolution, important for Retina screens for example.
    if (self)
    {
        [self wantsBestResolutionOpenGLSurface];
    }
    
    return self;
}


- (void)viewDidMoveToWindow
{
    [self addTrackingRect:[self bounds] owner:self userData:NULL assumeInside:YES];
    [[self window] makeFirstResponder:self];
    
    GlobalAppDelegate = self;
}

- (void)prepareOpenGL
{
    // Synchronize buffer swaps with vertical refresh rate
    GLint swapInt = 1;
    [[self window] setAcceptsMouseMovedEvents:YES];
    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
    
    axEventManager::GetInstance();
    axApp* app = axApp::CreateApp();
    axMain::MainEntryPoint(app);
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

-(id)MemberTestFunc
{
    [GlobalAppDelegate setNeedsDisplay:YES];
    
    return self;
}

- (void)windowDidResize:(NSEvent *)event
{
    std::cout << "Resize. " << std::endl;
}

- (void)windowDidMove:(NSNotification *)notification
{
    std::cout << "Mouve. " << std::endl;
}

- (void) setFrameSize:(NSSize)newSize
{
    NSSize myNSWindowSize = [ [ self window ] frame ].size;
    std::cout << "My size : " << myNSWindowSize.width << " " << myNSWindowSize.height << std::endl;
}


// Working.
-(void)mouseDown:(NSEvent *)event
{
    NSPoint locationInView = [self convertPoint:[event locationInWindow]
                                       fromView:nil];
    
    axPoint pos(locationInView.x, locationInView.y);
    
    axApp::MainInstance->GetPopupManager()->OnMouseLeftDown(pos);
    if(axApp::MainInstance->GetPopupManager()->IsEventReachWindow() == false)
    {
        axApp::MainInstance->GetWindowManager()->OnMouseLeftDown(pos);
    }
}

// Working.
- (void)mouseUp:(NSEvent *)anEvent
{
    NSPoint locationInView = [self convertPoint:[anEvent locationInWindow]
                                       fromView:nil];

    axPoint pos(locationInView.x, locationInView.y);
    axApp::MainInstance->GetPopupManager()->OnMouseLeftUp(pos);

    // TODO :: Fix this.
    axApp::MainInstance->GetWindowManager()->OnMouseLeftUp(pos);

}

// Working.
- (void)mouseDragged:(NSEvent *)theEvent
{
    NSPoint locationInView = [self convertPoint:[theEvent locationInWindow]
                                       fromView:nil];
    
    axPoint pos(locationInView.x, locationInView.y);
    axApp::MainInstance->GetPopupManager()->OnMouseLeftDragging(pos);
    if(axApp::MainInstance->GetPopupManager()->IsEventReachWindow() == false)
    {
        axApp::MainInstance->GetWindowManager()->OnMouseLeftDragging(pos);
    }
}

// Working.
- (void)mouseMoved:(NSEvent *)MyMouseMouse
{
    NSPoint locationInView = [self convertPoint:[MyMouseMouse locationInWindow]
                                       fromView:nil];

    axPoint pos(locationInView.x, locationInView.y);
    
    axApp::MainInstance->GetPopupManager()->OnMouseMotion(pos);
    if(axApp::MainInstance->GetPopupManager()->IsEventReachWindow() == false)
    {
        axApp::MainInstance->GetWindowManager()->OnMouseMotion(pos);
    }
}

// Working.
- (void)mouseEntered:(NSEvent *)theEvent
{
    NSLog(@"Mouse enter");
}

- (void)mouseExited:(NSEvent *)theEvent
{
    NSLog(@"Mouse leave");
}

static int test_value = 0;
void MyRunLoopObserver(CFRunLoopObserverRef observer,
                       CFRunLoopActivity activity,
                       void* info)
{
    axEventManager::GetInstance()->CallNext();
}

-(void) installRunLoopObserver
{
    std::cout << "Install run observer." << std::endl;
    
    // Run loop observer.
    CFRunLoopObserverRef myObserver = NULL;
    int myActivities = kCFRunLoopAllActivities; //kCFRunLoopBeforeWaiting;
    
    // Create observer reference.
    myObserver = CFRunLoopObserverCreate(NULL,
                                         myActivities,
                                         YES, 0,
                                         &MyRunLoopObserver,
                                         NULL);

    if(myObserver)
    {
        CFRunLoopAddObserver(CFRunLoopGetCurrent(),
                             myObserver,
                             kCFRunLoopCommonModes);
    }
    
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

    NSRect backingBounds = [self convertRectToBacking:[self bounds]];
    
    axCore* core = axApp::MainInstance->GetCore();
    
    axSize global_size = core->GetGlobalSize();
    if(global_size.x != backingBounds.size.width ||
       global_size.y != backingBounds.size.height)
    {
        core->ResizeGLScene(backingBounds.size.width,
                            backingBounds.size.height,
                            frame_height - backingBounds.size.height);
    }

    if ([self inLiveResize])
    {
        // Draw a quick approximation
        std::cout << "Live resize drawing." << std::endl;
    }
    else
    {
        // Is only gonna draw if necessary.
        axApp::MainInstance->GetCore()->DrawGLScene();
        glFlush();
    }

}


@end