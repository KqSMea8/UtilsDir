#include "stdafx.h"
#include "../../../application/application.h"
#include "client_tree_view.h"

#include "../tag_overview/tag_overview.h"

#include <boost/bind.hpp>

IMPLEMENT_DYNCREATE( CClientTreeView, CView )

BEGIN_MESSAGE_MAP( CClientTreeView, CView )
    ON_WM_CREATE()
    ON_WM_SIZE  ()
    ON_MESSAGE  ( ID_DISCONNECT_AVAILABLE, OnDisconnectAvailable )
    ON_COMMAND  ( ID_DISCONNECT,           OnDisconnectRequest )
    ON_COMMAND  ( ID_REFRESH,              OnRefreshRequest )
    ON_MESSAGE  ( ID_ADD_ITEM,             OnAddTagRequestIndirect )
    ON_COMMAND  ( ID_ADD_ITEM,             OnAddItemRequest )
    ON_COMMAND  ( ID_ADD_ITEM_RECURSIVE,   OnAddItemRecursiveRequest )
END_MESSAGE_MAP()

CClientTreeView::CClientTreeView() : m_tags(nullptr)
{
}

CClientTreeView::~CClientTreeView()
{
}

void CClientTreeView::OnDraw( CDC* pDC )
{
}

void CClientTreeView::OnInitialUpdate() 
{
    CView::OnInitialUpdate();
}

int CClientTreeView::OnCreate( LPCREATESTRUCT lpCreateStruct ) 
{
    if (CView::OnCreate( lpCreateStruct ) == -1)
        return -1;

    BOOL ret = m_tree.Create(
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_HSCROLL |
        TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS,
        CRect(),
        this,
        IDC_TREE
    ); 

    m_tree.LoadItemImages(); // WHY???????

    m_tree.OnPrepareContextMenu(
        boost::bind( &CClientTreeView::OnPrepareContextMenu, this, _1, _2 )
    );

    return (ret ? 0 : -1);
}


void CClientTreeView::OnSize( UINT nType, int cx, int cy ) 
{
    CView::OnSize( nType, cx, cy );

    if (IsWindow( m_tree.m_hWnd ))
        m_tree.SetWindowPos( &wndTop, 0, 0, cx, cy, SWP_NOACTIVATE );
}

void CClientTreeView::ConnectWithTags( CTagOverview* tags )
{
    m_tags = tags;
}

bool CClientTreeView::AddClient( zlo::client_ptr client )
{
    ASSERT( m_tags );

    if (m_tree.AddClient( client ))
    {
        // Subscribe to tag changes
        client->getGroup()->onItemChange( boost::bind( &CTagOverview::OnTagChange, m_tags, _1 ) );

        _clients.push_back( client );

        m_tree.SetFocus();

        return true;
    }

    return false;
}

LRESULT CClientTreeView::OnDisconnectAvailable( WPARAM wParam, LPARAM lParam )
{
    return GetParent()->
               GetParent()->
                   GetParent()->SendMessage( ID_DISCONNECT_AVAILABLE, wParam, lParam );
}

void CClientTreeView::OnDisconnectRequest()
{
    if (!m_tree.GetSelectedItem())
        return;

    ASSERT( m_tags );

    void*     ptr;
    HTREEITEM hItem = m_tree.GetSelectedItem();

    if ((ptr = m_tree.GetAsClient( hItem )))
    {
        zlo::client* client = reinterpret_cast< zlo::client* >(ptr);

        // Remove all client tags
        m_tags->OnDisconnect( client );

        // Remove this client
        m_tree.DeleteItem( hItem );

        for (auto it = _clients.begin(), end = _clients.end(); it != end; ++it)
        {
            if (it->get() == client)
            {
                _clients.erase( it );
                break;
            }
        }
    }
}

void CClientTreeView::OnRefreshRequest()
{
    if (!m_tree.GetSelectedItem())
        return;

    m_tree.RefreshItem( m_tree.GetSelectedItem() );
}

LRESULT CClientTreeView::OnAddTagRequestIndirect( WPARAM wParam, LPARAM lParam )
{
    ASSERT( m_tags );

    // Retranslate directly to tags view
    return m_tags->SendMessage( ID_ADD_ITEM, wParam, lParam );
}

void CClientTreeView::OnAddItemRequest()
{
    if (!m_tree.GetSelectedItem())
        return;

    ASSERT( m_tags );

    void*     ptr;
    HTREEITEM hItem = m_tree.GetSelectedItem();

    if ((ptr = m_tree.GetAsTag( hItem )) || (ptr = m_tree.GetAsNode( hItem )))
    {
        // Retranslate to tags view
        m_tags->SendMessage( ID_ADD_ITEM, reinterpret_cast< WPARAM >(ptr), FALSE );
    }
}

void CClientTreeView::OnAddItemRecursiveRequest()
{
    if (!m_tree.GetSelectedItem())
        return;

    ASSERT( m_tags );

    void*     ptr;
    HTREEITEM hItem = m_tree.GetSelectedItem();

    if ((ptr = m_tree.GetAsNode( hItem )))
    {
        // Retranslate to tags view
        m_tags->SendMessage( ID_ADD_ITEM, reinterpret_cast< WPARAM >(ptr), TRUE );
    }
}

void CClientTreeView::OnPrepareContextMenu( HTREEITEM hItem, CMenu** contextMenu )
{
    ASSERT( hItem );

    // Invalidate old menu
    m_contextMenu.DestroyMenu();

    void* ptr;

    if ((ptr = m_tree.GetAsClient( hItem )))
    {
        zlo::client* client =
            reinterpret_cast< zlo::client* >(ptr);

        m_contextMenu.LoadMenu( IDM_TREE_CLIENT );
    }

    else if ((ptr = m_tree.GetAsNode( hItem )))
    {
        zlo::client::browser::item* node =
            reinterpret_cast< zlo::client::browser::item* >(ptr);

        m_contextMenu.LoadMenu( IDM_TREE_NODE );
    }

    else if ((ptr = m_tree.GetAsTag( hItem )))
    {
        zlo::client::browser::item* tag =
            reinterpret_cast< zlo::client::browser::item* >(ptr);

        m_contextMenu.LoadMenu( IDM_TREE_TAG );
    }

    LOCALIZED_MENU_ITEM( (&m_contextMenu), ID_DISCONNECT );
    LOCALIZED_MENU_ITEM( (&m_contextMenu), ID_REFRESH );
    LOCALIZED_MENU_ITEM( (&m_contextMenu), ID_ADD_ITEM );
    LOCALIZED_MENU_ITEM( (&m_contextMenu), ID_ADD_ITEM_RECURSIVE );

    *contextMenu = m_contextMenu.GetSubMenu( 0 );
}
