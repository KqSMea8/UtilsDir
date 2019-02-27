#pragma once

#include <AnchoredDialog/AnchoredDialog.h>
#include <Localized/Localized.h>

#include <zlo.hpp>

#include "host_tree/host_tree.h"

class CConnectDlg :
    public CAnchoredDialog,
    public CLocalized
{
public:
    CConnectDlg( CWnd* pParent = NULL );

    enum { IDD = IDD_CONNECT };

protected:
    virtual void DoDataExchange( CDataExchange* pDX );
    virtual BOOL OnInitDialog();
    virtual void OnOK();

    void OnChange();

    void OnChange(
        zlo::host*   host,
        zlo::server* server );

    void OnConnect( zlo::server* server );

    DECLARE_MESSAGE_MAP()
    DECLARE_ANCHOR_MAP()

private:
    BOOL Connect();

    void MessageHostUnreachable();

    void MessageOpcEnumUnavailable();

    void MessageOpcServerNotFound();

    void MessageCreateInstanceFailed();

public:
    zlo::session_ptr _session;

protected:
    HICON     m_hIcon;
    CHostTree m_tree;
    CString   m_host;
    CString   m_opcServer;
};
