/*******************************************************************************//**
 * @file	axC++.h
 * @author	Alexandre Arsenault <alexandre.arsenault@axlib.net>
 * @date	21/04/2014
 **********************************************************************************/
/// @defgroup Utils 
/// @{
#ifndef __AX_CPP__
#define __AX_CPP__

#ifdef __linux__
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif //__linux__

#ifdef _MSC_VER
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#endif //_MSC_VER

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>
#endif // __APPLE__

#include <stdint.h>
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

//#define SPACE << " " <<

/// @todo Make a template function Clamp.
#define axCLIP( v, min, max ) if( v < min ) v = min; \
							  else if (v > max) v = max;

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

template<typename T>
T axClamp(T d, T min, T max)
{
    const T t = d < min ? min : d;
    return t > max ? max : t;
}

//axDouble axClampFloatRange(axDouble value, const axFloatRange& range)
//{
//    return axClamp<axDouble>(value, range.left, range.right);
//}


// Flag.
typedef uint16_t axFlag;
bool axFlag_exist(const axFlag& FLAG, axFlag flags);

#define axFLAG_NONE 0
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

#endif //__AX_CPP__

/// @}