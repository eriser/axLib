#ifndef __MINIMAL_PROJECT__
#define __MINIMAL_PROJECT__

#include "axLib.h"

class MyLED : public axPanel
{
public:
    MyLED(axWindow* parent,
          const axRect& rect);
    
    void SetActive(const bool& on);
    void SetOff();
    
    bool IsActive() const
    {
        return (bool)_imgIndex;
    }
    
private:
    void OnPaint();
    int _imgIndex;
    
    
    axImage* _ledImg;
};

class MyNumberPanel : public axPanel
{
public:
    MyNumberPanel(axWindow* parent,
          const axPoint& pos);
    
    void SetNumber(const int& num);
    int GetNumber() const;
    
private:
    void OnPaint();
    int _number;
    axFont* _font;
};

#endif // __MINIMAL_PROJECT__
