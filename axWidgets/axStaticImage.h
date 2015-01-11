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
#ifndef __AX_STATIC_IMAGE__
#define __AX_STATIC_IMAGE__

/// @defgroup Widgets
/// @{

/// @defgroup StaticImage
/// @{

#include "axWindow.h"
#include "axImage.h"
#include "axWidget.h"
#include "axObjectLoader.h"

/*******************************************************************************
 * axStaticImage.
 ******************************************************************************/
class axStaticImage: public axPanel
{
public:
    axStaticImage(axWindow* parent,
                  const axRect& rect,
                  const std::string& path);
    
    axStaticImage(axWindow* parent,
                  const axRect& rect,
                  axImage* img);
    
    
    /***************************************************************************
     * axStaticImageBuilder.
     **************************************************************************/
    class axStaticImageBuilder
    {
    public:
        axStaticImageBuilder(axWindow* win);
        
        axStaticImage* Create(axVectorPairString attributes);
        
    private:
        axWindow* _parent;
        std::string _path;
        axRect _rect;
        axSize _size;
        axImage* _img;
    };
    
private:
    axImage* _img;
    
    // Events.
    virtual void OnPaint();
};


/// @}
/// @}
#endif // __AX_STATIC_IMAGE__.
