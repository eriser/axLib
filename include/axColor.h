/// @defgroup Graphic 
/// @{
 
#ifndef __AX_COLOR__
#define __AX_COLOR__

#include "axC++.h"

class axColor
{
public:
	axColor(const axDouble& r, const axDouble& g, const axDouble& b);
    
    axColor(const axDouble& r, const axDouble& g,
            const axDouble& b, const axDouble& a);
    
	axColor();
	axColor(const axColor& col);
    axColor(const string& color);

	axDouble GetRed() const;
	axDouble GetGreen() const;
	axDouble GetBlue() const;
    axDouble GetAlpha() const;

private:
	axDouble _r, _g, _b, _a;
};


#endif //__AX_COLOR__

/// @}