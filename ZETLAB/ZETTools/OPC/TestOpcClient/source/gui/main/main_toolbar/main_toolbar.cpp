#include "stdafx.h"
#include "../../../application/application.h"
#include "main_toolbar.h"

void CMainToolBar::LoadItemImages()
{
    int width  = m_dpiAware.ScaleX( 16 );
    int height = m_dpiAware.ScaleY( 16 );

    m_imageList.Detach();
    m_imageList.Create( width, height, ILC_COLOR32, 1, 1 );

    // Load icons
    m_imageList.Add( LOAD_ICON(IDI_CONNECT,    width, height ));
    m_imageList.Add( LOAD_ICON(IDI_DISCONNECT, width, height ));
    m_imageList.Add( LOAD_ICON(IDI_RPC_PING,   width, height ));

    m_toolbar.GetToolBarCtrl().SetImageList( &m_imageList );
}

int CMainToolBar::Create(
    CWnd* pParentWnd,
    UINT  nIDContainer,
    UINT  nIDResource )
{
    BOOL ret = CToolBarEx::Create( pParentWnd, nIDContainer, nIDResource, CBRS_ALIGN_ANY );

    if (ret)
    {
        m_toolbar.ModifyStyle( 0, TBSTYLE_LIST );

        SetButtonText( ID_CONNECT,    IDS_TB_CONNECT );
        SetButtonText( ID_DISCONNECT, IDS_TB_DISCONNECT );
        SetButtonText( ID_RPC_PING,   IDS_TB_RPC_PING );

        EnableButton ( ID_DISCONNECT, FALSE );
    }

    return ret;
}
