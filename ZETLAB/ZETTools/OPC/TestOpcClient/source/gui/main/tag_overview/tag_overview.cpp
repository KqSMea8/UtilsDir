#include "stdafx.h"
#include "../../../application/application.h"
#include "tag_overview.h"

#include "../../../algo/hightlight_rule/hightlight_rule.hpp"

#include <boost/bind.hpp>

#include <string>

IMPLEMENT_DYNCREATE( CTagOverview, CView )

BEGIN_MESSAGE_MAP( CTagOverview, CView )
    ON_WM_CREATE ()
    ON_WM_SIZE   ()
    ON_WM_DESTROY()
    ON_MESSAGE   ( ID_ADD_ITEM,       OnAddItemRequest )
    ON_COMMAND   ( ID_COPY_TAG_NAME,  OnCopyTagNameRequest )
    ON_COMMAND   ( ID_REMOVE_TAG,     OnRemoveTagRequest )
    ON_MESSAGE   ( ID_TAG_PROPERTIES, OnTagPropertiesRequestIndirect )
    ON_COMMAND   ( ID_TAG_PROPERTIES, OnTagPropertiesRequest )
    ON_COMMAND   ( ID_SELECT_ALL,     OnSelectAllRequest )
END_MESSAGE_MAP()

CTagOverview::CTagOverview()
{
    m_allowTagChange = true;
}

CTagOverview::~CTagOverview()
{
}

void CTagOverview::OnDraw( CDC* pDC )
{
}

void CTagOverview::OnInitialUpdate() 
{
    CView::OnInitialUpdate();
}

int CTagOverview::OnCreate( LPCREATESTRUCT lpCreateStruct ) 
{
    if (CView::OnCreate( lpCreateStruct ) == -1)
        return -1;

    BOOL ret = m_tags.Create(
        WS_CHILD | WS_VISIBLE | WS_TABSTOP |
        LVS_REPORT | LVS_ALIGNLEFT | LVS_SHOWSELALWAYS | LVS_OWNERDATA,
        CRect(),
        this,
        IDC_LIST
    );

    m_tags.LoadItemImages(); // WHY???????
    
    m_tags.OnPrepareContextMenu(
        boost::bind( &CTagOverview::OnPrepareContextMenu, this, _1, _2 )
    );

    // Saved bundle
    RestoreWindow();

    return (ret ? 0 : -1);
}


void CTagOverview::OnSize( UINT nType, int cx, int cy ) 
{
    CView::OnSize( nType, cx, cy );

    if (IsWindow( m_tags.m_hWnd ))
        m_tags.SetWindowPos( &wndTop, 0, 0, cx, cy, SWP_NOACTIVATE );
}

void CTagOverview::OnDestroy()
{
    SaveWindow();

    CView::OnDestroy();
}

void CTagOverview::OnTagChange( zlo::client::group::item* const tag )
{
    if (!m_allowTagChange)
        return;

    int nItem = m_tags.GetItemIndex( tag );

    if (0 <= nItem && nItem < m_tags.GetItemCount())
        m_tags.RedrawItems( nItem, nItem );
}

void CTagOverview::OnDisconnect( zlo::client* const client )
{
    m_tags.DeleteAllClientTags( client );
    m_tags.Update();
}

LRESULT CTagOverview::OnAddItemRequest( WPARAM wParam, LPARAM lParam )
{
    zlo::client::browser::item* item =
        reinterpret_cast< zlo::client::browser::item* >(wParam);

    ASSERT(
        (zlo::client::browser::item::type_node == item->getItemType()) ||
        (zlo::client::browser::item::type_tag  == item->getItemType())
    );

    m_allowTagChange = false;

    int nItem = m_tags.AddItem( item, BOOL(lParam) );
    m_tags.Update ();
    m_tags.EnsureVisible( nItem, FALSE );

    m_allowTagChange = true;

    return 0;
}

void CTagOverview::OnCopyTagNameRequest()
{
    if (m_tags.GetSelectedCount() <= 0)
        return;

    if (OpenClipboard())
    {
        EmptyClipboard();

        POSITION pos = m_tags.GetFirstSelectedItemPosition();

        std::wstring string;
        while (pos)
        {
            int nItem                      = m_tags.GetNextSelectedItem( pos );
            zlo::client::group::item* item = m_tags.GetAsTag( nItem );

            ASSERT( item );

            if (!string.empty())
                string += _T("\r\n");
            string += item->getName();
        }

        size_t sizeInBytes = (string.size() + 1) * sizeof(TCHAR);
        HGLOBAL hGlobal    = GlobalAlloc( GMEM_DDESHARE, sizeInBytes );
        LPCTSTR buffer     = reinterpret_cast< LPCTSTR >(GlobalLock( hGlobal ));

        memcpy_s( (void*)buffer, sizeInBytes, string.c_str(), sizeInBytes );

        GlobalUnlock    ( hGlobal );
    #if UNICODE
        SetClipboardData( CF_UNICODETEXT, hGlobal );
    #else
        SetClipboardData( CF_TEXT, hGlobal );
    #endif
        CloseClipboard  ();
    }
}

void CTagOverview::OnRemoveTagRequest()
{
    if (m_tags.GetSelectedCount() <= 0)
        return;

    POSITION pos = m_tags.GetFirstSelectedItemPosition();

    int shift = 0;
    while (pos)
    {
        int nItem = m_tags.GetNextSelectedItem( pos );

        m_tags.SetItemState( nItem, 0, LVIS_SELECTED );
        m_tags.DeleteTag   ( nItem - shift++ );
    }

    m_tags.Update();
}

LRESULT CTagOverview::OnTagPropertiesRequestIndirect( WPARAM wParam, LPARAM lParam )
{
    CTagPropertiesDlg::item_list tags;

    tags.push_back( reinterpret_cast< zlo::client::group::item* >(wParam) );

    TagProperties( tags );

    return 0;
}

void CTagOverview::OnTagPropertiesRequest()
{
    if (m_tags.GetSelectedCount() <= 0)
        return;

    POSITION                     pos = m_tags.GetFirstSelectedItemPosition();
    CTagPropertiesDlg::item_list tags;

    while (pos)
    {
        int nItem = m_tags.GetNextSelectedItem( pos );
        tags.push_back( m_tags.GetAsTag( nItem ) );
    }

    TagProperties( tags );
}

void CTagOverview::TagProperties( const CTagPropertiesDlg::item_list& tags )
{
    CTagPropertiesDlg propertiesDlg;

    if (IDOK == propertiesDlg.DoModal( tags ))
    {
        for (auto it = tags.begin(), end = tags.end(); it != end; ++it)
        {
            CString value = propertiesDlg.m_value;

            if (value == _T("<>"))
                value = (*it)->getValueStr().c_str();

            WORD quality = _qualities[propertiesDlg.m_qualityIndex];

            if (0xffff == quality)
                quality = (*it)->getQuality();

            (*it)->write( (LPCTSTR)value, quality );
        }
    }
}

void CTagOverview::OnSelectAllRequest()
{
    m_tags.SelectAll();
}

void CTagOverview::OnPrepareContextMenu( int iItem, CMenu** contextMenu )
{
    ASSERT( iItem >= 0 );

    // Invalidate old menu
    m_contextMenu.DestroyMenu();

    m_contextMenu.LoadMenu( IDM_TAG_OVERVIEW );

    LOCALIZED_MENU_ITEM( (&m_contextMenu), ID_COPY_TAG_NAME );
    LOCALIZED_MENU_ITEM( (&m_contextMenu), ID_REMOVE_TAG );
    LOCALIZED_MENU_ITEM( (&m_contextMenu), ID_TAG_PROPERTIES );

    *contextMenu = m_contextMenu.GetSubMenu( 0 );
    (*contextMenu)->SetDefaultItem( ID_TAG_PROPERTIES );
}

void CTagOverview::RestoreWindow()
{
    info_bundle* info;
    UINT         sizeBundle;

    if (theApp.GetProfileBinary( _T(""), _T("Main.TagView"), (LPBYTE*)&info, &sizeBundle ) &&
        sizeof(*info) == sizeBundle)
    {
        m_tags.SetColumnWidth( CTagList::column_fullName,  info->colFullNameWidth );
        m_tags.SetColumnWidth( CTagList::column_value,     info->colValueWidth );
        m_tags.SetColumnWidth( CTagList::column_quality,   info->colQualityWidth );
        m_tags.SetColumnWidth( CTagList::column_timestamp, info->colTimestampWidth );
        m_tags.SetColumnWidth( CTagList::column_type,      info->colValueTypeWidth );

        m_tags.SetColumnOrderArray( m_tags.GetHeaderCtrl()->GetItemCount(), info->columnOrderArray );

        delete info;
	info = nullptr;
    }

    m_tags.SetHighlightRules(
        utils::deserialize_rules(
            theApp.GetProfileString( _T(""), _T("Settings.Rules") )
        )
    );
}

void CTagOverview::SaveWindow()
{
    info_bundle info;

    info.colFullNameWidth  = m_tags.GetColumnWidth( CTagList::column_fullName );
    info.colValueWidth     = m_tags.GetColumnWidth( CTagList::column_value );
    info.colQualityWidth   = m_tags.GetColumnWidth( CTagList::column_quality );
    info.colTimestampWidth = m_tags.GetColumnWidth( CTagList::column_timestamp );
    info.colValueTypeWidth = m_tags.GetColumnWidth( CTagList::column_type );

    m_tags.GetColumnOrderArray( info.columnOrderArray, m_tags.GetHeaderCtrl()->GetItemCount() );

    theApp.WriteProfileBinary( _T(""), _T("Main.TagView"), (LPBYTE)&info, sizeof(info) );
}
