//
//  axGraphicInterface.cpp
//  axLib
//
//  Created by Alexandre Arsenault on 2015-02-17.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "axGraphicInterface.h"
#include "axMath.h"
#include "axWindow.h"
#include "axApp.h"
#include "axConfig.h"

namespace axGraphicInterface
{
    void Resize(const axSize& size)
    {
        glViewport(0, 0, size.x, size.y);
        
        glMatrixMode(GL_PROJECTION);
        
        axMatrix4 proj;
        axOrtho2D(proj.Identity().GetData(), size);
        
        // Select the modelview matrix.
        glMatrixMode(GL_MODELVIEW);
    }
    
    void Init()
    {
        // Enable Smooth Shading.
        glShadeModel(GL_SMOOTH);
        
        // Black Background
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        
        // Depth Buffer Setup
        glClearDepth(1.0f);
        
        // Enables Depth Testing
        glEnable(GL_DEPTH_TEST);
        
        glEnable(GL_BLEND); // Enable blending.
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // The type of depth testing to do.
        glDepthFunc(GL_LEQUAL);
        
        // Really nice perspective calculations.
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    }
    
    void Draw(const axSize& size)
    {
        // Clear screen and depth buffer.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Set projection matrix.
        glMatrixMode(GL_PROJECTION);
        
        axMatrix4 proj;
        axOrtho2D(proj.Identity().GetData(), size);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        // Draw black rectangle.
        glColor4d(0.0, 0.0, 0.0, 1.0);
        
        // Draw black rectangle.
        axFloatRect rect(-1.0, -1.0, size.x * 2.0, size.y * 2.0);
        axRectFloatPoints points = rect.GetPoints();
        GLubyte indices[] = {0, 1,2, 2,3};
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_DOUBLE, 0, &points);
        glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_BYTE, indices);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
}

