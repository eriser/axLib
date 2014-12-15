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
#include "axMenu.h"


/********************************************************************************//**
* axMenuNode.
***********************************************************************************/
axMenuNode::axMenuNode( axWindow* parent,
                        const axRect& rect):
                        axPanel(parent, rect),
                        // Members.
                        m_label("ROOT"),
                        m_img(nullptr),
                        m_nCurrentImg( axMENU_NODE_IMAGE_SELECTED ),
                        m_select_mode( axMENU_SELECT_ANY )
{
    m_parentNode = NULL;

    m_nSubNode = 0;
    m_nSubNodeMax = axMENU_NODE_INIT_ARRAY_SIZE;
    //m_nodes = new axMenuNode* [ m_nSubNodeMax ];

    m_delta = -8;

    // Set default node size.
    SetSize(axSize(rect.size.x, axMENU_NODE_HEIGHT));
}

axMenuNode::axMenuNode(axMenuNode* parent,
                       const string& label,
                       string img_path,
                       axMenuNodeSelectionMode mode ):
// Heritage.
axPanel(parent, axRect(0, 0, 20, 20)),
// Members.
m_parentNode(parent),
m_label(label),
m_nCurrentImg(axMENU_NODE_IMAGE_NORMAL),
m_nSubNode(0),
m_nSubNodeMax(axMENU_NODE_INIT_ARRAY_SIZE)
{
    //m_nodes = new axMenuNode* [ m_nSubNodeMax ];
    m_delta = parent->GetDelta() + 8;

	m_img = new axImage(img_path);
}

void axMenuNode::AddSubNode(axMenuNode* node)
{
    if( node )
    {
        // Resize node to parent size.
        node->SetSize(axSize(GetParent()->GetSize().x, axMENU_NODE_HEIGHT));

        // If is not the fisrt one to be added.
        if(_nodes.size())
        {
            node->SetPosition(_nodes[_nodes.size()-1]->GetButtomPosition());
        }
        // First added.
        else 
        {
            node->SetPosition(axPoint(0, axMENU_NODE_HEIGHT));
        }

        // Add Node.
        _nodes.push_back(node);

        ResizeNode();
    }
}

void axMenuNode::ResizeNode()
{
    int y = axMENU_NODE_HEIGHT;

    if(_nodes.size() && m_nCurrentImg == axMENU_NODE_IMAGE_SELECTED)
    {
        for(auto n : _nodes)
        {
            y += n->GetSize().y;
        }

        y -= (_nodes.size() + 1);

        for(int i = 1; i < _nodes.size(); ++i)
        {
            _nodes[i]->SetPosition(_nodes[i - 1]->GetButtomPosition() - axPoint(0, 1));
        }

        for(auto n : _nodes)
        {
            cout << n->GetLabel() << endl;
            n->Show();
        }
    }

    else
    {
        for(auto n : _nodes)
        {
            n->Hide();
        }
    }

    SetSize(axSize(GetSize().x, y));

    if(m_parentNode) 
    {
        m_parentNode->ResizeNode();
    }
}

void axMenuNode::SelectNode()
{
    if( m_nCurrentImg == axMENU_NODE_IMAGE_NORMAL )
    {
        m_nCurrentImg = axMENU_NODE_IMAGE_SELECTED;

        ResizeNode();
        Update();
    }
}

axPoint axMenuNode::GetButtomPosition()
{
    return axPoint(GetRect().position.x, GetRect().position.y + GetRect().size.y + 1 );
}

void axMenuNode::UnselectAll()
{
}

void axMenuNode::OnMouseLeftDown(const axPoint& pos)
{
    // If node is already selected.
    if( m_nCurrentImg == axMENU_NODE_IMAGE_SELECTED )
    {
        // Set node UI info to unselected.
        m_nCurrentImg = axMENU_NODE_IMAGE_NORMAL;
    }

    else // Node if unselected.
    {
        m_nCurrentImg = axMENU_NODE_IMAGE_SELECTED;
    }

    ResizeNode();
    Update();
}

void axMenuNode::OnPaint()
{
    axGC* gc = GetGC();
    axSize size = GetSize();
    axRect rect( m_delta, 0, size.x, axMENU_NODE_HEIGHT );
	axRect rect0(0.0, 0.0, rect.size.x, rect.size.y);

    gc->SetColor(axColor(0.6, 0.6, 0.6));
    gc->DrawRectangle(rect0);

    gc->SetColor( axColor("#AAAAAA") );
    gc->SetFontSize(13);

    //cout << "Rect : " << rect.position.x << " " << rect.position.y << " " << rect.size.x << " " << rect.size.y << endl;
    gc->DrawStringAlignedCenter(m_label, rect);

    if( m_img != nullptr && m_img->IsImageReady() )
    {
        gc->DrawPartOfImage(m_img,
                            axPoint( 0, m_nCurrentImg * 12 ),
                            axSize( 12, 12 ), axPoint(m_delta + 7, 4));
    }

    // Contour.
    gc->SetColor(axColor(0.0, 0.0, 0.0));
    gc->DrawRectangleContour(axRect(0, 0, size.x, size.y));
}

/********************************************************************************//**
* axMenu.
***********************************************************************************/
axMenu::axMenu(axWindow* parent, const axRect& rect):
               axPanel(parent, rect)
{
    m_root = NULL;
}

bool axMenu::AddRootNode(axMenuNode* node)
{
    // If root node doesn't already exist.
    if( !m_root )
    {
        // If scroll bar exist
        //if(scrollBar != NULL)
        //{
            // Resize new node added with scrollbar.
        //    node->SetSize(node->GetSize() + wxSize(GetSize().x - INIT_WIDTH - scrollBar->GetSize().x, 0));
        //}
        //else
        //{
            // Resize new node added.
            //node->SetSize(node->GetSize() + wxSize(GetSize().x - INIT_WIDTH, 0));
        //}

        node->SetSize(axSize(GetSize().x, 15));
        m_root = node;

        return 1;
    }

    else return 0;
}

void axMenu::OnPaint()
{
    axGC* gc = GetGC();
    axSize size = GetSize();
    axRect rect(GetRect());
    axRect rect0(axPoint(0, 0), rect.size);

    gc->SetColor( axColor("#444444") );
    gc->DrawRectangle(rect0);

    // Contour.
    gc->SetColor(axColor("#000000"));
    gc->DrawRectangleContour(axRect(1, 1, rect0.size.x - 1, rect0.size.y - 1));
}

