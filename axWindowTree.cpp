#include "axWindowTree.h"

axWindowNode::axWindowNode(axWindow* win)
{
	window = win;
}

axWindowNode::axWindowNode()
{
}

axWindow* axWindowNode::GetWindow()
{
	return window;
}

void axWindowNode::SetWindow(axWindow* win)
{
	window = win;
}

void axWindowNode::AddWindow(axWindow* win)
{
	_childNodes.push_back(new axWindowNode(win));
}

axWindowNode* axWindowNode::Get(axWindow* win)
{
	for (axWindowNode* it : _childNodes)
	{
		if (it->window == win)
		{
			return it;
		}
	}
	return nullptr;
}

vector<axWindowNode*>& axWindowNode::GetChild()
{
	return _childNodes;
}

//-----------------------------------------------------------------------------
// axWindowTree
//-----------------------------------------------------------------------------
axWindowTree::axWindowTree()
{
}

deque<axWindow*> axWindowTree::GetWindowParents(axWindow* win)
{
	deque<axWindow*> windows;

	while_not_null(win->GetParent())
	{
		win = win->GetParent();
		windows.push_front(win);
	}

	return windows;
}

axWindowNode* axWindowTree::FindWinNode(axWindow* win)
{
	deque<axWindow*> windows = GetWindowParents(win);

	if (windows.size() == 0)
	{
		return Get(win);
	}
	else
	{
		axWindowNode* parent = nullptr;

		// Find parent node.
		if_not_null(parent = Get(windows[0]))
		{
			windows.pop_front();

			for (axWindow* it : windows)
			{
				parent = parent->Get(it);
				if (parent == nullptr)
				{
					return nullptr;
				}
			}

			if (parent == nullptr)
			{
				return nullptr;
			}
			return parent->Get(win);
		}

		// Didn't find the window.
		return nullptr;
	}
}

void axWindowTree::AddWindow(axWindow* win)
{
	deque<axWindow*> windows = GetWindowParents(win);

	// If there's no node in the nodes vector 
	// then it must be the first one to be added
	// with nullptr parent.
	if (_nodes.size() == 0 && windows.size() == 0)
	{
		_nodes.push_back(new axWindowNode(win));
	}
	else if (windows.size() == 0)
	{
		_nodes.push_back(new axWindowNode(win));
	}
	else
	{
		axWindowNode* node = FindWinNode(win->GetParent());
		if_not_null(node)
		{
			node->AddWindow(win);
		}
	}
}

void axWindowTree::DeleteWindow(axWindow* win)
{

}

vector<axWindowNode*> axWindowTree::GetMainNode()
{
	return _nodes;
}

axWindowNode* axWindowTree::Get(axWindow* win)
{
	for (axWindowNode* it : _nodes)
	{
		if (it->window == win)
		{
			return it;
		}
	}
	return nullptr;
}

axWindow* axWindowTree::FindMousePosition(const axPoint& pos)
{
	axWindowNode* node = nullptr;

	// Find first level window.
	for (axWindowNode* it : _nodes)
	{
		if (it->window->GetAbsoluteRect().IsPointInside(pos))
		{
			node = it;
		}
	}

	axWindowNode* n = node;
	if_not_null(n)
	{
		do
		{
			n = node;
			for (axWindowNode* it : n->GetChild())
			{
				if (it->window->GetAbsoluteRect().IsPointInside(pos))
				{
					node = it;
					break;
				}
			}

			//node = n;
		} while (n != node);
	}

	if_not_null(node)
	{
		return node->window;
	}

	return nullptr;
}