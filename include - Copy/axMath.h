/** @defgroup Graphic
 *  @{
 */
#ifndef __AX_MATH__
#define __AX_MATH__

//#include "axUtils.h"
#include "axC++.h"

void displayModelviewMatrix(float MV[16]);

//template<typename T>
class axMatrix4
{
public:
    axMatrix4();
    
    axMatrix4(double* data);
    
    axMatrix4(GLuint matrix);
    
    double* GetData();
    
    axMatrix4& Identity();
    
    axMatrix4& Translate(const axPoint& vec);
    
    axMatrix4& Multiply(const axMatrix4& mat);
    
    void Process();
    
//    void Save();
    
    void Load();
    
private:
    double _data[16];
};

void axOrtho2D(double* data, const axSize& size);

#endif // __AX_MATH__

/** @} */ // Graphic group.