#pragma once

#include "../../../../utils/list/list.h"

#include "../../../../../algo/hightlight_rule/hightlight_rule.hpp"

class CRuleList : public CListCtrlEx
{
private:
    typedef utils::highlight_rule_list rule_list;

public:
    enum column {
         column_where = 0,
         column_condition,
         column_what,
         column_color,
    };

public:
    virtual void LoadItemImages();

protected:
    afx_msg void OnGetDispInfo( NMHDR* pNMHDR, LRESULT* pResult );

    DECLARE_MESSAGE_MAP()

private:
    rule_list _rules;
};
