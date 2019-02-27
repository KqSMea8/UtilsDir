#pragma once

#include <AnchoredDialog/AnchoredDialog.h>
#include <Localized/Localized.h>

#include <utils/rpc_ping/zlo_rpc_ping.hpp>

class CRpcPingDlg :
    public CAnchoredDialog,
    public CLocalized
{
public:
    CRpcPingDlg( CWnd* pParent = NULL );

    enum { IDD = IDD_RPC_PING };

protected:
    virtual void DoDataExchange( CDataExchange* pDX );
    virtual BOOL OnInitDialog();
    virtual void OnOK() {};
    virtual void DoWait( const BOOL& wait_ = TRUE );

    afx_msg void OnDestroy();
    afx_msg BOOL OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message );
    afx_msg void OnRpcPingRequest();
    afx_msg void OnClose();

    void    OnRpcPingResponse( zlo::utils::obtained_return< bool > status );
    LRESULT OnRpcPingResponse_s( WPARAM wParam, LPARAM lParam );

    DECLARE_MESSAGE_MAP()
    DECLARE_ANCHOR_MAP()

protected:
    void RestoreWindow();
    void SaveWindow();

protected:
    BOOL    m_wait;
    HICON   m_hIcon;
    CString m_host;
    int     m_timeout;
    CStatic m_status;
    CString m_statusText;
    CString m_statusExt;
};
