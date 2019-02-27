#include "stdafx.h"
#include "../../../../../application/application.h"
#include "rule_list.h"

BEGIN_MESSAGE_MAP( CRuleList, CListCtrlEx )
    ON_NOTIFY_REFLECT( LVN_GETDISPINFO,  OnGetDispInfo )
END_MESSAGE_MAP()

void CRuleList::LoadItemImages()
{
    SetExtendedStyle(
        GetExtendedStyle()    |
        LVS_EX_DOUBLEBUFFER
    );

    InsertColumn( IDS_WHERE ),     SetColumnWidth( column_where,     LVSCW_AUTOSIZE_USEHEADER );
    InsertColumn( IDS_CONDITION ), SetColumnWidth( column_condition, LVSCW_AUTOSIZE_USEHEADER );
    InsertColumn( IDS_WHAT ),      SetColumnWidth( column_what,      LVSCW_AUTOSIZE_USEHEADER );
    InsertColumn( IDS_COLOR ),     SetColumnWidth( column_color,     LVSCW_AUTOSIZE_USEHEADER );
}

void CRuleList::OnGetDispInfo( NMHDR* pNMHDR, LRESULT* pResult )
{
    NMLVDISPINFO* pDispInfo = reinterpret_cast< NMLVDISPINFO* >(pNMHDR);

    *pResult = CDRF_DODEFAULT;

    LVITEM* pItem = &(pDispInfo)->item; 

    if (pItem->mask & LVIF_TEXT)
    {
        switch (pItem->iSubItem)
        {
//         case column_where:     _tcscpy_s( pItem->pszText, MAX_PATH, _rules[pItem->iItem].where );        break;
//         case column_condition: _tcscpy_s( pItem->pszText, MAX_PATH, _rules[pItem->iItem].condition );    break;
        case column_what:      _tcscpy_s( pItem->pszText, MAX_PATH, _rules[pItem->iItem].what.c_str() ); break;
//         case column_color:     _tcscpy_s( pItem->pszText, MAX_PATH, _rules[pItem->iItem].color );        break;
        default:               _tcscpy_s( pItem->pszText, MAX_PATH, _T("") ); break;
        }
    }
}
