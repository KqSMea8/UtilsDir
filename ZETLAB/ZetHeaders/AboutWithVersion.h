#pragma once
/*
����������� ������� ������ �������� �� About �������
                                    �������� ������ � cpp-����
���������� ������ CAboutDlg 
  � ������������ ��� ����������� ������ ��������

                                    ����������:
 *  ������� ������� IDD_ABOUTBOX
   * � ������� - ������ IDC_STATIC1
 * ���������� ������� IDS_VERSION ���� "������� ������ %d.%d.%d.%d"
*/
#include "winver.h"
#pragma comment(lib, "version.lib")

#ifndef IDS_VERSION
#include <shlwapi.h>
#endif


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

inline CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

inline BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	const int stLen = 200;
	const int bufLen = 2000;
	TCHAR wstr[stLen], lpchar[bufLen];
	//	���������� ���� ������������ �����
	::GetModuleFileNameW(NULL, wstr, stLen);
	DWORD verLen = GetFileVersionInfoSize(wstr, NULL);
	// ��������� ������ � ����� � �����
	BOOL bRes = GetFileVersionInfo(wstr, NULL, verLen, &lpchar);
	// ��������� ������ ������ �� ��������������� ���������� � ������ �����
	void *Pointer;
	// ����� ����� ������ ������ � ������������ �����������
	unsigned int Length = bufLen;
	// ������ ��������� � �����
	VerQueryValue(lpchar, _T("\\"), &Pointer, &Length);
	VS_FIXEDFILEINFO FileVer;
	// ����������� ���������� � ������ �����
	memmove(&FileVer, Pointer, Length);
	unsigned short Major, Minor, Release, Build;
	Major   = HIWORD(FileVer.dwFileVersionMS);
	Minor   = LOWORD(FileVer.dwFileVersionMS);
	Release = HIWORD(FileVer.dwFileVersionLS);
	Build   = LOWORD(FileVer.dwFileVersionLS);
	CString sVersion;
#ifdef IDS_VERSION
	sVersion.Format(IDS_VERSION, Major, Minor, Release, Build);
#else
#pragma warning("IDS_VERSION string resource is not defined. using module name instead")
	ASSERT(FALSE);
	sVersion.Format(_T("%s ������ %d.%d.%d.%d"),PathFindFileNameW(wstr) , Major, Minor, Release, Build);
#endif
#ifndef IDC_STATIC1
#pragma error("IDC_STATIC1 static text should be created on About dialog")
#endif
	CWnd* pItem = GetDlgItem(IDC_STATIC1);
	ASSERT(pItem);
	if (pItem)
		pItem->SetWindowText(sVersion);
	return TRUE;
}

inline void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()