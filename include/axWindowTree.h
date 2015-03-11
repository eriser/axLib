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
#ifndef __AX_WINDOW_TREE__
#define __AX_WINDOW_TREE__

/// @defgroup Core
/// @{

#include "axC++.h"
#include <map>
#include <deque>

class axWindow;

/***************************************************************************//*
 * axWindowNode
 *****************************************************************************/
class axWindowNode
{
public:
	axWindowNode(axWindow* win);

	axWindowNode();
    
    ~axWindowNode();

	axWindow* GetWindow();

	void SetWindow(axWindow* win);

	void AddWindow(axWindow* win);
	void DeleteWindow(axWindow* win);

	axWindowNode* Get(axWindow* win);

	vector<axWindowNode*>& GetChild();

	axWindow* window;
    
    void DrawNode();

private:
	vector<axWindowNode*> _childNodes;
};


/// @todo Find next window with past position.

/***************************************************************************//*
 * axWindowTree
 *****************************************************************************/
class axWindowTree
{
public:
	axWindowTree();

	~axWindowTree();

	/** 
	*  @brief Get all window parent's from top to bottom.
	*		  Index [0] is the older parent of the window after 
	*		  frame window. So if the window is a direct child from
	*		  frame window the size of the deque is null.
	*/
    std::deque<axWindow*> GetWindowParents(axWindow* win);

	axWindowNode* FindWinNode(axWindow* win);

	void AddWindow(axWindow* win);

	void DeleteWindow(axWindow* win);

	/**
	*  @brief Get the axWindowNode of the axWindow.
	*/
	axWindowNode* Get(axWindow* win);

	/**
	*  @brief Find the window on wich the point is over.
	*/
	axWindow* FindMousePosition(const axPoint& pos);

    std::vector<axWindowNode*> GetMainNode();
    
    void DrawTree();

private:
    std::vector<axWindowNode*> _nodes;
};

/// @}
#endif //__AX_WINDOW_TREE__