#include "axColor.h"
#include <stdio.h>

axColor::axColor(const axDouble& r, const axDouble& g, const axDouble& b):
		_r(r), _g(g), _b(b), _a(1.0)
{
}

axColor::axColor(const axDouble& r,
                 const axDouble& g,
                 const axDouble& b,
                 const axDouble& a):
_r(r), _g(g), _b(b), _a(a)
{
}

axColor::axColor() :
_r(0.0), _g(0.0), _b(0.0), _a(1.0)
{

}

axColor::axColor(const axColor& col)
{
    _r = col._r;
    _g = col._g;
    _b = col._b;
    _a = col._a;
}

axColor::axColor(const string& color)
{
	string c = color;

	unsigned int r_i, g_i, b_i;

	sscanf(c.substr(1, 2).c_str(), "%x", &r_i);
	sscanf(c.substr(3, 2).c_str(), "%x", &g_i);
	sscanf(c.substr(5, 2).c_str(), "%x", &b_i);

	_r = axDouble(r_i / 255.0);
	_g = axDouble(g_i / 255.0);
	_b = axDouble(b_i / 255.0);
    _a = 1.0;
}

axDouble axColor::GetRed() const
{
	return _r;
}

axDouble axColor::GetGreen() const
{
	return _g;
}

axDouble axColor::GetBlue() const
{
	return _b;
}

axDouble axColor::GetAlpha() const
{
    return _a;
}