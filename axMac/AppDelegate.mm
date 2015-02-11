
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
#import "AppDelegate.h"

#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>

#include <iostream>
#include "axLib.h"

axApp* GlobalApp = nullptr;
axAppDelegate* GlobalAppDelegate = nullptr;

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
    
//    std::cout << "******************************* PREPARE OPENGL " << std::endl;
    
    // Synchronize buffer swaps with vertical refresh rate
    GLint swapInt = 1;
    [[self window] setAcceptsMouseMovedEvents:YES];
    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
    
#ifdef _AX_VST_APP_
    axApp* app = axApp::CreateApp();

    axVstCoreMac* vstCoreMac = static_cast<axVstCoreMac*>(app->GetCore());
    axVstCoreData* coreData = vstCoreMac->GetVstCoreData();
    
    if(coreData->appDelegate == nullptr)
    {
        coreData->appDelegate = (__bridge void*)GlobalAppDelegate;
    }

    axMain::MainEntryPoint(app);

    axAppDelegate* d = (__bridge axAppDelegate*)coreData->appDelegate;
    [d setNeedsDisplay:YES];
    
#else
    axEventManager::GetInstance();
    axApp* app = axApp::CreateApp();
    app->GetCore()->Init(axSize(500, 500));
    app->CallMainEntryFunction();
    app->CallAfterGUILoadFunction();
//    axMain::MainEntryPoint(app);

    
    [GlobalAppDelegate setNeedsDisplay:YES];
#endif // _AX_VST_APP_
}

-(void)awakeFromNib
{

}

-(id)MemberTestFunc
{
#ifdef _AX_VST_APP_
    axApp* app = axApp::CreateApp();
    axVstCoreMac* vstCoreMac = static_cast<axVstCoreMac*>(app->GetCore());
    axAppDelegate* delegate = (__bridge axAppDelegate*)vstCoreMac->GetCurrentAppDelegate();
////    axVstCoreData* coreData = vstCoreMac->GetVstCoreData();
//    
//    std::vector<axVstCoreData>* data = vstCoreMac->GetManagerVector();
//    for(auto& n : *data)
//    {
//        if(n.appDelegate != nullptr)
//        {
//            axAppDelegate* d = (__bridge axAppDelegate*)n.appDelegate;
//            [d setNeedsDisplay:YES];
//        }
//    }
    
//    axAppDelegate* d = (__bridge axAppDelegate*)coreData->appDelegate;
    [delegate setNeedsDisplay:YES];
    return delegate;
#else
    
    [GlobalAppDelegate setNeedsDisplay:YES];
    
    return self;
#endif // _AX_VST_APP_

}

- (void)windowDidResize:(NSEvent *)event
{
//    std::cout << "Resize. " << std::endl;
}

- (void)windowDidMove:(NSNotification *)notification
{
//    std::cout << "Mouve. " << std::endl;
}

- (void) setFrameSize:(NSSize)newSize
{

}

- (void) SetFrameSize:(NSSize)newSize
{
//    std::cout << "SetFrameSize : RESIZE. " << std::endl;
    
    [[GlobalAppDelegate window] setFrame:NSMakeRect(0.f, 0.f, 800,
                                                    newSize.height)
                                 display:YES animate:YES];
}


// Working.
-(void)mouseDown:(NSEvent *)event
{
    NSPoint locationInView = [self convertPoint:[event locationInWindow]
                                       fromView:nil];
    
    axPoint pos(locationInView.x, locationInView.y);
    
    // Double click.
    if (event.clickCount == 2)
    {
        axApp::MainInstance->GetPopupManager()->OnMouseLeftDoubleClick(pos);
        if(axApp::MainInstance->GetPopupManager()->IsEventReachWindow() == false)
        {
            axApp::MainInstance->GetWindowManager()->OnMouseLeftDoubleClick(pos);
        }
    }
    
    // Simple click.
    else
    {
        axApp::MainInstance->GetPopupManager()->OnMouseLeftDown(pos);
        
        if(axApp::MainInstance->GetPopupManager()->IsEventReachWindow() == false)
        {
            axApp::MainInstance->GetWindowManager()->OnMouseLeftDown(pos);
        }
    }
}

- (void) rightMouseDown: (NSEvent*) event
{
    NSPoint locationInView = [self convertPoint:[event locationInWindow]
                                       fromView:nil];
    
    axPoint pos(locationInView.x, locationInView.y);
    
    // Double click.
    if (event.clickCount == 2)
    {
//        axApp::MainInstance->GetPopupManager()->OnMouseRightDoubleClick(pos);
//        if(axApp::MainInstance->GetPopupManager()->IsEventReachWindow() == false)
//        {
//            axApp::MainInstance->GetWindowManager()->OnMouseRightDoubleClick(pos);
//        }
    }
    
    // Simple click.
    else
    {
        axApp::MainInstance->GetPopupManager()->OnMouseRightDown(pos);
        
        if(axApp::MainInstance->GetPopupManager()->IsEventReachWindow() == false)
        {
            axApp::MainInstance->GetWindowManager()->OnMouseRightDown(pos);
        }
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
//    NSLog(@"Mouse enter");
}

- (void)mouseExited:(NSEvent *)theEvent
{
//    NSLog(@"Mouse leave");
}

- (void)keyDown: (NSEvent *) event
{
    unsigned short key = [event keyCode];
    
//    std::cout << "KEY : " << key << std::endl;
    
    // BackSpace.
    if(key == 51)
    {
        axApp::MainInstance->GetWindowManager()->OnBackSpaceDown();
    }
    // Delete
    else if(key == 117)
    {
        axApp::MainInstance->GetWindowManager()->OnKeyDeleteDown();
    }
    // Enter.
    else if(key == 36)
    {
        axApp::MainInstance->GetWindowManager()->OnEnterDown();
    }
    // Left arrow.
    else if(key == 123)
    {
        axApp::MainInstance->GetWindowManager()->OnLeftArrowDown();
    }
    // Right arrow.
    else if(key == 124)
    {
        axApp::MainInstance->GetWindowManager()->OnRightArrowDown();
    }
    else
    {
        std::string str = [[event characters] UTF8String];
        axApp::MainInstance->GetWindowManager()->OnKeyDown(str[0]);
    }
    
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
//    std::cout << "Install run observer." << std::endl;
    
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
//        std::cout << "Live resize drawing." << std::endl;
    }
    else
    {
        // Is only gonna draw if necessary.
        core->DrawGLScene();
        glFlush();
    }

}

@end