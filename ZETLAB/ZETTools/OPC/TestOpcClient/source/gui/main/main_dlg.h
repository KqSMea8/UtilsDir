#pragma once

#include <Accelerated/Accelerated.h>
#include <SplittedDialog/SplittedDialog.h>
#include <Localized/Localized.h>
#include <GdiPlused/GdiPlused.h>

#include "main_toolbar/main_toolbar.h"

class CTestOpcClientDlg :
    public CAccelerated< CSplittedDialog, IDA_MAIN >,
    public CLocalized,
    public CGdiPlused
{
public:
    CTestOpcClientDlg( CWnd* pParent = NULL );
    enum { IDD = IDD_MAIN };

protected:
    virtual void DoDataExchange( CDataExchange* pDX );
    virtual BOOL OnInitDialog();
    virtual void OnOK() {};
    virtual void OnCancel() {};

    afx_msg void	OnSysCommand( UINT nID, LPARAM lParam );
    afx_msg void    OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void    OnDestroy();
    afx_msg void    OnClose();
    afx_msg void    OnConnectRequest();
    afx_msg void    OnDisconnectRequest();
    afx_msg void    OnRpcPingRequest();
    afx_msg void    OnDcomConfiguratorRequest();
    afx_msg void    OnSettingsRequest();
    afx_msg LRESULT OnDisconnectAvailable( WPARAM wParam, LPARAM lParam );

    void CreateToolbar( UINT nIDContainer, UINT nIDResource );

    DECLARE_MESSAGE_MAP()
    DECLARE_ANCHOR_MAP()

protected:
    #pragma pack(push, 4) // Alignment by 4 bytes

    typedef struct _info_bundle
    {
        int  cx;
        int  cy;
        bool maximized;
        int colClientTree;
        int colTagOverview;

    } info_bundle;

    #pragma pack(pop)

    void RestoreWindow();
    void SaveWindow();

protected:
    HICON        m_hIcon;
    CMainToolBar m_toolbar;
};
