#pragma once

#include <Uxtheme.h>
#include <CDPI/cdpi.h>

#include <boost/signals.hpp>

#include <list>

#pragma comment(lib, "uxtheme.lib")

class CListCtrlEx : public CListCtrl
{
private:
    typedef struct _ITEMDATA
    {
        DWORD_PTR data;
        int       type;

    } ITEMDATA, *LPITEMDATA;

    std::list< ITEMDATA > _ITEMDATALIST;

public:
    CListCtrlEx( bool b7Style_ = true ) :
        m_7Style     (b7Style_),
        m_contextMenu(nullptr)
    {
    };

    virtual ~CListCtrlEx()
    {
    };

public:
    virtual void LoadItemImages()
    {
    };

    BOOL SetItemData( int nItem, int type, DWORD_PTR data )
    {
        ITEMDATA itemData;

        itemData.data = data;
        itemData.type = type;

        _ITEMDATALIST.push_back( itemData );

        return CListCtrl::SetItemData( nItem, reinterpret_cast< DWORD_PTR >(&_ITEMDATALIST.back()) );
    };

    DWORD_PTR GetItemData( int nItem, int* type )
    {
        LPITEMDATA itemData = reinterpret_cast< LPITEMDATA >(CListCtrl::GetItemData( nItem ));

       *type = itemData->type;
        return itemData->data;
    };

    int InsertColumn( LPCTSTR lpszText, int nFormat = LVCFMT_LEFT )
    {
        int width = GetStringWidth( lpszText ) + m_dpiAware.ScaleX( 16 );

        LVCOLUMN lvc;

        lvc.mask    = HDI_TEXT | HDI_WIDTH | HDI_FORMAT;
        lvc.pszText = const_cast< LPTSTR >( lpszText );
        lvc.cx      = width;
        lvc.fmt     = nFormat | HDF_STRING;

        return CListCtrl::InsertColumn( 2147483647, &lvc );
    };

    int InsertColumn( UINT nIDText, int nFormat = LVCFMT_LEFT )
    {
        CString tmp;

        if (tmp.LoadString( nIDText ))
            return InsertColumn( tmp, nFormat );

        return -1;
    };

    int InsertItem( int iImage, LPCTSTR lpszItem, ... )
    {
        int nItem(CListCtrl::InsertItem( 2147483647, lpszItem, iImage ));

        va_list v_arg;
        LPCTSTR lpszText_;
        int     nSubItem(1);

        va_start( v_arg, lpszItem );

        SetItemText( nItem, 0, lpszItem );
        for (;;nSubItem++)
        {
            lpszText_ = va_arg( v_arg, LPCTSTR );
            if (!lpszText_) break;

            SetItemText( nItem, nSubItem, lpszText_ );
        }

        va_end( v_arg );

        return nItem;
    };

    void SetColumnSortMarker( int nCol, int marker )
    {
        LVCOLUMN lvc;

        lvc.mask = LVCF_FMT;

        GetColumn( nCol, &lvc );

        if (HDF_SORTDOWN == marker)
            lvc.fmt = (lvc.fmt & ~HDF_SORTUP)   | marker;
        else if (HDF_SORTUP == marker)
            lvc.fmt = (lvc.fmt & ~HDF_SORTDOWN) | marker;
        else
            lvc.fmt &= marker;

        SetColumn( nCol, &lvc );
    };

    int GetColumnSortMarker( int nCol )
    {
        LVCOLUMN lvc;

        lvc.mask = LVCF_FMT;

        GetColumn( nCol, &lvc );

        if (HDF_SORTDOWN == (lvc.fmt & HDF_SORTDOWN))
            return HDF_SORTDOWN;
        else if (HDF_SORTUP == (lvc.fmt & HDF_SORTUP))
            return HDF_SORTUP;

        return -1;
    };

    BOOL DeleteAllColumns()
    {
        BOOL bResult = DeleteAllItems();

        for (int i = GetHeaderCtrl()->GetItemCount() - 1 ; i >= 0; --i)
        {
            bResult = bResult && DeleteColumn( i );
        }

        return bResult;
    };

    void SetItemHeight( int height )
    {
        m_imageList.Detach();
        m_imageList.Create( 1, m_dpiAware.ScaleY( height ), ILC_COLOR4, 1, 1 );
        SetImageList( &m_imageList, LVSIL_SMALL );
    };

    BOOL SelectAll()
    {
        return SetItemState( -1, LVIS_SELECTED, LVIS_SELECTED );
    }

protected:
    PTM_WARNING_DISABLE
    virtual const AFX_MSGMAP* GetMessageMap() const
    {
        return GetThisMessageMap();
    };

    static const AFX_MSGMAP* PASCAL GetThisMessageMap()
    {
        typedef CListCtrlEx ThisClass;
        typedef CListCtrl TheBaseClass;
        static const AFX_MSGMAP_ENTRY _messageEntries[] = {
            // Message map
            ON_WM_CONTEXTMENU()

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
        int  iItem;
        
        // if Shift-F10
        if (ptMousePos.x == -1 && ptMousePos.y == -1)
        {
            iItem = GetSelectionMark();

            CRect rc;

            GetItemRect( iItem, &rc, LVIR_LABEL );

            ptMousePos = rc.CenterPoint();
        }
        else
        {
            ScreenToClient( &ptMousePos );

            UINT uFlags;

            iItem = HitTest( ptMousePos, &uFlags );
        }

        if (-1 == iItem)
            return;

        sig_prepare_cm( iItem, &m_contextMenu );
        if (!m_contextMenu || !m_contextMenu->m_hMenu)
            return;

        ClientToScreen( &ptMousePos );

        m_contextMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptMousePos.x, ptMousePos.y, GetParent() );
    };

    void PreSubclassWindow()
    {
        SetExtendedStyle( GetExtendedStyle() |
            LVS_EX_FULLROWSELECT |
            LVS_EX_INFOTIP |
            LVS_EX_DOUBLEBUFFER );

        #ifdef UNICODE
        if (m_7Style)
            SetWindowTheme( m_hWnd, L"explorer", 0 );
        #endif

        LoadItemImages();

        CListCtrl::PreSubclassWindow();
    };

protected:
    bool       m_7Style;
    CImageList m_imageList;
    CMenu*     m_contextMenu;
    CDPI       m_dpiAware;

public:
    void OnPrepareContextMenu( const boost::function< void(int, CMenu**) >& f )
        { sig_prepare_cm.connect( f ); }

private:
    boost::signal< void(int, CMenu**) > sig_prepare_cm;
};
