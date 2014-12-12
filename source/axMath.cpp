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
#include "axMath.h"

axMatrix4::axMatrix4()
{
	for (int i = 0; i < 16; i++)
	{
		_data[i] = 0.0;
	}
}

axMatrix4::axMatrix4(double* data)
{
    for(int i = 0; i < 16; i++)
    {
        _data[i] = data[i];
    }
}

axMatrix4::axMatrix4(GLuint matrix)
{
    double data[16];
    glGetDoublev(matrix, data);
    
    for(int i = 0; i < 16; i++)
    {
        _data[i] = data[i];
    }
}

double* axMatrix4::GetData()
{
    return _data;
}

void axMatrix4::Load()
{
    glLoadMatrixd(_data);
}

axMatrix4& axMatrix4::Identity()
{
    for(int i = 0; i < 16; i++)
    {
        _data[i] = 0.0;
    }
    
    _data[0] = 1.0;
    _data[5] = 1.0;
    _data[10] = 1.0;
    _data[15] = 1.0;
    
    return *this;
}

axMatrix4& axMatrix4::Translate(const axPoint& vec)
{
    axMatrix4 trans;
    trans.Identity();
    trans._data[12] = vec.x;
    trans._data[13] = vec.y;
    trans._data[14] = 0.0;
    
    Multiply(trans);
    
    return *this;
}

axMatrix4& axMatrix4::Multiply(const axMatrix4& mat)
{
    for (unsigned int i = 0; i < 16; i += 4)
    {
        for (unsigned int j = 0; j < 4; ++j)
        {
            _data[i + j] = (mat._data[i] * _data[j]) +
                           (mat._data[i + 1] * _data[j +  4]) +
                           (mat._data[i + 2] * _data[j +  8]) +
                           (mat._data[i + 3] * _data[j + 12]);
        }
    }
    
    return *this;
}

void axMatrix4::Process()
{
    glMultMatrixd(_data);
}

void axOrtho2D(double* data, const axSize& size)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    data[0] = 2.0 / double(size.x);
    data[4] = 0.0;
    data[8] = 0.0;
    data[12] = -1.0;
    
    data[1] = 0.0;
    data[5] = -2.0 / double(size.y);
    data[9] = 0.0;
    data[13] = 1.0;
    
    data[2] = 0.0;
    data[6] = 0.0;
    data[10] = -1.0;
    data[14] = 0.0;
    
    data[3] = 0.0;
    data[7] = 0.0;
    data[11] = 0.0;
    data[15] = 1.0;
    
    glMultMatrixd(data);
}



