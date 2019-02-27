#pragma once

#include <zlo.hpp>

#include "../../../utils/list/list.h"

#include "../../../../algo/hightlight_rule/hightlight_rule.hpp"

#include <vector>
#include <map>

class CTagList : public CListCtrlEx
{
private:
    typedef std::vector< zlo::client::group::item* > item_list;
    typedef std::map< VARTYPE, int >   icon_map;
    typedef utils::highlight_rule_list rule_list;

public:
    enum column {
         column_fullName = 0,
         column_value,
         column_quality,
         column_timestamp,
         column_type,
    };

public:
    virtual void LoadItemImages();

public:
    zlo::client::group::item* const GetAsTag( int nItem );

    void Update();

    int AddItem( zlo::client::browser::item* item, BOOL bRecursive );

    void DeleteTag( int iItem );

    void DeleteAllClientTags( zlo::client* const client );

    int GetItemIndex( zlo::client::group::item* const item );

    void SetHighlightRules( const rule_list& rules );

protected:
    afx_msg void OnGetDispInfo( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnSearchItem( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnSort( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnDoubleClick( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult );

    DECLARE_MESSAGE_MAP()    

private:
    item_list _tags;
    icon_map  _icons;
    rule_list _rules;
};
