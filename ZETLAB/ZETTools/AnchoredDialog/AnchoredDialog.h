#pragma once

#include "../AnchorMap/AnchorMap.h"

class CAnchoredDialog : public CDialog
{
public:
    CAnchoredDialog( UINT nIDTemplate, CWnd* pParent = NULL ) :
        CDialog(nIDTemplate, pParent) {};

    virtual ~CAnchoredDialog()
    {
    };

protected:
    PTM_WARNING_DISABLE
    virtual const AFX_MSGMAP* GetMessageMap() const
    {
        return GetThisMessageMap();
    }

    static const AFX_MSGMAP* PASCAL GetThisMessageMap()
    {
        typedef CAnchoredDialog ThisClass;
        typedef CDialog TheBaseClass;
        static const AFX_MSGMAP_ENTRY _messageEntries[] = {
            // Message map
            ON_WM_SIZE()

            {0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 }
        };

        static const AFX_MSGMAP messageMap = {
            &TheBaseClass::GetThisMessageMap, &_messageEntries[0]
        };

        return &messageMap;
    }
    PTM_WARNING_RESTORE

    virtual void DoDataExchange( CDataExchange* pDX )
    {
        CDialog::DoDataExchange( pDX );
    }

    virtual BOOL OnInitDialog()
    {
        CDialog::OnInitDialog();

        InitAnchors();

        return TRUE;
    }

    virtual void InitAnchors( DWORD dwFlags = 0 )
    {
        m_bpfxAnchorMap.Initialize( m_hWnd, dwFlags );

        RECT rcWnd;
        ::GetWindowRect( m_hWnd, &rcWnd );
        m_bpfxAnchorMap.HandleAnchors( &rcWnd );
    }

    virtual void HandleAnchors( RECT* pRect )
    {
        m_bpfxAnchorMap.HandleAnchors( pRect );
    }

    afx_msg void OnSize( UINT nType, int cx, int cy )
    {
        CDialog::OnSize( nType, cx, cy );

        HandleAnchors( NULL );
        Invalidate( false );
    }

protected:
    CBPCtrlAnchorMap m_bpfxAnchorMap;
};
