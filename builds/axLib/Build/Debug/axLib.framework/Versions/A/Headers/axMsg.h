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
#ifndef __axManager__axMsg__
#define __axManager__axMsg__

/// @defgroup Core
/// @{

//#include "axWindow.h"

class axMsg
{
public:
    axMsg();
    
    virtual ~axMsg();
    
    virtual axMsg* GetCopy() = 0;
    
};

//template<typename T>
//class axSenderMsg : public axMsg
//{
//public:
//    axSenderMsg(T* sender)
//    {
//        _sender = sender;
//    }
//    
//    virtual ~axSenderMsg()
//    {
//        
//    }
//    
//    T* GetSender() const
//    {
//        return _sender;
//    }
//    
//    virtual axMsg* GetCopy()
//    {
//         return new axSenderMsg(*this);
//    };
//    
//private:
//    T* _sender;
//    
//};

/// @}
#endif /* defined(__axManager__axMsg__) */
