#ifndef __AX_WINDOW_TREE__
#define __AX_WINDOW_TREE__

#include "axC++.h"
#include "axWindow.h"
#include <map>
#include <deque>

/***************************************************************************//*
 * axWindowNode
 *****************************************************************************/
class axWindowNode
{
public:
	axWindowNode(axWindow* win);

	axWindowNode();

	axWindow* GetWindow();

	void SetWindow(axWindow* win);

	void AddWindow(axWindow* win);
	void DeleteWindow(axWindow* win);

	axWindowNode* Get(axWindow* win);

	vector<axWindowNode*>& GetChild();

	axWindow* window;

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

	/** 
	*  @brief Get all window parent's from top to bottom.
	*		  Index [0] is the older parent of the window after 
	*		  frame window. So if the window is a direct child from
	*		  frame window the size of the deque is null.
	*/
	deque<axWindow*> GetWindowParents(axWindow* win);

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

	vector<axWindowNode*> GetMainNode();

private:
	vector<axWindowNode*> _nodes;
};

#endif //__AX_WINDOW_TREE__