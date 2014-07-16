#include "axMenu.h"


/********************************************************************************//**
* axMenuNode.
***********************************************************************************/
axMenuNode::axMenuNode( axApp* app, axWindow* parent,
                        const axRect& rect):
                        axPanel(app, parent, rect),
                        // Members.
                        m_label("ROOT"),
                        m_img(""),
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

axMenuNode::axMenuNode( axMenuNode* parent,
                        const string& label,
                        string img_path,
                        axMenuNodeSelectionMode mode ):
                        axPanel( parent->GetApp(), parent, axRect(0, 0, 20, 20) ),
                        // Members.
                        m_img( img_path ),
                        m_parentNode( parent ),
                        m_label( label ),
                        m_nCurrentImg( axMENU_NODE_IMAGE_NORMAL ),
                        m_nSubNode( 0 ),
                        m_nSubNodeMax( axMENU_NODE_INIT_ARRAY_SIZE )
{
    //m_nodes = new axMenuNode* [ m_nSubNodeMax ];
    m_delta = parent->GetDelta() + 8;
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

    gc->SetColor( axColor("#666666") );
    gc->DrawRectangle( axRect(0, 0, size.x, size.y) );
    // gc->DrawRectangleColorFade( axRect(0, 0, size.x, axMENU_NODE_HEIGHT ),
    //                             axColor("#444444"),
    //                             axColor("#555555") );

    gc->SetColor( axColor("#AAAAAA") );
    gc->SetFontType("FreeSans.ttf");
    gc->SetFontSize(13);

    //cout << "Rect : " << rect.position.x << " " << rect.position.y << " " << rect.size.x << " " << rect.size.y << endl;
    gc->DrawStringAlignedCenter(m_label, rect);

    if( m_img.IsImageReady() )
    {
        gc->DrawPartOfImage( &m_img,
                             axPoint( 0, m_nCurrentImg * 12 ),
                             axSize( 12, 12 ), axPoint(m_delta + 7, 4));//axPoint( 7 + m_delta - 30, 4 ) );
    }

    // Contour.
    gc->SetColor( axColor("#000000") );
    gc->DrawRectangleContour( axRect( 0, 0, size.x, size.y ) );
}

/********************************************************************************//**
* axMenu.
***********************************************************************************/
axMenu::axMenu(axApp* app, axWindow* parent, const axRect& rect):
               axPanel(app, parent, rect)
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
    gc->DrawRectangleContour(rect0);
}

//axToggle::axToggle( axSTANDARD_WIDGET_PARAMS( axToggle ),
//                    string img_path,
//                    string label,
//                    axFlag flags ):
//                    // Members
//                    axSTANDARD_WIDGET_PARAMS_CONSTRUCT,
//                    m_currentColor( &m_info.normal_color ),
//                    m_btnImg( img_path.c_str() ),
//                    m_nCurrentImg( axTOGGLE_NORMAL ),
//                    m_label( label ),
//                    m_flags( flags ),
//                    m_selected( false )
//{

//#if( axUSING_IMLIB2)
//    SetCustomPaint( true );
//#endif
//}

//axToggle::axToggle( axApp* app,
//                    axWindow* parent,
//                    const axID& id,
//                    const axRect& rect,
//                    void (*fct_ptr)(void*),
//                    const axToggleInfo& info,
//                    string img_path,
//                    string label,
//                    axFlag flags ):
//                    // Members
//                    axWINDOW( app, parent, axID_ANY, rect ),
//                    m_info( info ),
//                    m_currentColor( &m_info.normal_color ),
//                    m_btnImg( img_path.c_str() ),
//                    m_nCurrentImg( axTOGGLE_NORMAL ),
//                    m_label( label ),
//                    m_flags( flags ),
//                    m_selected( false )
//{
//#if( axUSING_IMLIB2)
//    SetCustomPaint( true );
//#endif

//    m_eventsID.leftDown = axID_ANY;
//    parent->Connect( m_eventsID.leftDown, fct_ptr );

//}

//bool axToggle::GetValue() const
//{
//    return m_selected;
//}

//void axToggle::SetValue( const bool& v )
//{
//    if( m_selected != v )
//    {
//        m_selected = v;

//        if( m_selected )
//        {
//            m_currentColor = &m_info.selected_color;
//            m_nCurrentImg = axTOGGLE_SELECTED;
//        }
//        else
//        {
//            m_currentColor = &m_info.normal_color;
//            m_nCurrentImg = axTOGGLE_NORMAL;
//        }
//    }
//}

//void axToggle::OnMouseMotion()
//{
//    GetParent()->TriggerEvent( m_eventsID.motion );
//}

//void axToggle::OnMouseLeftDown()
//{
//    if( m_selected )
//    {
//        m_currentColor = &m_info.selected_click;
//        m_nCurrentImg = axTOGGLE_SELECTED_DOWN;
//    }
//    else
//    {
//        m_currentColor = &m_info.clicking_color;
//        m_nCurrentImg = axTOGGLE_DOWN;
//    }

//    m_selected = (!m_selected);

//    GrabMouse();

//    GetParent()->TriggerEvent( m_eventsID.leftDown );

//    Update();
//}

//void axToggle::OnMouseLeftUp()
//{
//    if( IsGrabbed() )
//    {
//        UnGrabMouse();

//        if( IsMouseInWindow() )
//        {
//            if( m_selected )
//            {
//                m_currentColor = &m_info.selected_hover;
//                m_nCurrentImg = axTOGGLE_SELECTED_HOVER;
//            }
//            else
//            {
//                m_currentColor = &m_info.hover_color;
//                m_nCurrentImg = axTOGGLE_HOVER;
//            }
//        }
//        else
//        {
//            if( m_selected )
//            {
//                m_currentColor = &m_info.selected_color;
//                m_nCurrentImg = axTOGGLE_SELECTED;
//            }
//            else
//            {
//                m_currentColor = &m_info.normal_color;
//                m_nCurrentImg = axTOGGLE_NORMAL;
//            }
//        }

//        GetParent()->TriggerEvent( m_eventsID.leftUp );

//        Update();
//    }
//}

//void axToggle::OnMouseEnter()
//{
//    if( !IsGrabbed() )
//    {
//        if( m_selected )
//        {
//            m_currentColor = &m_info.selected_hover;
//            m_nCurrentImg = axTOGGLE_SELECTED_HOVER;
//        }
//        else
//        {
//            m_currentColor = &m_info.hover_color;
//            m_nCurrentImg = axTOGGLE_HOVER;
//        }
//        Update();
//    }

//    GetParent()->TriggerEvent( m_eventsID.enter );
//}

//void axToggle::OnMouseLeave()
//{
//    if( !IsGrabbed() )
//    {
//        if( m_selected )
//        {
//            m_currentColor = &m_info.selected_color;
//            m_nCurrentImg = axTOGGLE_SELECTED;
//        }
//        else
//        {
//            m_currentColor = &m_info.normal_color;
//            m_nCurrentImg = axTOGGLE_NORMAL;
//        }
//    }

//    GetParent()->TriggerEvent( m_eventsID.leave );

//    Update();
//}

//void axToggle::OnPaint()
//{
//#if( axUSING_IMLIB2 )
//    axGC gc( GetBackBuffer() );
//    axSize size = GetSize();
//    axRect rect( 0, 0, size.x - 1, size.y - 1 );

//    gc.SetColor( *m_currentColor );
//    gc.DrawRectangle( rect );

//    if( m_btnImg.IsImageReady() )
//    {
//        if( axFlag_exist( axTOGGLE_FLAG_RESIZE_IMAGE, m_flags ) )
//        {
//            axSize img_size = m_btnImg.GetSize();
//            img_size.y /= 3;
//            gc.DrawPartOfImageWithResize( &m_btnImg,
//                                          axPoint( 0, m_nCurrentImg * ( img_size.y )  ),
//                                          img_size,
//                                          axPoint( 0, 0 ), GetSize() );
//        }
//        else
//        {
//            gc.DrawPartOfImage( &m_btnImg,
//                                axPoint( 0, m_nCurrentImg * size.y ),
//                                size,
//                                axPoint( 0, 0 ) );
//        }
//    }

//    if( m_label != "" )
//    {
//        //axDEBUG_CONSOLE->Write("toggle draw label");
//        gc.DrawTextAligned( m_label, axTEXT_CENTER,
//                            m_currentColor->GetColorRGB(),
//                            "8", axRect( 1, 1, size.x -2, size.y -2 ) );
//    }

//    gc.SetColor( m_info.contour_color );
//    gc.SetLineWidth( 1 );
//    gc.DrawRectangleContour( rect );

//    FlipScreen( gc );
//#endif // axUSING_IMLIB2.

//#if( axUSING_GLX )
//    axGC* gc = GetPaintGC();
//    axSize size = GetSize();
//    axRect rect( 0, 0, size.x, size.y );

//    gc->SetColor( *m_currentColor );
//    gc->DrawRectangle( rect );

//    if( m_btnImg.IsImageReady() )
//    {
////        if( axFlag_exist( axTOGGLE_FLAG_RESIZE_IMAGE, m_flags ) )
////        {
////            axSize img_size = m_btnImg.GetSize();
////            img_size.y /= 3;
////            gc.DrawPartOfImageWithResize( &m_btnImg,
////                                          axPoint( 0, m_nCurrentImg * ( img_size.y )  ),
////                                          img_size,
////                                          axPoint( 0, 0 ), GetSize() );
////        }
////        else
////        {
//            gc->DrawPartOfImage( &m_btnImg,
//                                 axPoint( 0, m_nCurrentImg * size.y ),
//                                 size,
//                                 axPoint( 0, 0 ) );
////        }
//    }

//    if( m_label != "" )
//    {
//        //axDEBUG_CONSOLE->Write("toggle draw label");
//        gc->DrawTextAligned( m_label, axTEXT_CENTER,
//                             m_currentColor->GetColorRGB(),
//                             "8", axRect( 1, 1, size.x -2, size.y -2 ) );
//    }

//    //gc->SetColor( m_info.contour_color );
//    //gc->SetLineWidth( 1 );
//    //gc->DrawRectangleContour( rect );

//    //FlipScreen( gc );
//#endif // axUSING_GLX.
//}
