

#include "axMath.h"

#include <iostream>
#include <iomanip>
using namespace std;

static const int Rx = 0;
static const int Ry = 1;
static const int Rz = 2;

static const int Ux = 4;
static const int Uy = 5;
static const int Uz = 6;

static const int Ax = 8;
static const int Ay = 9;
static const int Az = 10;

static const int Tx = 12;
static const int Ty = 13;
static const int Tz = 14;

void displayModelviewMatrix(float MV[16])
{
    int SPACING = 12;
    cout << left;
    cout << "\tMODELVIEW MATRIX\n";
    cout << "--------------------------------------------------" << endl;
    cout << setw(SPACING) << "R" << setw(SPACING) << "U" << setw(SPACING) << "A" << setw(SPACING) << "T" << endl;
    cout << "--------------------------------------------------" << endl;
    cout << setw(SPACING) << MV[Rx] << setw(SPACING) << MV[Ux] << setw(SPACING) << MV[Ax]  << setw(SPACING) << MV[Tx] << endl;
    cout << setw(SPACING) << MV[Ry] << setw(SPACING) << MV[Uy] << setw(SPACING) << MV[Ay]  << setw(SPACING) << MV[Ty] << endl;
    cout << setw(SPACING) << MV[Rz] << setw(SPACING) << MV[Uz] << setw(SPACING) << MV[Az] << setw(SPACING)  << MV[Tz] << endl;
    cout << setw(SPACING) << MV[3] << setw(SPACING) << MV[7] << setw(SPACING) << MV[11] << setw(SPACING) << MV[15] << endl;
    cout << "--------------------------------------------------" << endl;
    cout << endl;
}


axMatrix4::axMatrix4():
_data{0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0}
{
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



