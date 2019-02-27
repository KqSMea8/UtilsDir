//------------------------------------------------------------------------------
//	����� ������������ ��� ������ ������� ������ ����� (�� �����!).
//	�������� ���������� ���������������� ������.
//------------------------------------------------------------------------------
/*	������������� ������:
	CFolderDialog cd(L"����� ����� ��� ...", L"Maker", L"c:", this);
	if (cd.DoModal() == IDOK)
	{
		CString folder = cd.GetSelectedFolder();
	}	*/
//------------------------------------------------------------------------------
/*	���� ����� �������� ������, �� ���������� ������ ����� ���������.
	���� ������ �������� ����� (��������: �:), �� ����� ����� ������ �
��������� (������ �� ����� �).
	���� ������ ������� �����, �� �� ��������� ��� ����� ���������.
	��������� - ��� �� ��� ���� ������. �� ������������ ������ ����,
� ������� ������.

	���� ������� ������� ���������, ��������� � ���������� �����.

	����� ������
	m_bAddingSlash(false) - ���������� � ����� ��������� ����� � ����� �����	  
	m_bStatusText(true) - ����������� ��������� ����� ��� ����������.
	m_bViewFilesInFolder(true) - ����������� ������ � �����. ������ �� ��� ����.
	m_bCreateNewFolder(false) - ������ �������� ����� �����. true ���������
			m_bStatusText � m_bViewFilesInFolder;			*/
//------------------------------------------------------------------------------
#pragma once
#include "afxwin.h"
//------------------------------------------------------------------------------
class CFolderDialog
{
private:
	HWND m_hWndOwner;
	CString m_sTitle;
	CString m_sRoot;
	CString m_sFolder;

protected:

public:
	CFolderDialog(const CString sTitle = L"", const CString sCurFolder = L"",
		const CString sRoot = L"", const CWnd *pOwner = nullptr);
//	virtual ~CFolderDialog();

	bool m_bAddingSlash;
	bool m_bCreateNewFolder;
	bool m_bStatusText;
	bool m_bViewFilesInFolder;

	// ������� ������� --------------------------------------------------------
	CString GetSelectedFolder();
	CString GetRootFolder();
	CString GetTitle();

	void SetCurFolder(const CString sCurFolder);
	void SetOwner(const CWnd *pOwner);
	void SetRootFolder(const CString root);
	void SetTitle(const CString title);

	// ������� ������ ---------------------------------------------------------
	INT_PTR DoModal();
};
//------------------------------------------------------------------------------