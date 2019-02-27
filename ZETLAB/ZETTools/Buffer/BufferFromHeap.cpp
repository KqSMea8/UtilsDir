//------------------------------------------------------------------------------
#include "StdAfx.h"
#include "Buffer\BufferFromHeap.h"
//------------------------------------------------------------------------------
CBufferFromHeap::CBufferFromHeap(void)
{
	m_hHeap = 0;
	m_pAdd = NULL;
}
//------------------------------------------------------------------------------
CBufferFromHeap::~CBufferFromHeap(void)
{
	if(m_bInternalHeap)
		HeapDestroy(m_hHeap);
	else
		HeapFree(m_hHeap, 0, m_pAdd);
}
//------------------------------------------------------------------------------
bool CBufferFromHeap::Init(HANDLE hHeap)
{// —оздание хипа (если он внутренний). ¬ыделение первоначального блока с
// обнулением и определение размера
	m_size = 8;	// первоначальный размер = sizeof(double)
	if (hHeap)
	// внешний хип
		m_bInternalHeap = false;
	else
	{// внутренний хип
		m_bInternalHeap = true;
		m_hHeap = HeapCreate(0, m_size, 0);
	}

	bool ret = m_hHeap != 0;
	if (ret)
	{
		m_pAdd = HeapAlloc(m_hHeap, HEAP_ZERO_MEMORY, m_size);
		ret = m_pAdd != NULL;
		if (ret)
			m_size = HeapSize(m_hHeap, 0, m_pAdd);
		else
			m_size = 0;
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CBufferFromHeap::SetSize(DWORD size)
{// «адание нового размера. ≈сли размер уже больше, то ничего не делаетс€
	bool ret = size <= (DWORD)m_size;
	if (!ret)
	{
		m_pAdd = HeapReAlloc(m_hHeap, HEAP_ZERO_MEMORY, m_pAdd, (SIZE_T)size);
		ret = m_pAdd != NULL;
		if (ret)
		{
			m_size = HeapSize(m_hHeap, 0, m_pAdd);
			ret = (DWORD)m_size >= size;
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CBufferFromHeap::ReadFromBuffer(PVOID add, DWORD size)
{// „тение данных из хипа
	return (add != NULL) && ( (DWORD)m_size >= size ) &&
			(memcpy_s(add, size, m_pAdd, size) == 0);
}
//------------------------------------------------------------------------------
bool CBufferFromHeap::WriteIntoBuffer(PVOID add, DWORD size)
{// запись данных в хип
	return (add != NULL) && SetSize(size) &&
			(memcpy_s(m_pAdd, size, add, size) == 0);
}
//------------------------------------------------------------------------------