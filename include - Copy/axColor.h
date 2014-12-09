/// @defgroup Graphic 
/// @{
 
#ifndef __AX_COLOR__
#define __AX_COLOR__

#include "axC++.h"
#include <fstream>

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
    
    void SetRed(const double& red);
    void SetGreen(const double& green);
    void SetBlue(const double& blue);
    void SetAlpha(const double& alpha);
    
    void SerializeOutput(fstream& out);
    void SerializeInput(fstream& in);

private:
	axDouble _r, _g, _b, _a;
};


#endif //__AX_COLOR__

/// @}