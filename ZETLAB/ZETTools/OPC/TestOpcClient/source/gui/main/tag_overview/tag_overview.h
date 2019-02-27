#pragma once

#include <Accelerated/Accelerated.h>
#include <Localized/Localized.h>

#include <zlo.hpp>

#include "tag_list/tag_list.h"
#include "tag_properties/tag_properties_dlg.h"

#include <list>

class CTagOverview :
    public CAccelerated< CView, IDA_TAG_OVERVIEW >,
    public CLocalized
{
protected:
    CTagOverview();
    DECLARE_DYNCREATE(CTagOverview)

public:
    virtual ~CTagOverview();

public:
    virtual void OnDraw( CDC* pDC );
    virtual void OnInitialUpdate();

public:
    void OnTagChange( zlo::client::group::item* const tag );

    void OnDisconnect( zlo::client* const client );

protected:
    afx_msg int     OnCreate( LPCREATESTRUCT lpCreateStruct );
    afx_msg void    OnSize( UINT nType, int cx, int cy );
    afx_msg void    OnDestroy();
    afx_msg LRESULT OnAddItemRequest( WPARAM wParam, LPARAM lParam );
    afx_msg void    OnCopyTagNameRequest();
    afx_msg void    OnRemoveTagRequest();
    afx_msg LRESULT OnTagPropertiesRequestIndirect( WPARAM wParam, LPARAM lParam );
    afx_msg void    OnTagPropertiesRequest();
    afx_msg void    OnSelectAllRequest();
    afx_msg void    OnPrepareContextMenu( int iItem, CMenu** contextMenu );

    DECLARE_MESSAGE_MAP()

private:
    void TagProperties( const CTagPropertiesDlg::item_list& tags );

protected:
    #pragma pack(push, 4) // Alignment by 4 bytes

    typedef struct _info_bundle
    {
        int colFullNameWidth;
        int colValueWidth;
        int colQualityWidth;
        int colTimestampWidth;
        int colValueTypeWidth;
        int columnOrderArray[10];
    
    } info_bundle;

    #pragma pack(pop)

    void RestoreWindow();
    void SaveWindow();

protected:
    CTagList m_tags;
    CMenu    m_contextMenu;

private:
    bool m_allowTagChange;
};
