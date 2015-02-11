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
#ifndef __AX_COLOR__
#define __AX_COLOR__

/// @defgroup Graphic
/// @{

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
    
    void LoadFromString(const std::string& str);
    
    std::string ToString() const;

private:
	axDouble _r, _g, _b, _a;
};

/// @}
#endif //__AX_COLOR__