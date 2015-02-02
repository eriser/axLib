#ifndef __MINIMAL_PROJECT__
#define __MINIMAL_PROJECT__

#include "axLib.h"

class WorldMap : public axPanel
{
public:
    WorldMap();
    
    void SetCoordinates(const std::vector<axFloatPoint>& coord);
    
    
    
private:
    axImage* _worldMap;
    std::vector<axFloatPoint> _coordVector;
    
    virtual void OnPaint();
};

#endif // __MINIMAL_PROJECT__
