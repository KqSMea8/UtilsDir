#pragma once

#include <AnchoredDialog/AnchoredDialog.h>
#include <Localized/Localized.h>

#include "../utils/tab/tabs.h"

#include "tabs/tag_overview/settings_tag_overview_tab.h"

class CSettingsDlg :
    public CAnchoredDialog,
    public CLocalized
{
public:
    CSettingsDlg( CWnd* pParent = NULL );

    enum { IDD = IDD_SETTINGS };

protected:
    virtual void DoDataExchange( CDataExchange* pDX );
    virtual BOOL OnInitDialog();
    virtual void OnOK();

    void InitTabs();

    DECLARE_MESSAGE_MAP()
    DECLARE_ANCHOR_MAP()

protected:
    HICON           m_hIcon;
    CTabs           m_tabs;
    CTagOverviewTab m_tagOverview;
};
