#pragma once

#include <Accelerated/Accelerated.h>
#include <Localized/Localized.h>

#include <zlo.hpp>

#include "client_tree/client_tree.h"

#include <list>

// Forward declarations
class CTagOverview;

class CClientTreeView :
    public CAccelerated< CView, IDA_TREE_VIEW >,
    public CLocalized
{
private:
    typedef std::list< zlo::client_ptr > client_list;

protected:
    CClientTreeView();
    DECLARE_DYNCREATE(CClientTreeView)

public:
    virtual ~CClientTreeView();

public:
    virtual void OnDraw( CDC* pDC );
    virtual void OnInitialUpdate();

public:
    void ConnectWithTags( CTagOverview* tags );

    bool AddClient( zlo::client_ptr client );

    afx_msg void    OnDisconnectRequest();

protected:
    afx_msg int     OnCreate( LPCREATESTRUCT lpCreateStruct );
    afx_msg void    OnSize( UINT nType, int cx, int cy );
    afx_msg LRESULT OnDisconnectAvailable( WPARAM wParam, LPARAM lParam );
    afx_msg void    OnRefreshRequest();
    afx_msg LRESULT OnAddTagRequestIndirect( WPARAM wParam, LPARAM lParam );
    afx_msg void    OnAddItemRequest();
    afx_msg void    OnAddItemRecursiveRequest();
    afx_msg void    OnPrepareContextMenu( HTREEITEM hItem, CMenu** contextMenu );

    DECLARE_MESSAGE_MAP()

protected:
    CClientTree   m_tree;
    CTagOverview* m_tags;
    CMenu         m_contextMenu;

private:
    // Holders
    client_list _clients;
};
