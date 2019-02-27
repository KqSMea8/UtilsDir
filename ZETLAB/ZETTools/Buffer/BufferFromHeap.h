//------------------------------------------------------------------------------
#pragma once
#include "afxwin.h"
//------------------------------------------------------------------------------
class CBufferFromHeap
{
private:
	HANDLE m_hHeap;
	PVOID m_pAdd;
	bool m_bInternalHeap;
	SIZE_T m_size;

public:
	CBufferFromHeap(void);
	virtual ~CBufferFromHeap(void);

	bool Init(HANDLE hHeap = 0);
	DWORD GetSize() { return (DWORD)m_size; }
	bool SetSize(DWORD size);
	PVOID GetPoiterData() { return m_pAdd; }

	bool ReadFromBuffer(PVOID add, DWORD size);
	bool WriteIntoBuffer(PVOID add, DWORD size);
};
//------------------------------------------------------------------------------