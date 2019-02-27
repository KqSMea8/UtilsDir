#include "stdafx.h"
#include "application.h"
#include "../gui/main/main_dlg.h"

#ifdef _DEBUG
#   define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP( CTestOpcClientApp, CWinApp )
END_MESSAGE_MAP()

CTestOpcClientApp::CTestOpcClientApp()
{
}

CTestOpcClientApp theApp;

BOOL CTestOpcClientApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC  = ICC_WIN95_CLASSES;
    InitCommonControlsEx( &InitCtrls );

    CWinApp::InitInstance();

    CMFCVisualManager::SetDefaultManager( RUNTIME_CLASS(CMFCVisualManagerWindows7) );

    AfxEnableControlContainer();

    SetRegistryKey( _T("ZET\\ZETLab") );
    SetAppID      ( _T("ZET.ZETLab") );

    CTestOpcClientDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
