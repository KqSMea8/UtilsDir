#include "stdafx.h"
#include "../../application/application.h"
#include "rpc_ping_dlg.h"

#include <boost/bind.hpp>

#ifdef _DEBUG
#   define new DEBUG_NEW
#endif

CRpcPingDlg::CRpcPingDlg( CWnd* pParent ) :
    CAnchoredDialog(CRpcPingDlg::IDD, pParent)
{
    m_wait  = FALSE;
    m_hIcon = AfxGetApp()->LoadIcon( IDI_RPC_PING );
    m_statusText.LoadString( IDS_STATUS_UNCERTAIN );
}

void CRpcPingDlg::DoDataExchange( CDataExchange* pDX )
{
    CAnchoredDialog::DoDataExchange( pDX );

    DDX_Text   ( pDX, IDC_HOST,        m_host );
    DDX_Text   ( pDX, IDC_TIMEOUT,     m_timeout );
    DDX_Control( pDX, IDC_STATUS,      m_status );
    DDX_Text   ( pDX, IDC_STATUS_TEXT, m_statusText );
    DDX_Text   ( pDX, IDC_STATUS_EXT,  m_statusExt );
}

BEGIN_MESSAGE_MAP( CRpcPingDlg, CAnchoredDialog )
    ON_WM_DESTROY()
    ON_WM_SETCURSOR()
    ON_BN_CLICKED  ( IDC_RPC_PING, OnRpcPingRequest )
    ON_MESSAGE     ( WM_USER + 1,  OnRpcPingResponse_s )
    ON_BN_CLICKED  ( IDCLOSE,      OnClose )
END_MESSAGE_MAP()

BEGIN_ANCHOR_MAP( CRpcPingDlg )
END_ANCHOR_MAP()

BOOL CRpcPingDlg::OnInitDialog()
{
    CAnchoredDialog::OnInitDialog();

    // Localization
    LOCALIZED_WINDOW ( IDD_RPC_PING );
    LOCALIZED_CONTROL( IDC_RPC_PING_HELP );
    LOCALIZED_CONTROL( IDC_LABEL_HOST );
    LOCALIZED_CONTROL( IDC_LABEL_TIMEOUT );
    LOCALIZED_CONTROL( IDC_RPC_PING );
    LOCALIZED_CONTROL( IDCLOSE );

    // Icon
    SetIcon( m_hIcon, FALSE );

    // Saved bundle
    RestoreWindow();

    return TRUE;
}

void CRpcPingDlg::OnDestroy()
{
    SaveWindow();
}

BOOL CRpcPingDlg::OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message )
{
    if (m_wait)
    {
        AfxGetApp()->DoWaitCursor( 0 );

        return TRUE;
    }

    return CAnchoredDialog::OnSetCursor( pWnd, nHitTest, message );
}

void CRpcPingDlg::OnRpcPingRequest()
{
    DoWait();

    UpdateData();

    zlo::utils::rpc_ping_async(
        (LPCTSTR)m_host,
        m_timeout,
        boost::bind( &CRpcPingDlg::OnRpcPingResponse, this, _1 )
    );
}

void CRpcPingDlg::OnRpcPingResponse( zlo::utils::obtained_return< bool > status )
{
    if (m_hWnd && IsWindow( m_hWnd ))
        SendMessage( WM_USER + 1, reinterpret_cast< WPARAM >(&status) );
}

LRESULT CRpcPingDlg::OnRpcPingResponse_s( WPARAM wParam, LPARAM lParam )
{
    zlo::utils::obtained_return< bool >* status =
        reinterpret_cast< zlo::utils::obtained_return< bool >* >(wParam);

    DWORD icon   = *status ? IDI_STATUS_GOOD : IDI_STATUS_BAD;
    DWORD string = *status ? IDS_STATUS_GOOD : IDS_STATUS_BAD;

    m_status.    SetIcon   ( LOAD_ICON(icon, 16, 16) );
    m_statusText.LoadString( string );
    m_statusExt. Format    ( _T("0x%08X: %s"), status->id(),
                                               status->what().c_str() );
    UpdateData( FALSE );

    DoWait( FALSE );

    return 0;
}

void CRpcPingDlg::OnClose()
{
    OnCancel();
}

void CRpcPingDlg::RestoreWindow()
{    
    m_host    = theApp.GetProfileString( _T(""), _T("RpcPing.Host"),    _T("") );
    m_timeout = theApp.GetProfileInt   ( _T(""), _T("RpcPing.Timeout"), 500 );

    UpdateData( FALSE );
}

void CRpcPingDlg::SaveWindow()
{
    UpdateData();

    theApp.WriteProfileString( _T(""), _T("RpcPing.Host"),    m_host );
    theApp.WriteProfileInt   ( _T(""), _T("RpcPing.Timeout"), m_timeout );
}

void CRpcPingDlg::DoWait( const BOOL& wait_ )
{
    m_wait = wait_;

    AfxGetApp()->DoWaitCursor( m_wait ? 1 : -1 );
}
