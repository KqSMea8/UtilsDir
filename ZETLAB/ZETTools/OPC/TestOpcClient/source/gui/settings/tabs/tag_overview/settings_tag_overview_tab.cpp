#include "stdafx.h"
#include "../../../../application/application.h"
#include "settings_tag_overview_tab.h"

CTagOverviewTab::CTagOverviewTab( CWnd* pParent ) :
    CAccelerated< CTab, IDA_SETTINGS_TAG_OVERVIEW >(CTagOverviewTab::IDD, pParent)
{
}

CTagOverviewTab::~CTagOverviewTab()
{
}

void CTagOverviewTab::DoDataExchange( CDataExchange* pDX )
{
    CTab::DoDataExchange( pDX );

    DDX_Control( pDX, IDC_RULES, m_rules );
}

BEGIN_MESSAGE_MAP( CTagOverviewTab, CTab )
    ON_WM_DESTROY()
    ON_BN_CLICKED( ID_TO_UP,   OnToUp )
    ON_BN_CLICKED( ID_TO_DOWN, OnToDown )
END_MESSAGE_MAP()

BEGIN_ANCHOR_MAP( CTagOverviewTab )
    ANCHOR_MAP_ENTRY( IDC_LABEL_HIGHLIGHT_RULES, ANF_LEFT | ANF_TOP | ANF_RIGHT )
    ANCHOR_MAP_ENTRY( IDC_RULES,                 ANF_TOPLEFT | ANF_BOTTOMRIGHT )
    ANCHOR_MAP_ENTRY( ID_TO_UP,                  ANF_TOP | ANF_RIGHT )
    ANCHOR_MAP_ENTRY( ID_TO_DOWN,                ANF_TOP | ANF_RIGHT )
END_ANCHOR_MAP()

BOOL CTagOverviewTab::OnInitDialog()
{
    CTab::OnInitDialog();

    // Localization
    LOCALIZED_CONTROL( IDC_LABEL_HIGHLIGHT_RULES );
    LOCALIZED_CONTROL( ID_TO_UP );
    LOCALIZED_CONTROL( ID_TO_DOWN );

    // Saved bundle
    RestoreWindow();

    return TRUE;
}

void CTagOverviewTab::OnDestroy()
{
    SaveWindow();

    CTab::OnDestroy();
}

void CTagOverviewTab::OnToUp()
{
}

void CTagOverviewTab::OnToDown()
{
}

void CTagOverviewTab::RestoreWindow()
{
    info_bundle* info;
    UINT         sizeBundle;

    if (theApp.GetProfileBinary( _T(""), _T("Settings.TagViewTab"), (LPBYTE*)&info, &sizeBundle ) &&
        sizeof(*info) == sizeBundle)
    {
        m_rules.SetColumnWidth( CRuleList::column_where,     info->colWhereWidth );
        m_rules.SetColumnWidth( CRuleList::column_condition, info->colConditionWidth );
        m_rules.SetColumnWidth( CRuleList::column_what,      info->colWhatWidth );
        m_rules.SetColumnWidth( CRuleList::column_color,     info->colColorWidth );

        delete info;
	info = nullptr;
    }
}

void CTagOverviewTab::SaveWindow()
{
    info_bundle info;

    info.colWhereWidth     = m_rules.GetColumnWidth( CRuleList::column_where );
    info.colConditionWidth = m_rules.GetColumnWidth( CRuleList::column_condition );
    info.colWhatWidth      = m_rules.GetColumnWidth( CRuleList::column_what );
    info.colColorWidth     = m_rules.GetColumnWidth( CRuleList::column_color );

    theApp.WriteProfileBinary( _T(""), _T("Settings.TagViewTab"), (LPBYTE)&info, sizeof(info) );
}
