
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
#import "axVstAppDelegate.h"

#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>

#include <iostream>
#include "axLib.h"

//axApp* GlobalApp = nullptr;
//axVstAppDelegate* GlobalAppDelegate = nullptr;

static bool runLoopIsActive = false;

@implementation axVstAppDelegate

- (id)initWithFrame:(NSRect)frame
{
    if(!runLoopIsActive)
    {
        [self installRunLoopObserver];
        runLoopIsActive = true;
    }
    
    
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

}

- (void)ReInit
{
    if (self)
    {
        std::cout << "SELF exist." << std::endl;
        axVstCoreMac* vstCoreMac = static_cast<axVstCoreMac*>(axApp::MainInstance->GetCore());
        axVstAppDelegate* delegate = (__bridge axVstAppDelegate*)
                                     vstCoreMac->GetCurrentAppDelegate();
        
//        std::cout << "(void)ReInit currentIndex : " << vstCoreMac->GetCurrentManagerIndex() << std::endl;
        
        [[delegate openGLContext] makeCurrentContext];
        [delegate wantsBestResolutionOpenGLSurface];
        
        
        GLint swapInt = 1;
        [[delegate window] setAcceptsMouseMovedEvents:YES];
        [[delegate openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
        [delegate addTrackingRect:[delegate bounds] owner:delegate userData:NULL assumeInside:YES];
        [[delegate window] makeFirstResponder:delegate];
        [delegate setNeedsDisplay:YES];
        
        
//        [[self openGLContext] makeCurrentContext];
//        [self wantsBestResolutionOpenGLSurface];
//        
//        
//        GLint swapInt = 1;
//        [[self window] setAcceptsMouseMovedEvents:YES];
//        [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
//        [self addTrackingRect:[self bounds] owner:self userData:NULL assumeInside:YES];
//        [[self window] makeFirstResponder:self];
//        [self setNeedsDisplay:YES];
    }
}

- (void)prepareOpenGL
{
    // Synchronize buffer swaps with vertical refresh rate
    GLint swapInt = 1;
    [[self window] setAcceptsMouseMovedEvents:YES];
    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
    
    axApp* app = axApp::CreateApp();
    app->GetCore()->InitGL();

//    axVstCoreMac* vstCoreMac = static_cast<axVstCoreMac*>(app->GetCore());
//    axVstCoreData* coreData = vstCoreMac->GetVstCoreData();
    
//    if(coreData->appDelegate == nullptr)
//    {
//        coreData->appDelegate = (__bridge void*)GlobalAppDelegate;
//    }
    //coreData->appDelegate = (__bridge void*)self;
    
    
    axMain::MainEntryPoint(app);

    //axVstAppDelegate* d = (__bridge axVstAppDelegate*)coreData->appDelegate;
    [self setNeedsDisplay:YES];
}

-(void)awakeFromNib
{

}

-(id)MemberTestFunc
{
    axApp* app = axApp::CreateApp();
    axVstCoreMac* vstCoreMac = static_cast<axVstCoreMac*>(app->GetCore());
    
    axVstAppDelegate* delegate = (__bridge axVstAppDelegate*)
                                 vstCoreMac->GetCurrentAppDelegate();
    
    [[delegate openGLContext] makeCurrentContext];
    
    [delegate setNeedsDisplay:YES];
    
    std::cout << "-(id)MemberTestFunc currentIndex : " << vstCoreMac->GetCurrentManagerIndex() << std::endl;
    
    return delegate;
    
//    axApp* app = axApp::CreateApp();
//    axVstCoreMac* vstCoreMac = static_cast<axVstCoreMac*>(app->GetCore());
//    
//    axVstAppDelegate* delegate = (__bridge axVstAppDelegate*)vstCoreMac->GetCurrentAppDelegate();
//
//    [delegate setNeedsDisplay:YES];
//    return delegate;
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
    axApp* app = axApp::CreateApp();
    axVstCoreMac* vstCoreMac = static_cast<axVstCoreMac*>(app->GetCore());
    axVstAppDelegate* delegate = (__bridge axVstAppDelegate*)vstCoreMac->GetCurrentAppDelegate();
    
    [[delegate window] setFrame:NSMakeRect(0.f, 0.f, 800,
                                           newSize.height)
                                           display:YES animate:YES];
}


// Working.
-(void)mouseDown:(NSEvent *)event
{
//    std::cout << "-(void)mouseDown:(NSEvent *)event" << std::endl;

    NSPoint locationInView = [self convertPoint:[event locationInWindow]
                                       fromView:nil];
    
    axPoint pos(locationInView.x, locationInView.y);
    
//    std::cout << "Pos x = " << pos.x << " pos y = " << pos.y << std::endl;
    
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
    axApp* app = axApp::CreateApp();
    axVstCoreMac* vstCoreMac = static_cast<axVstCoreMac*>(app->GetCore());
    
    int index = 0;
    int i = 0;
    for(auto& n : *vstCoreMac->GetManagerVector())
    {
        if((__bridge void*)self == n.appDelegate)
        {
            index = i;
            break;
        }
        
        i++;
    }
    
    vstCoreMac->SetCurrentManagerIndex(index);
    axVstAppDelegate* delegate = (__bridge axVstAppDelegate*)
                                 vstCoreMac->GetCurrentAppDelegate();
    
    std::cout << "(void)mouseEntered currentIndex : " << vstCoreMac->GetCurrentManagerIndex() << std::endl;
    [[delegate openGLContext] makeCurrentContext];
//
//    [delegate addTrackingRect:[delegate bounds] owner:delegate userData:NULL assumeInside:YES];
//    [[delegate window] makeFirstResponder:delegate];

//    vstCoreMac->InitGL();
//    vstCoreMac->DrawGLScene();
    
//    vstCoreMac->InitGL();
//    vstCoreMac->ReInitApp();
    
    vstCoreMac->UpdateAll();
    
}

- (void)mouseExited:(NSEvent *)theEvent
{

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
    axVstCoreMac* vstCoreMac = static_cast<axVstCoreMac*>(axApp::MainInstance->GetCore());
    std::cout << "(void)drawRect currentIndex : " << vstCoreMac->GetCurrentManagerIndex() << std::endl;

    
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
//        [[self openGLContext] makeCurrentContext];
        // Is only gonna draw if necessary.
        core->DrawGLScene();
        glFlush();
    }

}


@end