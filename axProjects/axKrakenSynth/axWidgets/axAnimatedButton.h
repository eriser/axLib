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

#ifndef __AX_ANIMATED_BUTTON__
#define __AX_ANIMATED_BUTTON__

/*******************************************************************************
 * @file    axAnimatedButton.h
 * @author  Alexandre Arsenault <alx.arsenault@gmail.com>
 * @brief   axAnimatedButton.
 * @date    15/12/2014
 ******************************************************************************/

/// @defgroup Widgets
/// @{

/// @defgroup Button
/// @{

#include "axButton.h"
#include "axTimer.h"

/*******************************************************************************
 * axAnimatedButton.
 ******************************************************************************/
class axAnimatedButton : public axButton
{
public:
    axAnimatedButton(axWindow* parent,
                     const axRect& rect,
                     const axButton::Events& events,
                     string img_path = "",
                     string label = "");
    
    axEVENT_ACCESSOR(axTimerMsg, OnFadeTimer);
    
private:
    axTimer* _fadeTimer;
    double _fadeValue;
    int _nPastCurrentImg;
    std::mutex _fadeValueMutex;
    double _fadeTimeMs;
    
    void SetupTimer();
    
    virtual void OnPaint();
    virtual void OnMouseLeftDown(const axPoint& pos);
    virtual void OnMouseLeftUp(const axPoint& pos);
    virtual void OnMouseEnter();
    virtual void OnMouseLeave();
    
    void OnFadeTimer(const axTimerMsg& msg);
};

/// @}
/// @}
#endif //__AX_ANIMATED_BUTTON__




