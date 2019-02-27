#include "memoryControlDllDynamic.h"

MemoryControlDllDynamic::MemoryControlDllDynamic()
{
	HMODULE _hModuleDll = LoadLibrary(_T("MemoryControlDll.dll"));

	if ( _hModuleDll != NULL )
	{
		_getHeapInfo   = reinterpret_cast< pMCD_GetHeapInfo >  (GetProcAddress(_hModuleDll, "MC_GetHeapInfo"));
		_getStackInfo  = reinterpret_cast< pMCD_GetStackInfo >  (GetProcAddress(_hModuleDll, "MC_GetStackInfo"));
		_getGeneralInfo  = reinterpret_cast< pMCD_GetGeneralInfo >  (GetProcAddress(_hModuleDll, "MC_GetGeneralInfo"));

		if ( ( _getHeapInfo == NULL ) || ( _getStackInfo == NULL ) )
		{
			FreeLibrary(_hModuleDll);

			_hModuleDll = NULL;
			_getHeapInfo = NULL;
			_getStackInfo = NULL;
		}
	}
}

MemoryControlDllDynamic::~MemoryControlDllDynamic()
{
	if ( _hModuleDll != NULL )
		FreeLibrary( _hModuleDll );
}