#include "axColor.h"
#include <stdio.h>

axColor::axColor(const axFloat& r, const axFloat& g, const axFloat& b):
		_r(r), _g(g), _b(b)
{
}

axColor::axColor(const axColorStruct& color):
		_r(color.r), _g(color.g), _b(color.b)
{

}

axColor::axColor() :
_r(0.0), _g(0.0), _b(0.0)
{

}

axColor::axColor(const string& color)
{
	string c = color;

	unsigned int r_i, g_i, b_i;

	sscanf(c.substr(1, 2).c_str(), "%x", &r_i);
	sscanf(c.substr(3, 2).c_str(), "%x", &g_i);
	sscanf(c.substr(5, 2).c_str(), "%x", &b_i);

	_r = axFloat(r_i / 255.0);
	_g = axFloat(g_i / 255.0);
	_b = axFloat(b_i / 255.0);
}

axFloat axColor::GetRed() const
{
	return _r;
}

axFloat axColor::GetGreen() const
{
	return _g;
}

axFloat axColor::GetBlue() const
{
	return _b;
}

axColor::axColor(const axColor& col)
{
	_r = col._r;
	_g = col._g;
	_b = col._b;
}


axColorStruct axColor::GetColorStruct() const
{
	return axColorStruct(_r, _g, _b);
}