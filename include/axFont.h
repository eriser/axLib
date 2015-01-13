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
#ifndef __AX_FONT__
#define __AX_FONT__

/// @defgroup Graphic
/// @{

#include <ft2build.h>
#include FT_FREETYPE_H

#include "axC++.h"
#include "axImage.h"
#include <map>
#include <fstream>

//struct axFontStruct
//{
//	axFontStruct(void* data, unsigned int size) :
//	_data(data), _size(size)
//	{
//	}
//
//	void* _data;
//	unsigned int _size;
//};
//
//class axFontGlobalManager
//{
//public:
//	axFontGlobalManager();
//
//	bool LoadFont(const string& path, FT_Face& face);
//
//private:
//	std::map<std::string, axFontStruct> _fontMap;
//	FT_Library _freeType;
//};


class axFont
{
public:
	axFont(const string& font);
    
    ~axFont();

	GLuint GetTexture();
    
    bool operator== (const bool& exist)
    {
        return _isReady == exist;
    }
    
    bool operator!= (const bool& exist)
    {
        return _isReady != exist;
    }
    
    bool IsFontReady() const
    {
        return _isReady;
    }
    
    operator bool() const
    {
        return _isReady;
    }
    
    // Char won't work if SetFontSize is not call.
	void SetFontSize(const int& size);
    
	void SetChar(const char& letter);
    
	axSize GetSize() const { return _size; }
    
	axPoint GetDelta() const { return _delta; }
    
	int GetNextPosition() const { return _next; }
    
	void SetFontType(const string& font_type);
    
	int GetFontSize() const
	{
		return _font_size;
	}

private:
	FT_Library _freeType;
	FT_Face _face;
	GLuint _texture;
	axSize _size;
	axPoint _delta;
	int _next;
	int _font_size;

    bool LoadFont(const string& path, FT_Face& face);
    
    bool _isReady;
};

/// @}
#endif // __AX_FONT__