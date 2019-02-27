#pragma once

// Dpi aware
#include <CDPI/cdpi.h>

class CToolBarEx : public CStatic
{
private:
    class _CToolBarEx : public CToolBar
    {
    public:
        virtual BOOL Create( CWnd* pParentWnd, UINT nIDResource, DWORD dwAlignment )
        {
            if (CToolBar::Create( pParentWnd, WS_CHILD | WS_VISIBLE | CBRS_TOOLTIPS | dwAlignment) &&
                LoadToolBar( nIDResource ))
            {
                pParentWnd->RepositionBars( AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0 );

                return TRUE;
            }

            return FALSE;
        };

    protected:
        PTM_WARNING_DISABLE
        virtual const AFX_MSGMAP* GetMessageMap() const
        {
            return GetThisMessageMap();
        };

        static const AFX_MSGMAP* PASCAL GetThisMessageMap()
        {
            typedef _CToolBarEx ThisClass;
            typedef CToolBar TheBaseClass;
            static const AFX_MSGMAP_ENTRY _messageEntries[] = {
                // Message map
                ON_NOTIFY_EX_RANGE( TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText )

                { 0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 }
            };

            static const AFX_MSGMAP messageMap = {
                &TheBaseClass::GetThisMessageMap, &_messageEntries[0]
            };

            return &messageMap;
        };
        PTM_WARNING_RESTORE

        afx_msg BOOL OnToolTipText( UINT nID, NMHDR* pNMHDR, LRESULT* pResult )
        {
            LPTOOLTIPTEXT pTTT = reinterpret_cast< LPTOOLTIPTEXT >(pNMHDR);

            *pResult = CDRF_DODEFAULT;

            if (pTTT->hdr.idFrom)
            {
                CString cstrText;

                cstrText.LoadString( pTTT->hdr.idFrom );
                lstrcpyn( pTTT->szText, cstrText, sizeof(pTTT->szText) );

                return TRUE;
            }

            return FALSE;
        };
    };

public:
    virtual ~CToolBarEx()
    {
    };

public:
    virtual void LoadItemImages()
    {
    };

    virtual BOOL Create(
        CWnd* pParentWnd,
        UINT  nIDContainer,
        UINT  nIDResource,
        DWORD dwAlignment )
    {
        SubclassDlgItem( nIDContainer, pParentWnd );

        BOOL ret = m_toolbar.Create( this, nIDResource, dwAlignment );

        if (ret)
        {
            // Translate commands to parent window
            m_toolbar.SetOwner( pParentWnd );

            LoadItemImages();
        }

        return ret;
    };

    BOOL SetButtonText( UINT nID, UINT uResID )
    {
        int nIndex = m_toolbar.CommandToIndex( nID );

        // Autosize button
        m_toolbar.SetButtonStyle( nIndex, TBBS_AUTOSIZE );

        CString cstrText;
        cstrText.LoadString( uResID );

        return m_toolbar.SetButtonText( nIndex, cstrText );
    };

    void EnableButton( UINT nID, BOOL bEnable = TRUE )
    {
        m_toolbar.SetButtonStyle(
            m_toolbar.CommandToIndex( nID ), bEnable ? TBBS_BUTTON : TBBS_DISABLED
        );
    }

protected:
    PTM_WARNING_DISABLE
    virtual const AFX_MSGMAP* GetMessageMap() const
    {
        return GetThisMessageMap();
    };

    static const AFX_MSGMAP* PASCAL GetThisMessageMap()
    {
        typedef CToolBarEx ThisClass;
        typedef CStatic TheBaseClass;
        static const AFX_MSGMAP_ENTRY _messageEntries[] = {
            // Message map
            ON_WM_SIZE()

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
        CStatic::OnSize( nType, cx, cy );

        if (IsWindow( m_toolbar.m_hWnd ))
            m_toolbar.MoveWindow( 0, 0, cx, cy, FALSE );
    };

protected:
    _CToolBarEx m_toolbar;
    CImageList  m_imageList;
    CDPI        m_dpiAware;
};
