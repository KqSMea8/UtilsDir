#ifndef MEMORY_CONTROL_DLL_DYNAMIC
#define MEMORY_CONTROL_DLL_DYNAMIC

#include "stdio.h"
#include "Windows.h"
#include <atlbase.h>
#include "memoryControlDll.h"

// ��������� �� ������������ � ������ ��������� �������
typedef long (MCD_GetHeapInfo)( MEMORY_CONTROL_HEAP_INFO_STRUCTURE* );
typedef MCD_GetHeapInfo*  pMCD_GetHeapInfo;

typedef long (MCD_GetStackInfo)( MEMORY_CONTROL_STACK_INFO_STRUCTURE* );
typedef MCD_GetStackInfo* pMCD_GetStackInfo;

typedef long (MCD_GetGeneralInfo)( MEMORY_CONTROL_GENERAL_INFO_STRUCTURE* );
typedef MCD_GetGeneralInfo* pMCD_GetGeneralInfo;

class MemoryControlDllDynamic
{	
private:
	HMODULE _hModuleDll;
public:
	MemoryControlDllDynamic();
	~MemoryControlDllDynamic();

// ��������� �� �������
public:
	pMCD_GetHeapInfo    _getHeapInfo;			// ������ ���������� � ������������ ������
	pMCD_GetStackInfo   _getStackInfo;			// ������ ���������� � �����������  ������
	pMCD_GetGeneralInfo _getGeneralInfo;		// ������ ����� ���������� � ���������
};

#endif	// MEMORY_CONTROL_DLL_DYNAMIC