#include "axImage.h"

axImageGlobalMapLoader axImage::_globalMap;

axImageGlobalMapLoader::axImageGlobalMapLoader()
{

}

bool axImageGlobalMapLoader::LoadImage(const string& path, GLuint& _texture, axSize& _size)
{
	std::map<std::string, axImageStruct>::iterator it = _imageMap.find(path);

	if (it != _imageMap.end())
	{
		_texture = it->second._texture;
		_size = it->second._size;
		return true;
	}
	else
	{
		if (InitImage(path, _texture, _size) == false)
		{
			axImageStruct img_info(_texture, _size);
			_imageMap.insert(pair<std::string, axImageStruct>(path, img_info));
			return true;
		}
	}

	return false;
}

bool axImageGlobalMapLoader::InitImage(const string& path, 
									   GLuint& _texture, 
									   axSize& _size)
{
	// http://zarb.org/~gc/html/libpng.html.
	// http://stackoverflow.com/questions/11296644/loading-png-textures-to-opengl-with-libpng-only.
	// Reading png.
	png_byte header[8];	

	FILE *fp = fopen(path.c_str(), "rb");

	if (fp == NULL)
	{
		cerr << "Error opening png image : " << path << endl;
		cerr << "Error : " << strerror(errno) << endl;
		return 1;
	}

	// Read header.
	fread(header, 1, 8, fp);

	if (png_sig_cmp(header, 0, 8))
	{
		cerr << "Error invalid image format." << endl;
		fclose(fp);
		return true;
	}

	// Read png struct.
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
		NULL, NULL, NULL);
	if (!png_ptr)
	{
		cerr << "Error png_create_read_struct." << endl;
		fclose(fp);
		return true;
	}

	// Create png info struct.
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		cerr << "Error png_create_info_struct." << endl;
		png_destroy_read_struct(&png_ptr,
			(png_infopp)NULL,
			(png_infopp)NULL);
		fclose(fp);
		return true;
	}

	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info)
	{
		cerr << "Error png_create_info_struct." << endl;
		png_destroy_read_struct(&png_ptr,
			&info_ptr,
			(png_infopp)NULL);
		fclose(fp);
		return true;
	}

	// Gets called if libpng encounters an error.
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		cerr << "Error libpng." << endl;
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return true;
	}

	// Init png reading.
	png_init_io(png_ptr, fp);

	// Let libpng know you already read the first 8 bytes.
	png_set_sig_bytes(png_ptr, 8);

	// Read all info up to the image data.
	png_read_info(png_ptr, info_ptr);

	// Variables to get info.
	int depth, color_type;
	png_uint_32 w, h;

	// Get info about png.
	png_get_IHDR(png_ptr, info_ptr, &w, &h, &depth,
		&color_type, NULL, NULL, NULL);

	//    0       1,2,4,8,16  Each pixel is a grayscale sample.

	//    2       8,16        Each pixel is an R,G,B triple.

	//    3       1,2,4,8     Each pixel is a palette index;
	//                         a PLTE chunk must appear.

	//    4       8,16        Each pixel is a grayscale sample,
	//                         followed by an alpha sample.

	//    6       8,16        Each pixel is an R,G,B triple,
	//                         followed by an alpha sample.

	_size = axSize(w, h);

	// Update png info struct.
	png_read_update_info(png_ptr, info_ptr);

	// Row size in bytes.
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	// OpenGL glTexImage2d requires rows to be 4-byte aligned.
	rowbytes += 3 - ((rowbytes - 1) % 4);

	// Allocate the image_data as a big block, to be given to opengl.
	png_byte* image_data;
	image_data = (png_byte*)malloc(rowbytes * _size.y * sizeof(png_byte)+15);

	if (image_data == NULL)
	{
		cerr << "Error memory allocation for PNG image data." << endl;
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

		fclose(fp);
		return true;
	}

	// Pointing to image_data for reading the png with libpng.
	png_bytep* row_pointers = (png_bytep*)malloc(_size.y * sizeof(png_bytep));

	if (row_pointers == NULL)
	{
		cout << "Error memory allocation for PNG row pointers." << endl;
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

		free(image_data);
		fclose(fp);
		return 1;
	}

	// Set each row_pointer at the correct offsets of image_data.
	for (int i = 0; i < _size.y; ++i)
	{
		row_pointers[_size.y - 1 - i] = image_data + i * rowbytes;
	}

	// Read png into image_data through row_pointers.
	png_read_image(png_ptr, row_pointers);

	// Generate OpenGl texture object.
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);

	if (color_type == 2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _size.x, _size.y,
			0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _size.x, _size.y,
			0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	}

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		cout << "GL FUCKUP" << endl;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// clean up.
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	free(image_data);
	free(row_pointers);
	fclose(fp);

	return false;
}


axImage::axImage(const string& path)
{
	_path = path;

	if_not_empty(_path)
	{
		if (_globalMap.LoadImage(path, _texture, _size))
		{
			_path = path;
		}
		else
		{
			cerr << "Error opening image" << endl;
		}
	/*	if_error_in(InitImage(path))
		{
			cerr << "Error opening image" << endl;
		}*/
	}
}
//
//bool axImage::InitImage(const string& path)
//{
//	// http://zarb.org/~gc/html/libpng.html.
//	// http://stackoverflow.com/questions/11296644/loading-png-textures-to-opengl-with-libpng-only.
//	// Reading png.
//	png_byte header[8];
//
//	FILE *fp = fopen(path.c_str(), "rb");
//
//	if (fp == NULL)
//	{
//		cerr << "Error opening png image : " << path << endl;
//		cerr << "Error : " << strerror(errno) << endl;
//		return 1;
//	}
//
//	// Read header.
//	fread(header, 1, 8, fp);
//
//	if (png_sig_cmp(header, 0, 8))
//	{
//		cerr << "Error invalid image format." << endl;
//		fclose(fp);
//		return true;
//	}
//
//	// Read png struct.
//	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
//		NULL, NULL, NULL);
//	if (!png_ptr)
//	{
//		cerr << "Error png_create_read_struct." << endl;
//		fclose(fp);
//		return true;
//	}
//
//	// Create png info struct.
//	png_infop info_ptr = png_create_info_struct(png_ptr);
//	if (!info_ptr)
//	{
//		cerr << "Error png_create_info_struct." << endl;
//		png_destroy_read_struct(&png_ptr,
//			(png_infopp)NULL,
//			(png_infopp)NULL);
//		fclose(fp);
//		return true;
//	}
//
//	png_infop end_info = png_create_info_struct(png_ptr);
//	if (!end_info)
//	{
//		cerr << "Error png_create_info_struct." << endl;
//		png_destroy_read_struct(&png_ptr,
//			&info_ptr,
//			(png_infopp)NULL);
//		fclose(fp);
//		return true;
//	}
//
//	// Gets called if libpng encounters an error.
//	if (setjmp(png_jmpbuf(png_ptr)))
//	{
//		cerr << "Error libpng." << endl;
//		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
//		fclose(fp);
//		return true;
//	}
//
//	// Init png reading.
//	png_init_io(png_ptr, fp);
//
//	// Let libpng know you already read the first 8 bytes.
//	png_set_sig_bytes(png_ptr, 8);
//
//	// Read all info up to the image data.
//	png_read_info(png_ptr, info_ptr);
//
//	// Variables to get info.
//	int depth, color_type;
//	png_uint_32 w, h;
//
//	// Get info about png.
//	png_get_IHDR(png_ptr, info_ptr, &w, &h, &depth,
//				 &color_type, NULL, NULL, NULL);
//
//	//    0       1,2,4,8,16  Each pixel is a grayscale sample.
//
//	//    2       8,16        Each pixel is an R,G,B triple.
//
//	//    3       1,2,4,8     Each pixel is a palette index;
//	//                         a PLTE chunk must appear.
//
//	//    4       8,16        Each pixel is a grayscale sample,
//	//                         followed by an alpha sample.
//
//	//    6       8,16        Each pixel is an R,G,B triple,
//	//                         followed by an alpha sample.
//
//	_size = axSize(w, h);
//
//	// Update png info struct.
//	png_read_update_info(png_ptr, info_ptr);
//
//	// Row size in bytes.
//	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
//
//	// OpenGL glTexImage2d requires rows to be 4-byte aligned.
//	rowbytes += 3 - ((rowbytes - 1) % 4);
//
//	// Allocate the image_data as a big block, to be given to opengl.
//	png_byte* image_data;
//	image_data = (png_byte*)malloc(rowbytes * _size.y * sizeof(png_byte)+15);
//
//	if (image_data == NULL)
//	{
//		cerr << "Error memory allocation for PNG image data." << endl;
//		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
//
//		fclose(fp);
//		return true;
//	}
//
//	// Pointing to image_data for reading the png with libpng.
//	png_bytep* row_pointers = (png_bytep*)malloc(_size.y * sizeof(png_bytep));
//
//	if (row_pointers == NULL)
//	{
//		cout << "Error memory allocation for PNG row pointers." << endl;
//		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
//
//		free(image_data);
//		fclose(fp);
//		return 1;
//	}
//
//	// Set each row_pointer at the correct offsets of image_data.
//	for (int i = 0; i < _size.y; ++i)
//	{
//		row_pointers[_size.y - 1 - i] = image_data + i * rowbytes;
//	}
//
//	// Read png into image_data through row_pointers.
//	png_read_image(png_ptr, row_pointers);
//
//	// Generate OpenGl texture object.
//	glGenTextures(1, &_texture);
//	glBindTexture(GL_TEXTURE_2D, _texture);
//
//	if (color_type == 2)
//	{
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _size.x, _size.y,
//			0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
//	}
//	else
//	{
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _size.x, _size.y,
//			0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
//	}
//
//	GLenum err = glGetError();
//	if (err != GL_NO_ERROR)
//	{
//		cout << "GL FUCKUP" << endl;
//	}
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//
//	// clean up.
//	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
//	free(image_data);
//	free(row_pointers);
//	fclose(fp);
//	return false;
//}

GLuint axImage::GetTexture()
{
	return _texture;
}

axSize axImage::GetSize()
{
	return _size;
}

int axImage::GetHeight()
{
	return _size.y;
}

int axImage::GetWidth()
{
	return _size.x;
}

bool axImage::IsImageReady()
{
	return (_path != "");
}