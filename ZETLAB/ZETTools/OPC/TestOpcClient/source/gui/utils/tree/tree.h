#pragma once

#include <Uxtheme.h>
#include <CDPI/cdpi.h>

#include <boost/signals.hpp>

#include <list>

#pragma comment(lib, "uxtheme.lib")

class CTreeCtrlEx : public CTreeCtrl
{
private:
    typedef struct _ITEMDATA
    {
        DWORD_PTR data;
        int       type;

    } ITEMDATA, *LPITEMDATA;

    std::list< ITEMDATA > _ITEMDATALIST;

public:
    CTreeCtrlEx( bool b7Style_ = true ) :
        m_7Style     (b7Style_),
        m_contextMenu(nullptr)
    {
    };

    virtual ~CTreeCtrlEx()
    {
    };

public:
    virtual void LoadItemImages()
    {
    };

    BOOL SetItemData( HTREEITEM hItem, int type, DWORD_PTR data )
    {
        ITEMDATA itemData;

        itemData.data = data;
        itemData.type = type;

        _ITEMDATALIST.push_back( itemData );

        return CTreeCtrl::SetItemData( hItem, reinterpret_cast< DWORD_PTR >(&_ITEMDATALIST.back()) );
    };

    DWORD_PTR GetItemData( HTREEITEM hItem, int* type )
    {
        LPITEMDATA itemData = reinterpret_cast< LPITEMDATA >(CTreeCtrl::GetItemData( hItem ));

       *type = itemData->type;
        return itemData->data;
    };

protected:
    PTM_WARNING_DISABLE
    virtual const AFX_MSGMAP* GetMessageMap() const
    {
        return GetThisMessageMap();
    };

    static const AFX_MSGMAP* PASCAL GetThisMessageMap()
    {
        typedef CTreeCtrlEx ThisClass;
        typedef CTreeCtrl TheBaseClass;
        static const AFX_MSGMAP_ENTRY _messageEntries[] = {
            // Message map
            ON_WM_CONTEXTMENU()
            ON_NOTIFY_REFLECT( NM_RCLICK, OnRightClick )

            { 0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 }
        };

        static const AFX_MSGMAP messageMap = {
            &TheBaseClass::GetThisMessageMap, &_messageEntries[0]
        };

        return &messageMap;
    };
    PTM_WARNING_RESTORE

    afx_msg void OnContextMenu( CWnd* pWnd, CPoint ptMousePos ) 
    {
        HTREEITEM htItem;

        // if Shift-F10
        if (ptMousePos.x == -1 && ptMousePos.y == -1)
        {
            htItem = GetSelectedItem();

            CRect rc;

            GetItemRect( htItem, &rc, TRUE );

            ptMousePos = rc.CenterPoint();
        }
        else
        {
            ScreenToClient( &ptMousePos );

            UINT uFlags;

            htItem = HitTest( ptMousePos, &uFlags );
        }
        
        if (NULL == htItem)
            return;

        SelectItem( htItem );

        sig_prepare_cm( htItem, &m_contextMenu );
        if (!m_contextMenu || !m_contextMenu->m_hMenu)
            return;

        ClientToScreen( &ptMousePos );

        m_contextMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptMousePos.x, ptMousePos.y, GetParent() );
    };

    afx_msg void OnRightClick( NMHDR* pNMHDR, LRESULT* pResult )
    {
        SendMessage( WM_CONTEXTMENU, (WPARAM)m_hWnd, GetMessagePos() );

        *pResult = 1;
    }

    void PreSubclassWindow()
    {
        #ifdef UNICODE
        if (m_7Style)
            SetWindowTheme( m_hWnd, L"explorer", 0 );
        #endif

        LoadItemImages();

        CTreeCtrl::PreSubclassWindow();
    };

protected:
    bool       m_7Style;
    CImageList m_imageList;
    CMenu*     m_contextMenu;
    CDPI       m_dpiAware;

public:
    void OnPrepareContextMenu( const boost::function< void(HTREEITEM, CMenu**) >& f )
        { sig_prepare_cm.connect( f ); }

private:
    boost::signal< void(HTREEITEM, CMenu**) > sig_prepare_cm;
};
