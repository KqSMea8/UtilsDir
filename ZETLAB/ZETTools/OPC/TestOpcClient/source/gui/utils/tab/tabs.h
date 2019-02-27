#pragma once

class CTabs : public CTabCtrl
{
public:
    CTabs()
    {
    };

    virtual ~CTabs()
    {
    };

public:
    //
    template< typename T >
    inline LONG InsertItem( int nItem, T* dlg, int nImage )
    {
        // Create window
        dlg->Create( T::IDD, this );

        // Tab text
        CString cstrCaption;
        cstrCaption.LoadString( T::IDS );

        // Insert tab and associate with window
        return CTabCtrl::InsertItem(
            TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM,
            nItem, cstrCaption, nImage,
            reinterpret_cast< LPARAM >(dlg)
        );
    }

    int SetActiveTab( int nItem )
    {
        OnSelChangingTabs( NULL, NULL );

        int nOld = SetCurSel( nItem );

        OnSelChangeTabs( NULL, NULL );

        return nOld;
    };

    BOOL SetTabName( int nItem, LPCTSTR lpszText )
    {
        TCITEM tci;

        tci.mask    = TCIF_TEXT;
        tci.pszText = const_cast< LPTSTR >(lpszText);

        return SetItem( nItem, &tci );
    };

protected:
    PTM_WARNING_DISABLE
    virtual const AFX_MSGMAP* GetMessageMap() const
    {
        return GetThisMessageMap();
    };

    static const AFX_MSGMAP* PASCAL GetThisMessageMap()
    {
        typedef CTabs ThisClass;
        typedef CTabCtrl TheBaseClass;
        static const AFX_MSGMAP_ENTRY _messageEntries[] = {
            // Message map
            ON_WM_SIZE()
            ON_NOTIFY_REFLECT( TCN_SELCHANGING, OnSelChangingTabs )
            ON_NOTIFY_REFLECT( TCN_SELCHANGE,   OnSelChangeTabs )

            { 0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 }
        };

        static const AFX_MSGMAP messageMap = {
            &TheBaseClass::GetThisMessageMap, &_messageEntries[0]
        };

        return &messageMap;
    };
    PTM_WARNING_RESTORE

    afx_msg void OnSize( UINT nType, int cx, int cy )
    {
        CTabCtrl::OnSize( nType, cx, cy );

        OnSelChangeTabs( NULL, NULL );
    };

    afx_msg void OnSelChangingTabs( NMHDR* pNMHDR, LRESULT* pResult )
    {
        if (!IsWindow( this->m_hWnd )) return;

        // Hide current tab
        TCITEM		tci;
        CDialog*	tabView;

        tci.mask	= TCIF_PARAM;
        if (!GetItem( GetCurSel(), &tci )) return;

        tabView		= reinterpret_cast< CDialog* >(tci.lParam);
        if (tabView && tabView->m_hWnd && IsWindow( tabView->m_hWnd ))
        {
            tabView->SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE );
        }
    };

    afx_msg void OnSelChangeTabs( NMHDR* pNMHDR, LRESULT* pResult )
    {
        if (!IsWindow( this->m_hWnd )) return;

        // Show new tab
        TCITEM		tci;
        CDialog*	tabView;
        CRect		rc;

        GetWindowRect( &rc );
        ScreenToClient( &rc );
        AdjustRect( FALSE, &rc );

        tci.mask	= TCIF_PARAM;
        if (!GetItem( GetCurSel(), &tci )) return;

        tabView		= reinterpret_cast< CDialog* >(tci.lParam);
        if (tabView && tabView->m_hWnd && IsWindow( tabView->m_hWnd ))
        {
            tabView->SetWindowPos( &wndTop, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW );
        }
    };
};
