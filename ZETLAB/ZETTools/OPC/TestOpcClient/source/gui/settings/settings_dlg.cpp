#include "stdafx.h"
#include "../../application/application.h"
#include "settings_dlg.h"

#ifdef _DEBUG
#   define new DEBUG_NEW
#endif

CSettingsDlg::CSettingsDlg( CWnd* pParent ) :
    CAnchoredDialog(CSettingsDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon( IDI_SETTINGS );
}

void CSettingsDlg::DoDataExchange( CDataExchange* pDX )
{
    CAnchoredDialog::DoDataExchange( pDX );

    DDX_Control( pDX, IDC_TABS, m_tabs );
}

BEGIN_MESSAGE_MAP( CSettingsDlg, CAnchoredDialog )
END_MESSAGE_MAP()

BEGIN_ANCHOR_MAP( CSettingsDlg )
    ANCHOR_MAP_ENTRY( NULL, ANF_AUTOMATIC )
END_ANCHOR_MAP()

BOOL CSettingsDlg::OnInitDialog()
{
    CAnchoredDialog::OnInitDialog();

    // Localization
    LOCALIZED_WINDOW ( IDD_SETTINGS );
    LOCALIZED_CONTROL( IDOK );
    LOCALIZED_CONTROL( IDCANCEL );

    // Icon
    SetIcon( m_hIcon, FALSE );

    // Tabs
    InitTabs();

    return TRUE;
}

void CSettingsDlg::OnOK()
{
    CAnchoredDialog::OnOK();
}

void CSettingsDlg::InitTabs()
{
    m_tabs.InsertItem< CTagOverviewTab >( 0, &m_tagOverview, -1 );

    m_tabs.SetActiveTab( 0 );
}
