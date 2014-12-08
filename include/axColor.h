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
    
    
    void SerializeOutput(fstream& out);
    void SerializeInput(fstream& in);
//    fstream& operator >> (fstream& in);
//    friend ifstream& operator >> (fstream& in, axColor& color);
    
//    ofstream& operator << (ofstream& out);
//    friend ofstream& operator << (ofstream& out, const axColor& color);

private:
	axDouble _r, _g, _b, _a;
};


#endif //__AX_COLOR__

/// @}