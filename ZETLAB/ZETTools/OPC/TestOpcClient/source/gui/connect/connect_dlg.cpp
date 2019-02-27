#include "stdafx.h"
#include "../../application/application.h"
#include "connect_dlg.h"

#include "../../algo/network/network.hpp"

#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <boost/range/algorithm/find_if.hpp>

#ifdef _DEBUG
#   define new DEBUG_NEW
#endif

CConnectDlg::CConnectDlg( CWnd* pParent ) :
    CAnchoredDialog(CConnectDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon( IDI_CONNECT );
}

void CConnectDlg::DoDataExchange( CDataExchange* pDX )
{
    CAnchoredDialog::DoDataExchange( pDX );

    DDX_Control ( pDX, IDC_TREE,       m_tree );
    DDX_CBString( pDX, IDC_HOST,       m_host );
    DDX_CBString( pDX, IDC_OPC_SERVER, m_opcServer );
}

BEGIN_MESSAGE_MAP( CConnectDlg, CAnchoredDialog )
    ON_CBN_EDITCHANGE( IDC_HOST,       OnChange )
    ON_CBN_EDITCHANGE( IDC_OPC_SERVER, OnChange )
END_MESSAGE_MAP()

BEGIN_ANCHOR_MAP( CConnectDlg )
    ANCHOR_MAP_ENTRY( IDC_TREE,             ANF_AUTOMATIC )
    ANCHOR_MAP_ENTRY( IDC_LABEL_HOST,       ANF_BOTTOMLEFT )
    ANCHOR_MAP_ENTRY( IDC_LABEL_OPC_SERVER, ANF_BOTTOMRIGHT )
    ANCHOR_MAP_ENTRY( IDC_HOST,             ANF_BOTTOMLEFT )
    ANCHOR_MAP_ENTRY( IDC_OPC_SERVER,       ANF_BOTTOMRIGHT )
    ANCHOR_MAP_ENTRY( IDOK,                 ANF_BOTTOMRIGHT )
    ANCHOR_MAP_ENTRY( IDCANCEL,             ANF_BOTTOMRIGHT )
END_ANCHOR_MAP()

BOOL CConnectDlg::OnInitDialog()
{
    CAnchoredDialog::OnInitDialog();

    // Localization
    LOCALIZED_WINDOW ( IDD_CONNECT );
    LOCALIZED_CONTROL( IDC_LABEL_HOST );
    LOCALIZED_CONTROL( IDC_LABEL_OPC_SERVER );
    LOCALIZED_CONTROL( IDOK );
    LOCALIZED_CONTROL( IDCANCEL );

    // Icon
    SetIcon( m_hIcon, FALSE );

    m_tree.OnChange ( boost::bind( &CConnectDlg::OnChange,  this, _1, _2 ) );
    m_tree.OnConnect( boost::bind( &CConnectDlg::OnConnect, this, _1 ) );

    m_tree.AddHost   ( NULL, boost::make_shared< zlo::host >(_T("localhost")) );
    m_tree.AddNetwork( NULL, boost::make_shared< utils::network >() );

    return TRUE;
}

void CConnectDlg::OnChange()
{
    UpdateData();

    GetDlgItem( IDOK )->EnableWindow( !m_host.IsEmpty() && !m_opcServer.IsEmpty() );
}

void CConnectDlg::OnChange(
    zlo::host*   host,
    zlo::server* server )
{
    m_host      = host   ? host->  getName().  c_str() : _T("");
    m_opcServer = server ? server->getProgId().c_str() : _T("");

    GetDlgItem( IDOK )->EnableWindow( !m_host.IsEmpty() && !m_opcServer.IsEmpty() );

    UpdateData( FALSE );
}

void CConnectDlg::OnConnect( zlo::server* server )
{
    OnOK(); // will call Connect()
}

BOOL CConnectDlg::Connect()
{
    zlo::host_ptr host = boost::make_shared< zlo::host >((LPCTSTR)m_host);

    try
    {
        zlo::server_list servers = host->browse();

        auto server = boost::range::find_if( servers, [&]( const zlo::server_ptr& s ) {
            return s->getName()   == (LPCTSTR)m_opcServer ||
                   s->getProgId() == (LPCTSTR)m_opcServer;
        });

        if (servers.end() == server)
        {
            MessageOpcServerNotFound();

            return FALSE;
        }

        _session = (*server)->connect();

        return TRUE;
    }
    catch (zlo::host::host_unreachable&)
    {
        MessageHostUnreachable();
    }
    catch (zlo::host::opc_enum_unavailable&)
    {
        MessageOpcEnumUnavailable();
    }
    catch (zlo::create_instance_failed&)
    {
        MessageCreateInstanceFailed();
    }

    return FALSE;
}

void CConnectDlg::MessageHostUnreachable()
{
    CString caption;
    CString message;
    
    caption.LoadString( IDS_CONNECT_ERROR );
    message.LoadString( IDS_HOST_UNREACHABLE );

    MessageBox( message, caption, MB_OK | MB_ICONERROR );
}

void CConnectDlg::MessageOpcEnumUnavailable()
{
    CString caption;
    CString message;

    caption.LoadString( IDS_CONNECT_ERROR );
    message.LoadString( IDS_OPC_ENUM_UNAVAILABLE );

    MessageBox( message, caption, MB_OK | MB_ICONERROR );
}

void CConnectDlg::MessageOpcServerNotFound()
{
    CString caption;
    CString message;

    caption.LoadString( IDS_CONNECT_ERROR );
    message.LoadString( IDS_OPC_SERVER_NOT_FOUND );

    MessageBox( message, caption, MB_OK | MB_ICONERROR );
}

void CConnectDlg::MessageCreateInstanceFailed()
{
    CString caption;
    CString message;

    caption.LoadString( IDS_CONNECT_ERROR );
    message.LoadString( IDS_CREATE_INSTANCE_FAILED );

    MessageBox( message, caption, MB_OK | MB_ICONERROR );
}

void CConnectDlg::OnOK()
{
    if (!Connect())
        return;

    CAnchoredDialog::OnOK();
}
