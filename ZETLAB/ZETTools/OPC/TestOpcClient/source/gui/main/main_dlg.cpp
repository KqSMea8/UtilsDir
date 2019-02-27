#include "stdafx.h"
#include "../../application/application.h"
#include "main_dlg.h"

#include "client_tree_view/client_tree_view.h"
#include "tag_overview/tag_overview.h"

#include "../connect/connect_dlg.h"
#include "../rpc_ping/rpc_ping_dlg.h"
#include "../settings/settings_dlg.h"

#include <zlo.hpp>

#include <boost/make_shared.hpp>

#ifdef _DEBUG
#   define new DEBUG_NEW
#endif

CTestOpcClientDlg::CTestOpcClientDlg( CWnd* pParent ) :
    CAccelerated< CSplittedDialog, IDA_MAIN >(CTestOpcClientDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon( IDI_MAIN );
}

void CTestOpcClientDlg::DoDataExchange( CDataExchange* pDX )
{
    CSplittedDialog::DoDataExchange( pDX );
}

BEGIN_MESSAGE_MAP( CTestOpcClientDlg, CSplittedDialog )
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_DESTROY()
    ON_WM_CLOSE()
    ON_COMMAND( ID_CONNECT,              OnConnectRequest )
    ON_COMMAND( ID_DISCONNECT,           OnDisconnectRequest )
    ON_COMMAND( ID_RPC_PING,             OnRpcPingRequest )
    ON_COMMAND( ID_DCOM_CONFIGURATOR,    OnDcomConfiguratorRequest )
    ON_COMMAND( ID_SETTINGS,             OnSettingsRequest )
    ON_MESSAGE( ID_DISCONNECT_AVAILABLE, OnDisconnectAvailable )
END_MESSAGE_MAP()

BEGIN_ANCHOR_MAP( CTestOpcClientDlg )
    ANCHOR_MAP_ENTRY( NULL, ANF_AUTOMATIC )
END_ANCHOR_MAP()

BOOL CTestOpcClientDlg::OnInitDialog()
{
    CSplittedDialog::OnInitDialog();

    // Localization
    LOCALIZED_WINDOW    ( IDD_MAIN );
    LOCALIZED_MENU_POPUP( MAIN_MENU, 0, IDS_CONNECTION );
    LOCALIZED_MENU_ITEM ( MAIN_MENU, ID_CONNECT );
    LOCALIZED_MENU_ITEM ( MAIN_MENU, ID_DISCONNECT );
    LOCALIZED_MENU_POPUP( MAIN_MENU, 1, IDS_TOOLS );
    LOCALIZED_MENU_ITEM ( MAIN_MENU, ID_RPC_PING );
    LOCALIZED_MENU_ITEM ( MAIN_MENU, ID_DCOM_CONFIGURATOR );
    LOCALIZED_MENU_ITEM ( MAIN_MENU, ID_SETTINGS );

    // Icon
    SetIcon( m_hIcon, TRUE );
    SetIcon( m_hIcon, FALSE );

    // Toolbar
    CreateToolbar( IDC_TOOLBAR, IDT_MAIN );

    // Splitter
    SplitWindow( 1, 2 );
    CreateView ( 0, 0, RUNTIME_CLASS(CClientTreeView), CSize() );
    CreateView ( 0, 1, RUNTIME_CLASS(CTagOverview),    CSize() );

    // Saved bundle
    RestoreWindow();

    return TRUE;
}

void CTestOpcClientDlg::OnSysCommand( UINT nID, LPARAM lParam )
{
    if (SC_CLOSE == nID)
        PostQuitMessage( WM_QUIT );

    CSplittedDialog::OnSysCommand( nID, lParam );
}

void CTestOpcClientDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);

        SendMessage( WM_ICONERASEBKGND, reinterpret_cast< WPARAM >(dc.GetSafeHdc()), 0 );

        int cxIcon = GetSystemMetrics( SM_CXICON );
        int cyIcon = GetSystemMetrics( SM_CYICON );
        CRect rc;
        GetClientRect( &rc );
        int x = (rc.Width() - cxIcon + 1) / 2;
        int y = (rc.Height() - cyIcon + 1) / 2;

        dc.DrawIcon( x, y, m_hIcon );
    }
    else
    {
        CSplittedDialog::OnPaint();
    }
}

HCURSOR CTestOpcClientDlg::OnQueryDragIcon()
{
    return static_cast< HCURSOR >(m_hIcon);
}

void CTestOpcClientDlg::OnDestroy()
{
    SaveWindow();

    CSplittedDialog::OnDestroy();
}

void CTestOpcClientDlg::OnClose()
{
    PostQuitMessage( WM_QUIT );
}

void CTestOpcClientDlg::OnConnectRequest()
{
    CConnectDlg connectDlg;

    if (IDOK == connectDlg.DoModal())
    {
        if (!connectDlg._session)
            return;

        unsigned long updateRate = 1000;

        CClientTreeView* clients = 
            reinterpret_cast< CClientTreeView* >(m_splitter.GetPane( 0, 0 ));

        CTagOverview* tags = 
            reinterpret_cast< CTagOverview* >(m_splitter.GetPane( 0, 1 ));

        // Connect clients with tags
        clients->ConnectWithTags( tags );

        // Add new client
        clients->AddClient( boost::make_shared< zlo::client >(connectDlg._session, updateRate) );
    }
}

void CTestOpcClientDlg::OnDisconnectRequest()
{
    CClientTreeView* clients = 
        reinterpret_cast< CClientTreeView* >(m_splitter.GetPane( 0, 0 ));

    clients->OnDisconnectRequest();
}

void CTestOpcClientDlg::OnRpcPingRequest()
{
    CRpcPingDlg rpcPingDlg;

    rpcPingDlg.DoModal();
}

void CTestOpcClientDlg::OnDcomConfiguratorRequest()
{
}

void CTestOpcClientDlg::OnSettingsRequest()
{
    CSettingsDlg settingsDlg;

    settingsDlg.DoModal();
}

LRESULT CTestOpcClientDlg::OnDisconnectAvailable( WPARAM wParam, LPARAM lParam )
{
    zlo::client* client = reinterpret_cast< zlo::client* >(wParam);

    m_toolbar.EnableButton( ID_DISCONNECT, !!client );

    GetMenu()->GetSubMenu( 0 )->EnableMenuItem(
        ID_DISCONNECT,
        MF_BYCOMMAND | (!!client ? MF_ENABLED : (MF_DISABLED | MF_GRAYED))
    );

    return 0;
}

void CTestOpcClientDlg::CreateToolbar( UINT nIDContainer, UINT nIDResource )
{
    ASSERT( GetDlgItem( nIDContainer) );

    m_toolbar.Create( this, nIDContainer, nIDResource );
}

void CTestOpcClientDlg::RestoreWindow()
{
    info_bundle* info;
    UINT         sizeBundle;

    if (theApp.GetProfileBinary( _T(""), _T("Main.Window"), (LPBYTE*)&info, &sizeBundle ) &&
        sizeof(*info) == sizeBundle)
    {
        SetWindowPos( &wndTop, 0, 0, info->cx, info->cy, SWP_NOMOVE );

        if (info->maximized)
            ShowWindow( SW_MAXIMIZE );

        SetColumnInfo( 0, info->colClientTree,  0 );
        SetColumnInfo( 1, info->colTagOverview, 0 );

        delete info;
	info = nullptr;
    }
    else
    {
        SetWindowPos ( &wndTop, 0, 0, 600, 450, SWP_NOMOVE );
        SetColumnInfo( 0, 200, 0 );
        SetColumnInfo( 1, 358, 0 );
    }
}

void CTestOpcClientDlg::SaveWindow()
{
    WINDOWPLACEMENT wps;
    int             colClientTree;
    int             colTagOverview;
    int             tmp;

    GetWindowPlacement( &wps );
    GetColumnInfo     ( 0, colClientTree,  tmp );
    GetColumnInfo     ( 1, colTagOverview, tmp );

    info_bundle info;

    info.cx             = wps.rcNormalPosition.right - wps.rcNormalPosition.left;
    info.cy             = wps.rcNormalPosition.bottom - wps.rcNormalPosition.top;
    info.maximized      = (SW_SHOWMAXIMIZED == wps.showCmd);
    info.colClientTree  = colClientTree;
    info.colTagOverview = colTagOverview;

    theApp.WriteProfileBinary( _T(""), _T("Main.Window"), (LPBYTE)&info, sizeof(info) );
}
