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
#ifndef __AX_IMAGE__
#define __AX_IMAGE__

/// @defgroup Graphic
/// @{

//#define PNG_SKIP_SETJMP_CHECK

/// @cond EXCLUDE_FROM_DOC 
#define PNG_DEBUG 3
/// @endcond



#include "axC++.h"
#include <string>
#include <stdlib.h>
#include <stdio.h>

class axImage;

struct axImageStruct
{
	axImageStruct(GLuint tex, axSize size) :
	_texture(tex), _size(size)
	{
	}

	GLuint _texture;
	axSize _size;
};

class axImageGlobalMapLoader
{
public:
	axImageGlobalMapLoader();

	bool LoadImage(const string& path, GLuint& _texture, axSize& _size);

private:
	std::map<std::string, axImageStruct> _imageMap;

	bool InitImage(const string& path, 
				   GLuint& _texture, 
				   axSize& _size);
};

class axImage
{
public:
	axImage(const string& path);
    
    axImage(void* data, const axSize& size);

	GLuint GetTexture();
	bool IsImageReady();
	axSize GetSize();
	int GetHeight();
	int GetWidth();
    
    bool SaveImage(const std::string& path);
    
    std::string GetImagePath() const
    {
        return _path;
    }

private:
	GLuint _texture;
	axSize _size;
    std::string _path;

	//bool InitImage(const string& path);

	static axImageGlobalMapLoader _globalMap;

	friend axImageGlobalMapLoader;
};

class axBigImage
{
public:
	axBigImage(const string& path);

	~axBigImage()
	{
		delete[] _imgData;
	}

	bool InitImage(const string& path);

	enum ColorType
	{
		RGB,
		RGBA
	};

	enum PixelDepth
	{
		UNSIGNED_BYTE
	};

	void* GetImageData()
	{
		return _imgData;
	}

	ColorType GetColorType() const
	{
		return _colorType;
	}

	PixelDepth GetPixelDepth() const
	{
		return _pixelDepth;
	}

	axSize GetImageSize() const
	{
		return _size;
	}

	bool IsImageReady() const
	{
		return _imgData != nullptr;
	}

private:
	std::string _path;
	axSize _size;
	void* _imgData;
	ColorType _colorType;
	PixelDepth _pixelDepth;
};

/// @}
#endif //__AX_IMAGE__

