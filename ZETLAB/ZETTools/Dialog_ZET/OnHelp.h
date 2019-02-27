//------------------------------------------------------------------------------
/*			����� ������������ ��� ���������� ������ ������� � ����������,
���������� �� �� ������� ������ CDialog_ZET. ������� ����� ���� ���� � ���������
�����, ���� � ����� �����. ��� ���� �� ����� Dialog_ZET.cpp.

			������� Create.
		�������� pZetPath ��������� ��� ����������� ����� � ������� �������.
���� pZetPath = NULL, �� ��������� ������ CDZetPath ����� ������ �����������,
�� ��� ����� ��������� ������ pParent. ������� ����������� ������ ����
����� ����-�� ���� �� ���������� pZetPath � pParent.
*/
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <Dialog_ZET\CDZetPath.h>
//------------------------------------------------------------------------------
class COnHelp
{
private:
	CString m_sExeName;
	CString m_sSystemRoot;
	CString m_sFileHelp;
	PROCESS_INFORMATION m_piHelp;

protected:

public:
	COnHelp();
	~COnHelp();
	bool Create(CDZetPath *pZetPath, CWnd *pParent = NULL);
	bool Create(const CString sFolderHelp, CString chmName = L"");

	// ������� ������� --------------------------------------------------------
	CString GetSystemRoot() {return m_sSystemRoot;}
	CString GetExeName() {return m_sExeName;}
	CString GetFileHelp() {return m_sFileHelp;}

	// ������� ������ ---------------------------------------------------------
	bool OnHelp();
};
//------------------------------------------------------------------------------