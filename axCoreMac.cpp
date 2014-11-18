//
//  axCore.cpp
//  test
//
//  Created by Caroline Ross on 2014-04-24.
//  Copyright (c) 2014 alexarse. All rights reserved.
//

#include "axCoreMac.h"

/// @todo Put in axLib core lib.
#include "axCocoaInterfaceMac.h"

void axCoreMac::UpdateAll()
{
    axCore::UpdateAll();
//    std::cout << "UpdateAll" << std::endl;
    TestFunctionInterface();
}

std::string axCoreMac::OpenFileDialog()
{
    return axOpenFileDialog();
}

//void axMacCore::clear(float r, float g, float b,
//                      float a, bool depth)
//{
//    glClearColor(r, g, b, a);
//    
//    if (depth)
//    {
//        glClear(GL_COLOR_BUFFER_BIT);
//    }
//}
//
//void axMacCore::flush()
//{
//    glFlush();
//}
//
//void axMacCoreImplementation::init()
//{
//    shift_direction = 1;
//    shift = 0.0f;
//}
//
//void axMacCoreImplementation::update()
//{
//#define SHIFT_MOVE 0.005f
//    if (shift_direction==1)
//    {
//        shift +=SHIFT_MOVE;
//        if (shift>=1.0)
//            shift_direction = 0;
//    } else
//    {
//        shift -=SHIFT_MOVE;
//        if (shift<=0.0)
//            shift_direction = 1;
//    }
//}
//
//void axMacCoreImplementation::render()
//{
//    
//    clear();
//    //draw_triangles();
//    flush();
//}
//
//void axMacCoreImplementation::draw_triangles()
//{
//    glColor3f(1.0f, 0.85f, 0.35f);
//    glBegin(GL_TRIANGLES);
//    glVertex3f( -1.0+shift,  1.0, 0.0);
//    glVertex3f( -1.0, -1.0, 0.0);
//    glVertex3f(  1.0, -1.0 ,0.0);
//    glColor3f(1.0f, 0.0f, 0.35f);
//    glVertex3f(  1.0-shift,  1.0, 0.0);
//    glVertex3f( -1.0, -1.0, 0.0);
//    glVertex3f(  1.0, -1.0 ,0.0);
//    glEnd();
//}