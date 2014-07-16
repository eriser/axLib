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

	bool InitImage(const string& path);
};

#endif //__AX_IMAGE__

/// @}