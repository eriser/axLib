/********************************************************************************//**
 * @file	axUtils.h
 * @author	Alexandre Arsenault <alexandre.arsenault@polymtl.ca>
 * @date	19/07/2013
 ***********************************************************************************/

/// @defgroup Utils 
/// @{

#ifndef __AX_UTILS__
#define __AX_UTILS__

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <deque>
#include <algorithm>

#ifdef __linux__
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> 
#endif __linux__

using namespace std;

string axFloatToString( const double& value, int num_char = 4 );
string axIntToString(const int &value );

string OpenFileDialog(const string& app_name, string folder_path = "");

string axGetExtension(const string& path);

template < typename T >
struct axRange
{
    axRange() {}
    axRange( const T& minimum, const T& maximum ) :
             left( minimum ), right( maximum ) {}

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


template < typename CPos, typename CSize, typename T >
class axTemplateRect
{
public:
    axTemplateRect( CPos pt_ = CPos(0, 0), CSize size_ = CSize(0, 0) ):
                   size( size_ ), position( pt_ ){}

	axTemplateRect(T x, T y, T w, T h): position(x, y), size(w, h){}

	bool IsPointInside(const CPos& pos)
	{
		return	pos.x >= position.x &&
				pos.x <= position.x + size.x &&
				pos.y >= position.y &&
				pos.y <= position.y + size.y;
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
};

//=========================================================================
//= Multiplicative LCG for generating uniform(0.0, 1.0) random numbers    =
//=   - x_n = 7^5*x_(n-1)mod(2^31 - 1)                                    =
//=   - With x seeded to 1 the 10000th x value should be 1043618065       =
//=   - From R. Jain, "The Art of Computer Systems Performance Analysis," =
//=     John Wiley & Sons, 1991. (Page 443, Figure 26.2)                  =
//=========================================================================
double rand_val(int seed);

//===========================================================================
//=  Function to generate normally distributed random variable using the    =
//=  Box-Muller method                                                      =
//=    - Input: mean and standard deviation                                 =
//=    - Output: Returns with normally distributed random variable          =
//===========================================================================
double axNormalDistributionRandomGenerator(const double& mean, 
										   const double& std_dev);

#endif

/// @}
