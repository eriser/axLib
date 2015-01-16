/*******************************************************************************
 * Copyright (c) 2015 Alexandre Arsenault.
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
#ifndef __AX_WIDGET__
#define __AX_WIDGET__

#include "axPanel.h"

enum axParameterType
{
    axINT, axUINT, axLONG, axULONG,
    axFLOAT, axDOUBLE, axBOOL, axSTRING,
    axCOLOR, axRECT, axPOINT, axSIZE
};

class axWidget : public axPanel
{
public:
    axWidget(axWindow* parent, const axRect& rect);
    axWidget(int f, axWindow* parent, const axRect& rect);
    
    bool IsEditable() const;
    bool IsInfoEditable() const;
    bool AcceptChild() const;
    
    void SetInfoParam(const std::string& name, axVar param);
    void SetEditParam(const std::string& name, axVar param);
    
    const axStringVector& GetListOfEditInfoName() const;
    const axStringVector& GetListOfEditInfoParamType() const;
//    const axStringVector& GetListOfEditInfoParamValue() const;
    
    const axStringVector& GetListOfEditName() const;
    const axStringVector& GetListOfEditParamType() const;
//    const axStringVector& GetListOfEditParamValue() const;
    
private:
    bool _isEditable, _isInfoEditable, _acceptChild;
    std::string _name;
};

#endif //__AX_WIDGET__
