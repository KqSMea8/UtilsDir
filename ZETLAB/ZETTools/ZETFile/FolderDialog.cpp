//------------------------------------------------------------------------------
#include "stdafx.h"
#include "shlobj.h"
#include <ZetFile\FolderDialog.h>
#include <ZETFile\include\ZETFile.h>
//------------------------------------------------------------------------------
static int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	TCHAR szPath[_MAX_PATH] = { 0 };
	switch (uMsg)
	{
	case BFFM_INITIALIZED:
		if (lpData)
			SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lpData);
		break;

	case BFFM_SELCHANGED:
		SHGetPathFromIDListW(reinterpret_cast<LPITEMIDLIST>(lParam), szPath);
		SendMessage(hWnd, BFFM_SETSTATUSTEXT, NULL, reinterpret_cast<LPARAM>(szPath));
		break;
	}
	return 0;
}
//------------------------------------------------------------------------------
CFolderDialog::CFolderDialog(const CString sTitle, const CString sFolder,
	const CString sRoot, const CWnd *pOwner)
	: m_hWndOwner(NULL)
	, m_bAddingSlash(false)
	, m_bCreateNewFolder(false)
	, m_bStatusText(true)
	, m_bViewFilesInFolder(true)
{
	SetTitle(sTitle);
	SetCurFolder(sFolder);
	SetRootFolder(sRoot);
	SetOwner(pOwner);
}
//------------------------------------------------------------------------------
//CFolderDialog::~CFolderDialog()
//{
//}
//------------------------------------------------------------------------------
CString CFolderDialog::GetSelectedFolder()
{
	if (m_bAddingSlash)
		return m_sFolder + L"\\";
	else
		return m_sFolder;
}
//------------------------------------------------------------------------------
CString CFolderDialog::GetRootFolder()
{
	return m_sFolder;
}
//------------------------------------------------------------------------------
CString CFolderDialog::GetTitle()
{
	return m_sTitle;
}
//------------------------------------------------------------------------------
void CFolderDialog::SetCurFolder(const CString sCurFolder)
{
	if (NoError == zfDirectoryExists(sCurFolder))
		m_sFolder = sCurFolder;
}
//------------------------------------------------------------------------------
void CFolderDialog::SetTitle(const CString sTitle)
{
	m_sTitle = sTitle;
}
//------------------------------------------------------------------------------
void CFolderDialog::SetRootFolder(const CString sRoot)
{
	if (NoError == zfDirectoryExists(sRoot))
		m_sRoot = sRoot;
}
//------------------------------------------------------------------------------
void CFolderDialog::SetOwner(const CWnd *pOwner)
{
	if (pOwner != nullptr)
		m_hWndOwner = pOwner->GetSafeHwnd();
	else
		m_hWndOwner = NULL;
}
//------------------------------------------------------------------------------
INT_PTR CFolderDialog::DoModal()
{
	INT_PTR ret(IDCANCEL);
	LPMALLOC pMalloc(nullptr);
	if (CoGetMalloc(1, &pMalloc) == S_OK)
	{	// «адание полей структуры BROWSEINFO
		TCHAR sFolder[_MAX_PATH] = { 0 };		
		BROWSEINFO bi;
		ZeroMemory(&bi, sizeof(BROWSEINFO));
		bi.ulFlags = BIF_RETURNONLYFSDIRS;
		if (m_bViewFilesInFolder)
			bi.ulFlags |= BIF_BROWSEINCLUDEFILES;
		if (m_bStatusText)
			bi.ulFlags |=  BIF_STATUSTEXT;
		if (m_bCreateNewFolder)
			bi.ulFlags |= BIF_NEWDIALOGSTYLE;

		bi.hwndOwner = m_hWndOwner;
		bi.lpszTitle = m_sTitle.GetString();
		bi.lpfn = BrowseCallbackProc;
		bi.lParam = (LPARAM)m_sFolder.GetString();
		
		// задание корневого каталога
		LPITEMIDLIST pIdlRoot(nullptr);
		bool bRoot = !m_sRoot.IsEmpty() && NoError == zfDirectoryExists(m_sRoot);
		if (bRoot)
		{
			IShellFolder *pDF;
			bRoot = SHGetDesktopFolder(&pDF) == S_OK;
			if (bRoot)
			{				
				ULONG dwAttributes(0);
				bRoot = pDF->ParseDisplayName(NULL, NULL,
					(LPTSTR)m_sRoot.GetString(), nullptr,
					&pIdlRoot, &dwAttributes) == S_OK;
				if (bRoot)
					bi.pidlRoot = pIdlRoot;
				pDF->Release();
			}
		}

		// выбор папки
		LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
		if (pidl != nullptr)
		{	// определение полного пути выбранной папки
			if (SHGetPathFromIDList(pidl, sFolder) != FALSE)
			{
				ret = IDOK;
				m_sFolder = (CString)sFolder;
			}
			CoTaskMemFree(pidl);
		}
		if (bRoot)
			CoTaskMemFree(pIdlRoot);
	}
	if (ret != IDOK)
		m_sFolder.Empty();
	return ret;
}
//------------------------------------------------------------------------------