//------------------------------------------------------------------------------
//	������ CThreadWait.h
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
//   ����� ������� ������.
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ZetThread\CustomZetThread.h>
//------------------------------------------------------------------------------
struct ParamThreadWait
{
	HANDLE pHandle[2];
};
//------------------------------------------------------------------------------
class CThreadWait : public CCustomZetThread
{
private:
	bool m_bObjectInternal;
	ParamThreadWait m_ptw;

protected:

public:
	CThreadWait();
	virtual bool Create( CString nameThread, HWND hwnd,
		FunctionThread function, PVOID paramFunction = NULL);
	virtual ~CThreadWait();

	// ����� ������
	// ������ �������
	// ������ ������

	virtual long End();				// ����������� ������� ������ "�����"
	virtual long EndAndWait();		// ���������� ������� ������ "�����"
	virtual long Stop();			// ����������� ������� ������ "����"
	virtual long StopAndWait();		// ���������� ������� ������ "����"

	// ����������� ������
	void SetSynchro();

	// ������� �������� ����������������� ������� ����,
	// ������� ������ ���� � �����������
	void SetExternalSynchroObject(HANDLE handle);
};
//------------------------------------------------------------------------------