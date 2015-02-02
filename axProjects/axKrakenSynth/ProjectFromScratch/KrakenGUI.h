#ifndef __KrakenGUI__
#define __KrakenGUI__

#include "axLib.h"

class KrakenGUI: public axPanel
{
public:
    KrakenGUI(axWindow* parent,
              const axRect& rect);
    
private:
    axImage* _woodside;
    // Events.
    virtual void OnPaint();
};

#endif // __KrakenGUI__