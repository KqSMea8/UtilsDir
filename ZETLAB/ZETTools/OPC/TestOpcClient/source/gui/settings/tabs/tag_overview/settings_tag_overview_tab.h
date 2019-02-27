#pragma once

#include <Accelerated/Accelerated.h>

#include "../../../utils/tab/default_tab.h"

#include "rule_list/rule_list.h"

class CTagOverviewTab : public CAccelerated< CTab, IDA_SETTINGS_TAG_OVERVIEW >
{
public:
    CTagOverviewTab( CWnd* pParent = NULL );
    virtual ~CTagOverviewTab();

    enum { IDD = IDD_SETTINGS_TAG_OVERVIEW_TAB };
    enum { IDS = IDS_IDD_SETTINGS_TAG_OVERVIEW_TAB };

protected:
    virtual void DoDataExchange( CDataExchange* pDX );
    virtual BOOL OnInitDialog();

    afx_msg void    OnDestroy();
    afx_msg void    OnToUp();
    afx_msg void    OnToDown();

    DECLARE_MESSAGE_MAP()
    DECLARE_ANCHOR_MAP()

protected:
    #pragma pack(push, 4) // Alignment by 4 bytes

    typedef struct _info_bundle
    {
        int colWhereWidth;
        int colConditionWidth;
        int colWhatWidth;
        int colColorWidth;

    } info_bundle;

    #pragma pack(pop)

    void RestoreWindow();
    void SaveWindow();

protected:
    CRuleList m_rules;
};
