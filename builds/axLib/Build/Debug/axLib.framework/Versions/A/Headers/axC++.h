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
#ifndef __AX_CPP__
#define __AX_CPP__

/// @defgroup Utils
/// @{

#ifdef __linux__
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif //__linux__

#ifdef _MSC_VER
#include <windows.h>
#include "GL/glew.h"
//#include <gl\gl.h>
#include <gl\glu.h>
#endif //_MSC_VER

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>
#endif // __APPLE__

#include <stdint.h>
#include <memory>
#include <functional>
#include "axUtils.h"

#define axDEBUG_LEVEL0 0
#define axDEBUG_MINIMAL_LEVEL 1
#define axDEBUG_MEDIUM_LEVEL 2
#define axDEBUG_HIGH_LEVEL 3

#define axDEBUG_LEVEL (axDEBUG_MINIMAL_LEVEL)

#if axDEBUG_LEVEL > 0
#define axREMOVE_ON_RELEASE(x) x
#else
#define axREMOVE_ON_RELEASE(x) 
#endif

//#define DSTREAM cout
#define DSTREAM(x) if(x <= axDEBUG_LEVEL) cout 

// If altenative.
#define if_error_in if
#define if_no_error_in if 
#define if_not_empty(str) if((str) != "")
#define if_not_null(ptr) if((ptr) != nullptr)
#define if_not_zero(var) if((var) != 0)

// While alternative.
#define while_not_null(ptr) while((ptr) != nullptr)

#define axOUTPUT_VARIABLE(v) std::cout << #v << " : " << v << std::endl;

// Standard type.
typedef float axFloat;
typedef double axDouble;
typedef int axInt;
typedef unsigned int axUint;
typedef long axLong;
typedef unsigned long axUlong;
typedef unsigned char axByte;

// Point.
typedef axTemplate2DPoint<axInt> axPoint;
typedef axTemplate2DPoint<double> axFloatPoint;

// Size.
typedef axTemplate2DPoint<axInt> axSize;
typedef axTemplate2DPoint<axDouble> axFloatSize;

// Rectangle.
typedef axTemplateRect<axPoint, axSize, axInt> axRect;
typedef axTemplateRect<axFloatPoint, axFloatSize, axDouble> axFloatRect;

// Range.
typedef axRange<axDouble> axFloatRange;
typedef axRange<axInt> axIntRange;

typedef axRectPoints<axTemplate2DPoint<axDouble>> axRectFloatPoints;

// Since variadic templates are recursive, must have a base case.
void axPrint();

template <typename T, typename ...P>
void axPrint(T t, P ...p)
{
    std::cout << t << ' ';
    {
        axPrint(p...);
    }
}

// Since variadic templates are recursive, must have a base case.
void axError();

template <typename T, typename ...P>
void axError(T t, P ...p)
{
    std::cerr << "Error : " << t << ' ';
    {
        axError(p...);
    }
}

// Flag.
typedef uint16_t axFlag;

typedef unsigned char axByte;

//[[deprecated]]
bool axFlag_exist(const axFlag& FLAG, axFlag flags);


bool IsFlag(const axFlag& FLAG, axFlag flags);

#define axNO_FLAG 0
#define axFLAG_NONE 0
#define axFLAG_0  0
#define axFLAG_1  1
#define axFLAG_2  2
#define axFLAG_3  4
#define axFLAG_4  8
#define axFLAG_5  16
#define axFLAG_6  32
#define axFLAG_7  64
#define axFLAG_8  128
#define axFLAG_9  256
#define axFLAG_10 512
#define axFLAG_11 1024
#define axFLAG_12 2048
#define axFLAG_13 4096
#define axFLAG_14 8192
#define axFLAG_15 16384
#define axFLAG_16 32768

/// @}
#endif //__AX_CPP__


