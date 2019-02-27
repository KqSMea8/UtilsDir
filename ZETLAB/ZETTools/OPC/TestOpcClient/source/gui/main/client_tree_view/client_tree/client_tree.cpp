#include "stdafx.h"
#include "../../../../application/application.h"
#include "client_tree.h"

BEGIN_MESSAGE_MAP( CClientTree, CTreeCtrlEx )
    ON_NOTIFY_REFLECT( TVN_SELCHANGED,    OnSelectionChanged )
    ON_NOTIFY_REFLECT( NM_DBLCLK,         OnDoubleClick )
    ON_NOTIFY_REFLECT( TVN_ITEMEXPANDING, OnExpanding )
END_MESSAGE_MAP()

void CClientTree::LoadItemImages()
{
    SetItemHeight( m_dpiAware.ScaleY( 22 ) );

    int width  = m_dpiAware.ScaleX( 16 );
    int height = m_dpiAware.ScaleY( 16 );

    m_imageList.Detach();
    m_imageList.Create( width, height, ILC_COLOR32, 1, 1 );

    // Load icons
    Client = m_imageList.Add( LOAD_ICON(IDI_OPC_SERVER, width, height ));
    Node   = m_imageList.Add( LOAD_ICON(IDI_OPC_NODE,   width, height ));
    Tag    = m_imageList.Add( LOAD_ICON(IDI_OPC_TAG,    width, height ));

    SetImageList( &m_imageList, TVSIL_NORMAL );
}

zlo::client* const CClientTree::GetAsClient( HTREEITEM hItem )
{
    ASSERT( hItem );

    int       type;
    DWORD_PTR itemData = GetItemData( hItem, &type );

    // Cause' client item is a root node
    if (Node == type)
    {
        // Is a root item
        if (!GetParentItem( hItem ))
        {
            return reinterpret_cast< zlo::client::browser::item* >(itemData)->getClient();
        }
    }

    return nullptr;
}

zlo::client::browser::item* const CClientTree::GetAsNode( HTREEITEM hItem )
{
    ASSERT( hItem );

    int       type;
    DWORD_PTR itemData = GetItemData( hItem, &type );

    if (Node == type)
        return reinterpret_cast< zlo::client::browser::item* >(itemData);

    return nullptr;
}

zlo::client::browser::item* const CClientTree::GetAsTag( HTREEITEM hItem )
{
    ASSERT( hItem );

    int       type;
    DWORD_PTR itemData = GetItemData( hItem, &type );

    if (Tag == type)
        return reinterpret_cast< zlo::client::browser::item* >(itemData);

    return nullptr;
}

HTREEITEM CClientTree::AddClient( zlo::client_ptr client )
{
    TVINSERTSTRUCT tvis;

    tvis.hParent             = TVI_ROOT;
    tvis.hInsertAfter        = TVI_LAST;
    tvis.item.mask           = TVIF_CHILDREN | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
    tvis.item.pszText        = const_cast< LPTSTR >(client->getName().c_str());
    tvis.item.cchTextMax     = _tcslen( tvis.item.pszText ) + 1;
    tvis.item.iImage         = Client;
    tvis.item.iSelectedImage = Client;
    tvis.item.cChildren      = 1;

    HTREEITEM ret = InsertItem( &tvis );

    if (ret)
    {
        // Set item data type as a Node
        SetItemData( ret, Node, reinterpret_cast< DWORD_PTR >(client->getBrowser()->getRoot()) );
    }

    return ret;
}

HTREEITEM CClientTree::AddNode(
    HTREEITEM                      hParent,
    zlo::client::browser::item_ptr node )
{
    TVINSERTSTRUCT tvis;

    tvis.hParent             = hParent;
    tvis.hInsertAfter        = TVI_LAST;
    tvis.item.mask           = TVIF_CHILDREN | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
    tvis.item.pszText        = const_cast< LPTSTR >(node->getName().c_str());
    tvis.item.cchTextMax     = _tcslen( tvis.item.pszText ) + 1;
    tvis.item.iImage         = Node;
    tvis.item.iSelectedImage = Node;
    tvis.item.cChildren      = 1;

    HTREEITEM ret = InsertItem( &tvis );

    if (ret)
    {
        SetItemData( ret, Node, reinterpret_cast< DWORD_PTR >(node.get()) );
    }

    return ret;
}

HTREEITEM CClientTree::AddTag(
    HTREEITEM                      hParent,
    zlo::client::browser::item_ptr tag )
{
    HTREEITEM ret = InsertItem( tag->getName().c_str(), Tag, Tag, hParent );

    if (ret)
    {
        SetItemData( ret, Tag, reinterpret_cast< DWORD_PTR >(tag.get()) );
    }

    return ret;
}

void CClientTree::BrowseItem( HTREEITEM hItem, BOOL bForce )
{
    int       type;
    DWORD_PTR itemData = GetItemData( hItem, &type );

    if (Node == type)
    {
        TVITEM tvi;

        tvi.mask      = TVIF_HANDLE | TVIF_CHILDREN;
        tvi.hItem     = hItem;
        tvi.cChildren = 0;
        
        zlo::client::browser::item* node =
            reinterpret_cast< zlo::client::browser::item* >(itemData);

        if (bForce)
            node->clearCache();

        if (node->isBrowsed())
            return;

        // Add nodes & tags
        auto nodes = node->browse( zlo::client::browser::filter_node );
        auto tags  = node->browse( zlo::client::browser::filter_tag );

        for (auto it = nodes.begin(), end = nodes.end(); it != end; ++it)
        {
            AddNode( hItem, (*it) );

            tvi.cChildren++;
        }

        for (auto it = tags.begin(), end = tags.end(); it != end; ++it)
        {
            AddTag ( hItem, (*it) );

            tvi.cChildren++;
        }

        SetItem( &tvi );
    }
}

void CClientTree::RefreshItem( HTREEITEM hItem )
{
    ASSERT( hItem );

    // Delete all child items
    HTREEITEM hChild = GetChildItem( hItem );

    while (hChild)
    {
        HTREEITEM hToDelete = hChild;

        hChild = GetNextSiblingItem( hChild );

        DeleteItem( hToDelete );
    }

    // Re-add child items
    BrowseItem( hItem, TRUE /* forced */ );
}

void CClientTree::OnSelectionChanged( NMHDR* pNMHDR, LRESULT* pResult )
{
    *pResult = CDRF_DODEFAULT;

    HTREEITEM hItem = GetSelectedItem();

    if (!hItem)
        return;

    GetParent()->SendMessage(
        ID_DISCONNECT_AVAILABLE,
        reinterpret_cast< WPARAM >(GetAsClient( hItem ))
    );
}

void CClientTree::OnDoubleClick( NMHDR* pNMHDR, LRESULT* pResult )
{
    *pResult = CDRF_DODEFAULT;

    HTREEITEM hItem = GetSelectedItem();

    if (!hItem)
        return;

    zlo::client::browser::item* item = GetAsTag( hItem );

    if (item)
    {
        GetParent()->SendMessage( ID_ADD_ITEM, reinterpret_cast< WPARAM >(item) );
    }
}

void CClientTree::OnExpanding( NMHDR* pNMHDR, LRESULT* pResult )
{
    LPNMTREEVIEW pNMTV = reinterpret_cast< LPNMTREEVIEW >(pNMHDR);

    *pResult = CDRF_DODEFAULT;

    if (TVE_EXPAND == pNMTV->action && pNMTV->itemNew.hItem)
    {
        BrowseItem( pNMTV->itemNew.hItem );
    }
}
