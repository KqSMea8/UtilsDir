#pragma once

#include <zlo.hpp>

#include "../../../utils/tree/tree.h"

class CClientTree : public CTreeCtrlEx
{
public:
    void LoadItemImages();

public:
    zlo::client* const GetAsClient( HTREEITEM hItem );

    zlo::client::browser::item* const GetAsNode( HTREEITEM hItem );

    zlo::client::browser::item* const GetAsTag( HTREEITEM hItem );

    HTREEITEM AddClient( zlo::client_ptr client );

    void RefreshItem( HTREEITEM hItem );

private:
    HTREEITEM AddNode(
        HTREEITEM                      hParent,
        zlo::client::browser::item_ptr node );

    HTREEITEM AddTag(
        HTREEITEM                      hParent,
        zlo::client::browser::item_ptr tag );

    void BrowseItem( HTREEITEM hItem, BOOL bForce = FALSE );

protected:
    afx_msg void OnSelectionChanged( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnDoubleClick     ( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnExpanding       ( NMHDR* pNMHDR, LRESULT* pResult );

    DECLARE_MESSAGE_MAP()

private:
    int Client;
    int Node;
    int Tag;
};
