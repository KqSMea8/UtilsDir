#include "stdafx.h"
#include "../../../../application/application.h"
#include "tag_list.h"

#include "../../../../algo/ico_generator/ico_generator.hpp"

#include <boost/range/algorithm/find.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <boost/range/algorithm/remove_if.hpp>

BEGIN_MESSAGE_MAP( CTagList, CListCtrlEx )
    ON_NOTIFY_REFLECT( LVN_GETDISPINFO,  OnGetDispInfo )
    ON_NOTIFY_REFLECT( LVN_ODFINDITEM,   OnSearchItem )
    ON_NOTIFY        ( HDN_ITEMCLICK, 0, OnSort )
    ON_NOTIFY_REFLECT( NM_DBLCLK,        OnDoubleClick )
    ON_NOTIFY_REFLECT( NM_CUSTOMDRAW,    OnCustomDraw )
END_MESSAGE_MAP()

void CTagList::LoadItemImages()
{
    SetExtendedStyle(
        GetExtendedStyle()    |
        LVS_EX_FULLROWSELECT  |
        LVS_EX_INFOTIP        |
        LVS_EX_HEADERDRAGDROP |
        LVS_EX_DOUBLEBUFFER
    );

    InsertColumn( IDS_TAG_NAME ),      SetColumnWidth( column_fullName,  LVSCW_AUTOSIZE_USEHEADER );
    InsertColumn( IDS_TAG_VALUE ),     SetColumnWidth( column_value,     LVSCW_AUTOSIZE_USEHEADER );
    InsertColumn( IDS_TAG_QUALITY ),   SetColumnWidth( column_quality,   LVSCW_AUTOSIZE_USEHEADER );
    InsertColumn( IDS_TAG_TIMESTAMP ), SetColumnWidth( column_timestamp, LVSCW_AUTOSIZE_USEHEADER );
    InsertColumn( IDS_TAG_TYPE ),      SetColumnWidth( column_type,      LVSCW_AUTOSIZE_USEHEADER );

    int width  = m_dpiAware.ScaleX( 16 );
    int height = m_dpiAware.ScaleY( 16 );

    m_imageList.Detach();
    m_imageList.Create( width, height, ILC_COLOR32, 1, 1 );

    SetImageList( &m_imageList, LVSIL_SMALL );
}

void CTagList::OnGetDispInfo( NMHDR* pNMHDR, LRESULT* pResult )
{
    NMLVDISPINFO* pDispInfo = reinterpret_cast< NMLVDISPINFO* >(pNMHDR);

    *pResult = CDRF_DODEFAULT;

    LVITEM* pItem = &(pDispInfo)->item; 

    if (pItem->mask & LVIF_TEXT)
    {
        switch (pItem->iSubItem)
        {
        case column_fullName:  _tcscpy_s( pItem->pszText, MAX_PATH, _tags[pItem->iItem]->getName().c_str() );         break;
        case column_value:     _tcscpy_s( pItem->pszText, MAX_PATH, _tags[pItem->iItem]->getValueStr().c_str() );     break;
        case column_quality:   _tcscpy_s( pItem->pszText, MAX_PATH, _tags[pItem->iItem]->getQualityStr().c_str() );   break;
        case column_timestamp: _tcscpy_s( pItem->pszText, MAX_PATH, _tags[pItem->iItem]->getTimestampStr().c_str() ); break;
        case column_type:      _tcscpy_s( pItem->pszText, MAX_PATH, _tags[pItem->iItem]->getTypeStr().c_str() );      break;
        default:               _tcscpy_s( pItem->pszText, MAX_PATH, _T("") ); break;
        }
    }
    if (pItem->mask & LVIF_IMAGE)
    {
        VARTYPE vt = _tags[pItem->iItem]->getType();
        auto    it = _icons.find( vt );

        if (_icons.end() == it)
        {
            int width  = m_dpiAware.ScaleX( 16 );
            int height = m_dpiAware.ScaleY( 16 );

            _icons[vt] = m_imageList.Add( utils::generate_ico( vt, width, height ) );
        }

        pItem->iImage = _icons[vt];
    }
}

void CTagList::OnSearchItem( NMHDR* pNMHDR, LRESULT* pResult )
{
    NMLVFINDITEM* pFindInfo = reinterpret_cast< NMLVFINDITEM* >(pNMHDR);

    *pResult = -1;

    if (pFindInfo->lvfi.flags & LVFI_STRING)
    {
        LPCTSTR filter       = pFindInfo->lvfi.psz;
        int     filterLength = _tcslen( filter );

        auto it = boost::range::find_if( _tags, [&](zlo::client::group::item* const i) {
            return !_tcsnicmp( i->getName().c_str(), filter, filterLength );
        });

        if (_tags.end() != it)
            *pResult = (it - _tags.begin());
    }
}

void CTagList::OnSort( NMHDR* pNMHDR, LRESULT* pResult )
{
    LPNMLISTVIEW pNMLV = reinterpret_cast< LPNMLISTVIEW >(pNMHDR);

    *pResult = CDRF_DODEFAULT;

    int  marker    = GetColumnSortMarker( pNMLV->iItem );
    bool ascending = true;

    if (HDF_SORTDOWN == marker)
        marker = HDF_SORTUP;
    else
        marker = HDF_SORTDOWN, ascending = false;

    typedef zlo::client::group::item item;

    switch (pNMLV->iItem)
    {
    case column_fullName:
        boost::range::sort( _tags, [&](item* const i1, item* const i2) {
            return ascending ?
                   0 > _tcscmp( i1->getName().c_str(), i2->getName().c_str() ):
                   0 < _tcscmp( i1->getName().c_str(), i2->getName().c_str() );
        });
        break;

    case column_value:
        boost::range::sort( _tags, [&](item* const i1, item* const i2) {
            return ascending ?
                   0 > _tcscmp( i1->getValueStr().c_str(), i2->getValueStr().c_str() ):
                   0 < _tcscmp( i1->getValueStr().c_str(), i2->getValueStr().c_str() );
        });
        break;

    case column_quality:
        boost::range::sort( _tags, [&](item* const i1, item* const i2) {
            return ascending ?
                   i1->getQuality() > i2->getQuality():
                   i1->getQuality() < i2->getQuality();
        });
        break;

    case column_timestamp:
        boost::range::sort( _tags, [&](item* const i1, item* const i2) {
            return ascending ?
                   i1->getTimestamp() < i2->getTimestamp():
                   i1->getTimestamp() > i2->getTimestamp();
        });
        break;

    case column_type:
        boost::range::sort( _tags, [&](item* const i1, item* const i2) {
            return ascending ?
                   i1->getType() > i2->getType():
                   i1->getType() < i2->getType();
        });
        break;
    }

    // Reset all old sort markers
    for (int i = 0, c = GetHeaderCtrl()->GetItemCount(); i < c; ++i)
        SetColumnSortMarker( i, (~HDF_SORTDOWN & ~HDF_SORTUP) );

    // Set sort marker for this column
    SetColumnSortMarker( pNMLV->iItem, marker );

    RedrawWindow();
}

void CTagList::OnDoubleClick( NMHDR* pNMHDR, LRESULT* pResult )
{
    *pResult = CDRF_DODEFAULT;

    int nItem = GetSelectionMark();

    if (-1 == nItem)
        return;

    ASSERT( size_t(nItem) < _tags.size() );

    GetParent()->SendMessage( ID_TAG_PROPERTIES, reinterpret_cast< WPARAM >(_tags[nItem]) );
}

void CTagList::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
{
    LPNMLVCUSTOMDRAW pNMCD = reinterpret_cast< LPNMLVCUSTOMDRAW >(pNMHDR);

    *pResult = CDRF_DODEFAULT;

    switch (pNMCD->nmcd.dwDrawStage)
    {
    case CDDS_PREPAINT:
        *pResult = CDRF_NOTIFYITEMDRAW;
        break;

    case CDDS_ITEMPREPAINT:
        *pResult = CDRF_NOTIFYSUBITEMDRAW;
        break;

    case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
        {
            // Black text color by default
            pNMCD->clrText = GetSysColor( COLOR_WINDOWTEXT );
            
            const int where = pNMCD->iSubItem;

            // First rule has highest priority, and it will be processed last.
            for (auto it = _rules.rbegin(), rend = _rules.rend(); it != rend; ++it)
            {
                if (where == it->where)
                {
                    zlo::client::group::item* const item =
                        _tags[pNMCD->nmcd.dwItemSpec];

                    bool condition = !it->condition;

                    switch (where)
                    {
                    case column_fullName:  condition = !!_tcsstr( item->getName().c_str(),         it->what.c_str() ); break;
                    case column_value:     condition = !!_tcsstr( item->getValueStr().c_str(),     it->what.c_str() ); break;
                    case column_quality:   condition = !!_tcsstr( item->getQualityStr().c_str(),   it->what.c_str() ); break;
                    case column_timestamp: condition = !!_tcsstr( item->getTimestampStr().c_str(), it->what.c_str() ); break;
                    case column_type:      condition = !!_tcsstr( item->getTypeStr().c_str(),      it->what.c_str() ); break;
                    }

                    if (( condition &&  it->condition) ||
                        (!condition && !it->condition))
                    {
                        pNMCD->clrText = it->color;
                    }
                }
            }
        }
        break;
    }
}

zlo::client::group::item* const CTagList::GetAsTag( int nItem )
{
    ASSERT( nItem >= 0 );
    ASSERT( size_t(nItem) < _tags.size() );

    return _tags[nItem];
}

void CTagList::Update()
{
    SetItemCount( _tags.size() );
}

int CTagList::AddItem( zlo::client::browser::item* item, BOOL bRecursive )
{
    zlo::client::group* group = item->getClient()->getGroup();

    if (group->isItemExists( item->getFullPath() ))
        return -1;

    if (zlo::client::browser::item::type_node == item->getItemType())
    {
        auto tags = item->browse( zlo::client::browser::filter_tag, false );

        for (auto it = tags.begin(), end = tags.end(); it != end; ++it)
        {
            AddItem( (*it).get(), FALSE );
        }

        if (bRecursive)
        {
            auto nodes = item->browse( zlo::client::browser::filter_node, false );

            for (auto it = nodes.begin(), end = nodes.end(); it != end; ++it)
            {
                AddItem( (*it).get(), TRUE );
            }
        }
    }
    else if (zlo::client::browser::item::type_tag == item->getItemType())
    {
        zlo::client::group::item* tag = group->addItem( item->getFullPath() );

        if (!tag)
            return -1;

        _tags.push_back( tag );
    }

    return _tags.size() - 1;
}

void CTagList::DeleteTag( int iItem )
{
    auto it = _tags.begin() + size_t(iItem);

    if (_tags.end() == it)
        return;

    (*it)->removeThis();

    _tags.erase( it );
}

void CTagList::DeleteAllClientTags( zlo::client* const client )
{
    auto new_last = boost::range::remove_if( _tags, [&](zlo::client::group::item* i)->bool {
        if (i->getGroup()->getClient() == client)
        {
            i->removeThis();
            return true;
        }
        return false;
    });

    _tags = item_list(_tags.begin(), new_last);
}

int CTagList::GetItemIndex( zlo::client::group::item* const item )
{
    auto it = boost::range::find( _tags, item );

    if (_tags.end() == it)
        return -1;

    return (it - _tags.begin());
}

void CTagList::SetHighlightRules( const rule_list& rules )
{
    _rules = rules;
}
