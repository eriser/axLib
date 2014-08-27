/// @defgroup Graphic 
/// @{
#ifndef __AX_IMAGE__
#define __AX_IMAGE__

//#define PNG_SKIP_SETJMP_CHECK

/// @cond EXCLUDE_FROM_DOC 
#define PNG_DEBUG 3
/// @endcond

#include <png.h>

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

	GLuint GetTexture();
	bool IsImageReady();
	axSize GetSize();
	int GetHeight();
	int GetWidth();

private:
	GLuint _texture;
	axSize _size;
	string _path;

	//bool InitImage(const string& path);

	static axImageGlobalMapLoader _globalMap;

	friend axImageGlobalMapLoader;
};

#endif //__AX_IMAGE__

/// @}