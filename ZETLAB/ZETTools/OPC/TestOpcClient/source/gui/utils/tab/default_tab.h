#pragma once

#include <AnchoredDialog/AnchoredDialog.h>
#include <Localized/Localized.h>

class CTab :
    public CAnchoredDialog,
    public CLocalized
{
public:
    CTab( UINT nIDTemplate, CWnd* pParent = NULL ) :
        CAnchoredDialog(nIDTemplate, pParent),
        n_tooltips(1)
    {
    };

    virtual ~CTab()
    {
    };

protected:
    virtual void DoDataExchange( CDataExchange* pDX )
    {
        CAnchoredDialog::DoDataExchange( pDX );
    };

    virtual BOOL OnInitDialog()
    {
        CAnchoredDialog::OnInitDialog();

        m_brush.Detach();
        m_brush.CreateSolidBrush( GetSysColor( COLOR_WINDOW ) );

        InitAnchors();

        InitToolTips();

        return TRUE;
    };

    virtual BOOL PreTranslateMessage( MSG* pMsg )
    {
        m_toolTips.RelayEvent( pMsg );

        return CAnchoredDialog::PreTranslateMessage( pMsg );
    };

    virtual void InitToolTips()
    {
        if (m_toolTips.m_hWnd)
            m_toolTips.DestroyToolTipCtrl();

        m_toolTips.Create( this );
        m_toolTips.SetMaxTipWidth( 300 );
    };

    virtual void OnOK()
    {
        GetParent()->GetParent()->SendMessage( WM_COMMAND, IDOK, 0 );
    };

    virtual void OnCancel()
    {
        GetParent()->GetParent()->SendMessage( WM_COMMAND, IDCANCEL, 0 );
    };

    void AddToolTip( CWnd* const pWnd, UINT nResID )
    {
        if (csLocalization.LoadString( nResID ))
            AddToolTip( pWnd, csLocalization );
    };

    void AddToolTip( CWnd* const pWnd, LPCTSTR lpszText )
    {
        CRect rc;

        pWnd->GetClientRect( rc );
        m_toolTips.AddTool( pWnd, lpszText, rc, n_tooltips++ );
    };

protected:
    PTM_WARNING_DISABLE
    virtual const AFX_MSGMAP* GetMessageMap() const
    {
        return GetThisMessageMap();
    };

    static const AFX_MSGMAP* PASCAL GetThisMessageMap()
    {
        typedef CTab ThisClass;
        typedef CAnchoredDialog TheBaseClass;
        static const AFX_MSGMAP_ENTRY _messageEntries[] = {
            // Message map
            ON_WM_SIZE()
            ON_WM_CTLCOLOR()

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
        CAnchoredDialog::OnSize( nType, cx, cy );

        HandleAnchors( NULL );
        Invalidate   ( false );
    };

    afx_msg HBRUSH OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
    {
        return m_brush;
    };
    
    virtual void InitAnchors( DWORD dwFlags = 0 )
    {
        m_bpfxAnchorMap.Initialize( m_hWnd, dwFlags );
        RECT rcWnd;
        ::GetWindowRect( m_hWnd, &rcWnd );
        m_bpfxAnchorMap.HandleAnchors( &rcWnd );
    };

    virtual void HandleAnchors( RECT *pRect )
    {
        m_bpfxAnchorMap.HandleAnchors( pRect );
    };

protected:
    CBPCtrlAnchorMap m_bpfxAnchorMap;
    CToolTipCtrl     m_toolTips;
    unsigned int     n_tooltips;
    CBrush           m_brush;
};
