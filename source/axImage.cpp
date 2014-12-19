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
#include "axImage.h"
#include "png.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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
    glGenTextures(1, &_texture);
    
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
	int rowbytes = (int)png_get_rowbytes(png_ptr, info_ptr);

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

	// Bind texture object.
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
	}
}

axImage::axImage(void* data, const axSize& size):
_size(size),
_path("tmp")
{
    glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_2D, _texture);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _size.x, _size.y,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

}

bool axImage::SaveImage(const std::string& path)
{
    FILE * fp;
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    png_byte** row_pointers = nullptr;
    
    /* "status" contains the return value of this function. At first
     it is set to a value which means 'failure'. When the routine
     has finished its work, it is set to a value which means
     'success'. */
    int status = -1;
    /* The following number is set by trial and error only. I cannot
     see where it it is documented in the libpng manual.
     */
    int pixel_size = 4;
    int depth = 8;
    
    fp = fopen(path.c_str(), "wb");
    
    if (!fp)
    {
        std::cerr << "axImage::SaveImage : Can't open file : " << path << std::endl;
        return false;
    }
    
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    
    if (png_ptr == nullptr)
    {
        fclose(fp);
        return false;
    }
    
    info_ptr = png_create_info_struct(png_ptr);
    
    if (info_ptr == nullptr)
    {
        png_destroy_write_struct (&png_ptr, &info_ptr);
        fclose (fp);
        return false;
//        goto png_create_info_struct_failed;
    }
    
    // Set up error handling.
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_write_struct (&png_ptr, &info_ptr);
        fclose (fp);
        return false;
    }
    
    // Set image attributes.
    png_set_IHDR(png_ptr,
                 info_ptr,
                 _size.x,
                 _size.y,
                 depth,
                 PNG_COLOR_TYPE_RGBA,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
    
    // Initialize rows of PNG.
    
    //0????????????????????????????????????????????????
    row_pointers = (png_byte**)png_malloc(png_ptr, _size.y * sizeof(png_byte*));
    
    
    // Get OpenGL texture data.
    glBindTexture(GL_TEXTURE_2D, _texture);
    
    unsigned char* data = new unsigned char[_size.x * _size.y * 4];
    glGetTexImage(GL_TEXTURE_2D,
                  0,
                  GL_RGBA, // Format.
                  GL_UNSIGNED_BYTE, // Type.
                  (void*)data);
    
//    unsigned char* data_ptr = data + sizeof(unsigned char) * _size.x * _size.y * 4;
    unsigned char* data_ptr = data;
    for (int y = 0; y < _size.y; ++y)
    {
        png_byte* row = (png_byte*)
        png_malloc (png_ptr, sizeof(uint8_t) * _size.x * pixel_size);
        
        row_pointers[y] = row;
        
        for (int x = 0; x < _size.x; ++x)
        {
            data_ptr = data + _size.x * (_size.y - 1 - y) * 4 + x * 4;
            
            *row++ = *data_ptr++;
            *row++ = *data_ptr++;
            *row++ = *data_ptr++;
            *row++ = *data_ptr++;
        }
    }
    
    // Write the image data to "fp".
    
    png_init_io(png_ptr, fp);
    png_set_rows(png_ptr, info_ptr, row_pointers);
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
    
    /* The routine has successfully written the file, so we set
     "status" to a value which indicates success. */
    
    status = 0;
    
    for (int y = 0; y < _size.y; y++)
    {
        png_free(png_ptr, row_pointers[y]);
    }
    png_free (png_ptr, row_pointers);
    
//png_failure:
//png_create_info_struct_failed:
    png_destroy_write_struct (&png_ptr, &info_ptr);
    
//png_create_write_struct_failed:
    fclose (fp);
    
//fopen_failed:
    return true; // status.
}

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