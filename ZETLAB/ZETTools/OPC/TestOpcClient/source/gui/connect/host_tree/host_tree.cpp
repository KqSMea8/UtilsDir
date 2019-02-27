#include "stdafx.h"
#include "../../../application/application.h"
#include "host_tree.h"

BEGIN_MESSAGE_MAP( CHostTree, CTreeCtrlEx )
    ON_NOTIFY_REFLECT( TVN_SELCHANGED,    OnSelectionChange )
    ON_NOTIFY_REFLECT( NM_DBLCLK,         OnDoubleClick )
    ON_NOTIFY_REFLECT( TVN_ITEMEXPANDING, OnExpanding )
END_MESSAGE_MAP()

void CHostTree::LoadItemImages()
{
    SetItemHeight( m_dpiAware.ScaleY( 22 ) );

    int width  = m_dpiAware.ScaleX( 16 );
    int height = m_dpiAware.ScaleY( 16 );

    m_imageList.Detach();
    m_imageList.Create( width, height, ILC_COLOR32, 1, 1 );

    // Load icons
    Network   = m_imageList.Add( LOAD_ICON(IDI_NETWORK,    width, height ));
    Host      = m_imageList.Add( LOAD_ICON(IDI_HOST,       width, height ));
    OpcServer = m_imageList.Add( LOAD_ICON(IDI_OPC_SERVER, width, height ));

    SetImageList( &m_imageList, TVSIL_NORMAL );
}

HTREEITEM CHostTree::AddNetwork( HTREEITEM hNetwork, utils::network_ptr network )
{
    TVINSERTSTRUCT tvis;

    tvis.hParent             = hNetwork ? hNetwork : TVI_ROOT;
    tvis.hInsertAfter        = TVI_LAST;
    tvis.item.mask           = TVIF_CHILDREN | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
    tvis.item.pszText        = const_cast< LPTSTR >(network->getName());
    tvis.item.cchTextMax     = _tcslen( tvis.item.pszText ) + 1;
    tvis.item.iImage         = Network;
    tvis.item.iSelectedImage = Network;
    tvis.item.cChildren      = 1;

    HTREEITEM ret = InsertItem( &tvis );

    if (ret)
    {
        SetItemData( ret, Network, reinterpret_cast< DWORD_PTR >(network.get()) );

        _networks.push_back( network );
    }

    return ret;
}

HTREEITEM CHostTree::AddHost( HTREEITEM hNetwork, zlo::host_ptr host )
{
    HTREEITEM ret = InsertItem( host->getName().c_str(), Host, Host, hNetwork ? hNetwork : TVI_ROOT );

    if (ret)
    {
        SetItemData( ret, Host, reinterpret_cast< DWORD_PTR >(host.get()) );

        _hosts.push_back( host );

        try
        {
            // Add host servers
            zlo::server_list servers = host->browse();

            for (auto it = servers.begin(), end = servers.end(); it != end; ++it)
            {
                AddOpcServer( ret, *it );
            }
        }
        catch (zlo::host::host_unreachable&)
        {
        }
        catch (zlo::host::opc_enum_unavailable&)
        {
        }
    }

    return ret;
}

HTREEITEM CHostTree::AddOpcServer( HTREEITEM hHost, zlo::server_ptr server )
{
    assert (NULL != hHost);

    HTREEITEM ret = InsertItem( server->getName().c_str(), OpcServer, OpcServer, hHost );

    if (ret)
    {
        SetItemData( ret, OpcServer, reinterpret_cast< DWORD_PTR >(server.get()) );

        _servers.push_back( server );
    }

    return ret;
}

void CHostTree::OnSelectionChange( NMHDR* pNMHDR, LRESULT* pResult )
{
    LPNMTREEVIEW pNMTV = reinterpret_cast< LPNMTREEVIEW >(pNMHDR);

    *pResult = CDRF_DODEFAULT;

    if (pNMTV->itemNew.hItem)
    {
        int       type;
        DWORD_PTR itemData = GetItemData( pNMTV->itemNew.hItem, &type );

        if (OpcServer == type)
        {
            zlo::server* server = reinterpret_cast< zlo::server* >(itemData);

            _onChange( server->getHost().get(), server );
        }
        else if (Host == type)
        {
            zlo::host* host = reinterpret_cast< zlo::host* >(itemData);

            _onChange( host, nullptr );
        }
        else
        {
            _onChange( nullptr, nullptr );
        }
    }
}

void CHostTree::OnDoubleClick( NMHDR* pNMHDR, LRESULT* pResult )
{
    *pResult = CDRF_DODEFAULT;

    HTREEITEM hItem = GetSelectedItem();

    if (!hItem)
        return;

    int       type;
    DWORD_PTR itemData = GetItemData( hItem, &type );

    if (OpcServer == type)
    {
        zlo::server* server = reinterpret_cast< zlo::server* >(itemData);

        _onConnect( server );
    }
}

void CHostTree::OnExpanding( NMHDR* pNMHDR, LRESULT* pResult )
{
    LPNMTREEVIEW pNMTV = reinterpret_cast< LPNMTREEVIEW >(pNMHDR);

    *pResult = CDRF_DODEFAULT;

    if (TVE_EXPAND == pNMTV->action && pNMTV->itemNew.hItem)
    {
        int       type;
        DWORD_PTR itemData = GetItemData( pNMTV->itemNew.hItem, &type );

        if (Network == type)
        {
            utils::network* network = reinterpret_cast< utils::network* >(itemData);

            if (network->isOpen())
                return;

            bool removePlus = false;

            if (network->open())
            {
                // Add network items
                utils::network_list items = network->browse();

                if (items.empty())
                    removePlus = true;

                for (auto it = items.begin(), end = items.end(); it != end; ++it)
                {
                    switch ((*it)->getType())
                    {
                    case utils::network::type_network:
                        AddNetwork( pNMTV->itemNew.hItem, *it );
                        break;
                    }
                }
            }
            else removePlus = true;

            if (removePlus)
            {
                pNMTV->itemNew.cChildren = 0;
                SetItem( &(pNMTV->itemNew) );
            }
        }
    }
}
