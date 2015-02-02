#ifndef __MINIMAL_PROJECT__
#define __MINIMAL_PROJECT__

#include "axLib.h"

class MyProject: public axPanel
{
public:
    MyProject(axWindow* parent,
              const axRect& rect);

    axEVENT_ACCESSOR(axButton::Msg, OnGenerate);
    axEVENT_ACCESSOR(axTextBox::Msg, OnEnterDown);
    
private:
    axTextBox* _textBox;
    axPlot* _plot;
    // Events.
    virtual void OnPaint();
    
    void OnGenerate(const axButton::Msg& msg);
    void OnEnterDown(const axTextBox::Msg& msg);
};

#endif // __MINIMAL_PROJECT__
