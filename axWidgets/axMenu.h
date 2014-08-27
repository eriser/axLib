/********************************************************************************//**
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
 * licenses are available, visit www.axLibrary.com for more information.
 ***********************************************************************************/

#ifndef __AX_MENU__
#define __AX_MENU__

#include "axEvent.h"
#include "axPanel.h"
#include "axColor.h"
#include "axGC.h"
#include "axImage.h"

class axMenu;

enum axMenuNodeSelectionMode
{
    axMENU_SELECT_ANY = 0,
    axMENU_SELECT_ONLY_THIS_ID
};

/********************************************************************************//**
* axMenuNode.
***********************************************************************************/
class axMenuNode: public axPanel
{
public:
    // Root Node.
    axMenuNode(axApp* app, axWindow* parent, const axRect& rect);

    axMenuNode( axMenuNode* parent,
                const string& label,
                string img_path = "",
                axMenuNodeSelectionMode mode = axMENU_SELECT_ANY );

    /********************************************************************************//**
     * @brief  Add a node to a parent node
     * !IMPORTANT! Always add parent node to his parent before adding child node
     * EX:
     * @code
     * axMenu* menu = new axTreeMenu(panel, wxID_ANY, wxPoint(0, 0), wxSize(150, 450));
     * axMenuNode* root = new axTreeMenuNode(menu, wxID_ANY, wxT("ROOT"), nodeData, nodeImg);
     * menu->addRootNode(root);
     *
     * axMenuNode* n1 = new axTreeMenuNode(root, wxID_ANY, wxT("Node 1"), nodeData, nodeImg);
     * root->addSubNode( n1 );
     * s1->addSubNode( new axTreeMenuNode(n1, wxID_ANY, wxT("Sub Node 1"), nodeData, nodeImg));
     * @endcode
     *
     * @param node Node to be added
     ***********************************************************************************/
    void AddSubNode( axMenuNode* node );

    void SelectNode();
    void UnselectAll();

    string GetLabel() const
    {
        return m_label;
    }

    /********************************************************************************//**
     * @brief  Return buttom left position of the last node.
     *         Is used to place new node.
     *
     * @return buttom left position of the last node
     ***********************************************************************************/
    axPoint GetButtomPosition();

    int GetDelta()
    {
        return m_delta;
    }

private:
    axMenuNodeSelectionMode m_select_mode;
    axImage* m_img;
    string m_label;
    int m_nCurrentImg;
    int m_delta;

    // Sub Node Params
    int m_nSubNode, m_nSubNodeMax;
    axMenuNode** m_nodes;

    vector<axMenuNode*> _nodes;

    // Parent
    axMenuNode* m_parentNode;

    enum axMenuNodeImageState
    {
        axMENU_NODE_IMAGE_NORMAL = 0,
        axMENU_NODE_IMAGE_SELECTED
    };

    static const int axMENU_NODE_INIT_ARRAY_SIZE = 8;
    static const int axMENU_NODE_HEIGHT = 25;

    void ResizeNode();
    virtual void OnMouseLeftDown(const axPoint& pos);

    void OnPaint();
};

/********************************************************************************//**
* axMenu.
***********************************************************************************/
class axMenu: public axPanel
{
public:
    axMenu(axApp* app, axWindow* parent, const axRect& rect);
    // axMenu(axApp* app, axWindow* parent, const axRect& rect, string test);

    bool AddRootNode(axMenuNode* node);

private:
    axMenuNode* m_root;
    void OnPaint();
};

#endif // __AX_MENU__
