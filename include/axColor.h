/// @defgroup Graphic 
/// @{
 
#ifndef __AX_COLOR__
#define __AX_COLOR__

#include "axC++.h"

struct axColorStruct
{
	axColorStruct(){}
	axColorStruct(const axFloat& r_, const axFloat& g_, const axFloat& b_):
		r(r_), g(g_), b(b_)
	{
	}


	axFloat r, g, b;
};

class axColor
{
public:
	axColor(const axFloat& r, const axFloat& g, const axFloat& b);
	axColor(const string& color);
	axColor(const axColorStruct& color);
	axColor();
	axColor(const axColor& col);

	axFloat GetRed() const;
	axFloat GetGreen() const;
	axFloat GetBlue() const;
	axColorStruct GetColorStruct() const;

private:
	axFloat _r, _g, _b;
};

#endif //__AX_COLOR__

/// @}