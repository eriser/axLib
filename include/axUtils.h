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

#ifndef __AX_UTILS__
#define __AX_UTILS__

/*******************************************************************************
 * @file    axUtils.h
 * @author  Alexandre Arsenault <alx.arsenault@gmail.com>
 * @brief   Defines template classes for point and rectangle, clamp and
            interpolation.
 * @date    19/07/2013
 ******************************************************************************/

/// @defgroup Utils
/// @{

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <map>
#include <deque>
#include <algorithm>

#ifdef __linux__
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> 
#endif //__linux__

using namespace std;

#define DEPRECATED_ATTRIBUTE __attribute__((deprecated))

//[[deprecated]]
#pragma message("Use std::to_string instead.")
string axFloatToString( const double& value, int num_char = 4 );

//[[deprecated]]
#pragma message("Use std::to_string instead.")
string axIntToString(const int &value );
//
//[[deprecated]]
//#pragma message("Use the one from axApp instead.")
//string OpenFileDialog(const string& app_name, string folder_path = "");

string axGetExtension(const string& path);

typedef std::vector<std::pair<std::string, std::string>> axVectorPairString;
typedef std::vector<std::string> axStringVector;
axStringVector GetVectorFromStringDelimiter(const std::string& str,
                                            const std::string& delimiter);

template <typename T>
double axLineairInterpole(const T& y1,
                          const T& y2,
                          const T& mu)
{
    return y1 + mu * (y2 - y1);
}

enum axDirection
{
    axDIRECTION_LEFT,
    axDIRECTION_UP,
    axDIRECTION_RIGHT,
    axDIRECTION_DOWN
};

template<typename T>
T axClamp(T d, T min, T max)
{
    const T t = d < min ? min : d;
    return t > max ? max : t;
}

template < typename T >
struct axRange
{
    axRange() {}
    axRange( const T& minimum, const T& maximum ) :
             left( minimum ), right( maximum ) {}
    
    double GetZeroToOneValue(const double& value) const
    {
        double v = double(value - left) / double(right - left);
        if(v > 1.0) v = 1.0;
        if(v < 0.0) v = 0.0;
        
        return v;
    }
    
    double GetValueFromZeroToOne(const double& value)
    {
        double v = double(left) + value * double(right - left);
        if(v > right) v = right;
        if(v < left) v = left;
        return v;
    }

    T left, right;
};

template < typename T >
class axTemplate2DPoint
{
public:
	axTemplate2DPoint(T X = 0, T Y = 0) 
	{ 
		x = X; y = Y;
	}

	T x, y;

	axTemplate2DPoint operator + (const axTemplate2DPoint& pt) const
	{
		return axTemplate2DPoint(x + pt.x, y + pt.y);
	}

	axTemplate2DPoint operator - (const axTemplate2DPoint& pt) const
	{
        return axTemplate2DPoint(x - pt.x, y - pt.y);
	}

	axTemplate2DPoint& operator += (const axTemplate2DPoint& pt) 
	{
		x += pt.x; y += pt.y; return *this;
	}

	axTemplate2DPoint& operator -= (const axTemplate2DPoint& pt) 
	{
        x -= pt.x; y -= pt.y; return *this;
	}

	axTemplate2DPoint& operator = (const T& pt) 
	{
		x = pt; y = pt; return *this;
	}

	axTemplate2DPoint& operator = (const axTemplate2DPoint& pt) 
	{
		x = pt.x; y = pt.y; return *this;
	}

	bool operator == (const axTemplate2DPoint& pt) 
	{ return (x == pt.x && y == pt.y); }

	bool operator < (const axTemplate2DPoint& pt) 
	{ return (x < pt.x && y < pt.y); }

	bool operator <= (const axTemplate2DPoint& pt) 
	{ return (x <= pt.x && y <= pt.y); }

	bool operator > (const axTemplate2DPoint& pt) 
	{ return (x > pt.x && y > pt.y); }

	bool operator >= (const axTemplate2DPoint& pt) 
	{ return (x >= pt.x && y >= pt.y); }

	axTemplate2DPoint& operator()(const T& X, const T& Y)
	{
		x = X;
		y = Y;
		return *this;
	}
};

template<typename T>
struct axRectPoints
{
    axRectPoints(){}
    
    axRectPoints(const T& bl,
                 const T& tl,
                 const T& tr,
                 const T& br):
    bottom_left(bl),
    top_left(tl),
    top_right(tr),
    bottom_right(br)
    {
    }
    
    T bottom_left, top_left, top_right, bottom_right;
};

template < typename CPos, typename CSize, typename T >
class axTemplateRect
{
public:
    axTemplateRect( CPos pt_ = CPos(0, 0), CSize size_ = CSize(0, 0) ):
                   size( size_ ), position( pt_ ){}

	axTemplateRect(T x, T y, T w, T h): position(x, y), size(w, h){}

	axTemplateRect(const string& path)
	{
		Load(path);
	}

	CSize size;
	CPos position;							  

	axTemplateRect operator + (const axTemplateRect& rect) const
	{
		return axTemplateRect(position.x + rect.position.x, 
							  position.y + rect.position.y, 
			                  size.x + rect.size.x, 
							  size.y + rect.size.y);
	}

	axTemplateRect operator - (const axTemplateRect& rect) const
	{
		return axTemplateRect(position.x - rect.position.x, 
							  position.y - rect.position.y, 
							  size.x - rect.size.x, 
							  size.y - rect.size.y);
	}

	bool operator == (const axTemplateRect& rect) 
	{ 
		return (rect.position.x == position.x && 
				rect.position.y == position.y && 
				rect.size.x == size.x && 
				rect.size.y == size.y); 
	}

    bool operator != (const axTemplateRect& rect) 
	{ 
		return ( (rect.position.x != position.x || 
				  rect.position.y != position.y) ||
			   ( rect.size.x != size.x || 
			     rect.size.y != size.y ) ); 
	}
	
	bool operator < (const axTemplateRect& rect) 
	{ 
		return (size.x < rect.size.x && size.y < rect.size.y); 
	}
	
	bool operator <= (const axTemplateRect& rect) 
	{ 
		return (size.x <= rect.size.x && size.y <= rect.size.y); 
	}

	bool operator > (const axTemplateRect& rect) 
	{ 
		return (size.x > rect.size.x && size.y > rect.size.y); 
	}

	bool operator >= (const axTemplateRect& rect) 
	{ 
		return (size.x >= rect.size.x && size.y >= rect.size.y); 
	}
    
    bool IsPointInside(const CPos& pos)
    {
        return	pos.x >= position.x &&
        pos.x <= position.x + size.x &&
        pos.y >= position.y &&
        pos.y <= position.y + size.y;
    }
    
    axTemplateRect GetInteriorRect(const CPos& point) const
    {
        return axTemplateRect(point.x, point.y,
                      size.x - (T)(2.0 * point.x),
                      size.y - (T)(2.0 * point.y));
    }
    
    CPos GetTopLeft() const
    {
        return position;
    }
    
    CPos GetTopRight() const
    {
        return CPos(position.x + size.x, position.y);
    }
    
    CPos GetBottomLeft() const
    {
        return CPos(position.x, position.y + size.y);
    }
    
    CPos GetBottomRight() const
    {
        return CPos(position.x + size.x, position.y + size.y);
    }
    
    CPos GetMiddle() const
    {
        return CPos(position.x + size.x * 0.5, position.y + size.y * 0.5);
    }
    
	bool Load(const string& path)
	{
//		ifstream file;
//		file.open(path);
//
//		if (file.fail())
//		{
//			cerr << "Problem opening file " << path << endl;
//			return false;
//		}
//		else
//		{
//			if (!file.fail())
//			{
//				file >> position.x;
//				file >> position.y;
//				file >> size.x;
//				file >> size.y;
//			}
//		}
	}
    
    axRectPoints<CPos> GetPoints() const
    {
        return axRectPoints<CPos>(CPos(position.x, position.y + size.y),
                                  CPos(position.x, position.y),
                                  CPos(position.x + size.x, position.y),
                                  CPos(position.x + size.x, position.y + size.y));
    }
};

/// @}
#endif


