#include "MemoryControlDll.h"
#include <windows.h>
#include <winternl.h>
#include <tlhelp32.h>
#include <boost/interprocess/windows_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>

MEMORY_CONTROL_API long MC_GetHeapInfo ( PMEMORY_CONTROL_HEAP_INFO_STRUCTURE heapInfo_ ) // Взятие диагностической информации о куче
{
	long   funcStatus     = MC_FUNC_CORRECT;
	DWORD  numHeapRegions = 0;
	DWORD  numHeapBlocks  = 0;
	long   totalHeapSize  = 0;
	HANDLE processHeaps[MAX_NUM_PROCESS_HEAPS];
	PROCESS_HEAP_ENTRY heapEntry;

	heapInfo_->_validData = MC_VALID_DATA;

	// 1. Определяем количество регионов динамической памяти
	numHeapRegions = GetProcessHeaps(0, NULL);
	if (numHeapRegions > MAX_NUM_PROCESS_HEAPS)
		numHeapRegions = MAX_NUM_PROCESS_HEAPS;
	numHeapRegions = GetProcessHeaps( numHeapRegions, processHeaps );

	// Проверяем корректность кучи
	for ( unsigned int i = 0; i < numHeapRegions; i++ )
		if ( HeapValidate(processHeaps[i], NULL, NULL) != TRUE )
		{
			heapInfo_->_validData = 0; //исправлена из-за исправления ошибки найденной 2014.04.15 pvs-studio
			funcStatus = MC_FUNC_INCORRECT;
			return funcStatus;
		}

	// 2. Подсчитываем общее количество блоков и общее количество выделенной памяти
	memset(&heapEntry, 0x00, sizeof(PROCESS_HEAP_ENTRY));
	heapEntry.wFlags |= (PROCESS_HEAP_REGION | PROCESS_HEAP_ENTRY_BUSY);
	for ( unsigned int i = 0; i < numHeapRegions; i++ )
	{
		funcStatus = 0;

		HeapLock(processHeaps[i]);
		{
			while ( HeapWalk(processHeaps[i], &heapEntry) )
			{
				totalHeapSize += heapEntry.cbData;
				numHeapBlocks ++;
			}
		}
		HeapUnlock(processHeaps[i]);

		if ( GetLastError() != ERROR_NO_MORE_ITEMS )
		{
			funcStatus = MC_FUNC_INCORRECT;
			heapInfo_->_validData = MC_INVALID_DATA;
			break;
		}

		memset(&heapEntry, 0x00, sizeof(PROCESS_HEAP_ENTRY));
		heapEntry.wFlags |= (PROCESS_HEAP_REGION | PROCESS_HEAP_ENTRY_BUSY);
	}

	// 3. Заполняем структуру полученной информацией
	heapInfo_->_numBlocksHeapMemory = numHeapBlocks;
	heapInfo_->_numRegionsHeapMemory = numHeapRegions;
	heapInfo_->_szBusyHeapMemory     = totalHeapSize;
	
	return funcStatus;
}


MEMORY_CONTROL_API long MC_GetStackInfo ( PMEMORY_CONTROL_STACK_INFO_STRUCTURE stackInfo_ ) // Взятие диагностической информации о стеке
{
	long funcStatus = MC_FUNC_CORRECT;

	stackInfo_->_szBusyStack = 0;
	stackInfo_->_validData   = 0;

	NtQueryInfoThread NtQueryInformationThread = (NtQueryInfoThread)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtQueryInformationThread");
	if ( NtQueryInformationThread == NULL )
		funcStatus = MC_FUNC_INCORRECT;
	else
	{
		HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
		DWORD processId = GetCurrentProcessId();

		if (h != INVALID_HANDLE_VALUE) 
		{
			THREADENTRY32 te;
			te.dwSize = sizeof(te);

			if (Thread32First(h, &te)) 
			{
				do {
					if (te.dwSize >= FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) + sizeof(te.th32OwnerProcessID)) 
					{
						if ( processId == te.th32OwnerProcessID )
						{
							myTHREAD_BASIC_INFORMATION basicInfo;

							HANDLE thrdHandle = OpenThread(THREAD_ALL_ACCESS, false, te.th32ThreadID);

							if ( thrdHandle != 0 )
							{
								printf("ProcessID: %u, threadID: %u\n", te.th32OwnerProcessID, te.th32ThreadID);

								NTSTATUS status = NtQueryInformationThread(thrdHandle, (THREADINFOCLASS)MY_DEFINE_THREAD_BASIC_INFORMATION, &basicInfo, sizeof(basicInfo), NULL);
								NT_TIB *tib = (NT_TIB*)basicInfo.TebBaseAddress;

								if ( tib != NULL )
									stackInfo_->_szBusyStack += ((long)tib->StackBase - (long)tib->StackLimit);

								CloseHandle(thrdHandle);
							}
						}
					}

					te.dwSize = sizeof(te);
				} while (Thread32Next(h, &te));
			}
			CloseHandle(h);
		}
	}

	for (unsigned int i = 0; i < LEN_HISTORY_BUSY_STACK_SIZE-1; i++)
		stackInfo_->_szBusyStackHistory[i] = stackInfo_->_szBusyStackHistory[i+1];
	stackInfo_->_szBusyStackHistory[LEN_HISTORY_BUSY_STACK_SIZE-1] = stackInfo_->_szBusyStack;

	return funcStatus;
}

MEMORY_CONTROL_API long MC_GetGeneralInfo( PMEMORY_CONTROL_GENERAL_INFO_STRUCTURE generalInfo_ )	// Взятие общей информации о программной памяти
{
	long funcStatus = MC_FUNC_CORRECT;

	HMODULE hBegin = GetModuleHandle(NULL);

	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)hBegin;
	PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((PBYTE)hBegin + dosHeader->e_lfanew);

	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);
	GlobalMemoryStatusEx(&status);

	if ( ( dosHeader != NULL ) && ( ntHeaders != NULL ) && ( status.ullAvailPhys != 0 ) && ( status.ullTotalPhys != 0 ) )
	{
		generalInfo_->_sizeOfHeapCommit   = ntHeaders->OptionalHeader.SizeOfHeapCommit;
		generalInfo_->_sizeOfHeapReserve  = ntHeaders->OptionalHeader.SizeOfHeapReserve;
		generalInfo_->_sizeOfStackCommit  = ntHeaders->OptionalHeader.SizeOfStackCommit;
		generalInfo_->_sizeOfStackReserve = ntHeaders->OptionalHeader.SizeOfStackReserve;

		generalInfo_->_sizeTotalPhysMemory = (long)status.ullTotalPhys;
		generalInfo_->_sizeAvailPhysMemory = (long)status.ullAvailPhys;
	}
	else
		funcStatus = MC_FUNC_INCORRECT;

	return funcStatus;
}

MEMORY_CONTROL_API long MC_TestCreatedSharedMemory( void )
{
	long funcStatus = MC_FUNC_CORRECT;

	try
	{
		boost::interprocess::windows_shared_memory _mcSharedMemory(boost::interprocess::open_only, MEMORY_CONTROL_SHARED_MEMORY_NAME, boost::interprocess::read_write);
		boost::interprocess::mapped_region         _mcMappedRegion(_mcSharedMemory, boost::interprocess::read_write);

		if ( _mcMappedRegion.get_address() == NULL )
			funcStatus = MC_FUNC_INCORRECT;
	}
	catch(...)
	{
		funcStatus = MC_FUNC_INCORRECT;
	}

	return funcStatus;
}

MEMORY_CONTROL_API long MC_ReserveCell( DWORD processId_ )
{
	long funcStatus = MC_FUNC_INCORRECT;

	try
	{
		boost::interprocess::windows_shared_memory _mcSharedMemory(boost::interprocess::open_only, MEMORY_CONTROL_SHARED_MEMORY_NAME, boost::interprocess::read_write);
		boost::interprocess::mapped_region         _mcMappedRegion(_mcSharedMemory, boost::interprocess::read_write);

		if ( _mcMappedRegion.get_address() != NULL )
		{
			PMEMORY_CONTROL_SHARED_STRUCTURE _mcSharedStructure = reinterpret_cast<PMEMORY_CONTROL_SHARED_STRUCTURE>(_mcMappedRegion.get_address());

			for ( unsigned int i = 0; i < MAX_NUM_PROCESS_IN_SHARED_MEMORY; i++ )
				if ( _mcSharedStructure->_processMemoryControl[i]._processId == processId_ )
				{
					funcStatus = MC_FUNC_CORRECT;

					break;
				}

			if ( funcStatus == MC_FUNC_INCORRECT )
			{
				for ( unsigned int i = 0; i < MAX_NUM_PROCESS_IN_SHARED_MEMORY; i++ )
				{
					if ( _mcSharedStructure->_processMemoryControl[i]._processId == 0 )
					{
						funcStatus = MC_FUNC_CORRECT;
						_mcSharedStructure->_processMemoryControl[i]._processId = processId_;

						break;
					}
				}
			}
		}
	}
	catch(...)
	{
		funcStatus = MC_FUNC_INCORRECT;
	}

	return funcStatus;
}

MEMORY_CONTROL_API long MC_FreeCell( DWORD processId_ )
{
	long funcStatus = MC_FUNC_INCORRECT;

	try
	{
		boost::interprocess::windows_shared_memory _mcSharedMemory(boost::interprocess::open_only, MEMORY_CONTROL_SHARED_MEMORY_NAME, boost::interprocess::read_write);
		boost::interprocess::mapped_region         _mcMappedRegion(_mcSharedMemory, boost::interprocess::read_write);

		if ( _mcMappedRegion.get_address() != NULL )
		{
			PMEMORY_CONTROL_SHARED_STRUCTURE _mcSharedStructure = reinterpret_cast<PMEMORY_CONTROL_SHARED_STRUCTURE>(_mcMappedRegion.get_address());

			for ( unsigned int i = 0; i < MAX_NUM_PROCESS_IN_SHARED_MEMORY; i++ )
			{
				if ( _mcSharedStructure->_processMemoryControl[i]._processId == processId_ )
				{
					funcStatus = MC_FUNC_CORRECT;
					_mcSharedStructure->_processMemoryControl[i]._processId = 0;

					break;
				}
			}
		}
	}
	catch(...)
	{
		funcStatus = MC_FUNC_INCORRECT;
	}

	return funcStatus;
}

MEMORY_CONTROL_API long MC_PutProcessMemoryInfo ( DWORD processId_, PMEMORY_CONTROL_GENERAL_INFO_STRUCTURE inGeneralInfo_, 
					PMEMORY_CONTROL_HEAP_INFO_STRUCTURE inHeapInfo_, PMEMORY_CONTROL_STACK_INFO_STRUCTURE inStackInfo_ )
{
	long funcStatus = MC_FUNC_INCORRECT;

	try
	{
		boost::interprocess::windows_shared_memory _mcSharedMemory(boost::interprocess::open_only, MEMORY_CONTROL_SHARED_MEMORY_NAME, boost::interprocess::read_write);
		boost::interprocess::mapped_region         _mcMappedRegion(_mcSharedMemory, boost::interprocess::read_write);

		if ( _mcMappedRegion.get_address() != NULL )
		{
			PMEMORY_CONTROL_SHARED_STRUCTURE _mcSharedStructure = reinterpret_cast<PMEMORY_CONTROL_SHARED_STRUCTURE>(_mcMappedRegion.get_address());

			for ( unsigned int i = 0; i < MAX_NUM_PROCESS_IN_SHARED_MEMORY; i++ )
			{
				if ( _mcSharedStructure->_processMemoryControl[i]._processId == processId_ )
				{
					funcStatus = MC_FUNC_CORRECT;

					memcpy_s(&_mcSharedStructure->_processMemoryControl[i]._generalInfo, sizeof(MEMORY_CONTROL_GENERAL_INFO_STRUCTURE), inGeneralInfo_, sizeof(MEMORY_CONTROL_GENERAL_INFO_STRUCTURE));
					memcpy_s(&_mcSharedStructure->_processMemoryControl[i]._heapInfo,    sizeof(MEMORY_CONTROL_HEAP_INFO_STRUCTURE),    inHeapInfo_,    sizeof(MEMORY_CONTROL_HEAP_INFO_STRUCTURE));
					memcpy_s(&_mcSharedStructure->_processMemoryControl[i]._stackInfo,   sizeof(MEMORY_CONTROL_STACK_INFO_STRUCTURE),   inStackInfo_,   sizeof(MEMORY_CONTROL_STACK_INFO_STRUCTURE));

					break;
				}
			}
		}
	}
	catch(...)
	{
		funcStatus = MC_FUNC_INCORRECT;
	}

	return funcStatus;
}

MEMORY_CONTROL_API long MC_GetProcessMemoryInfo ( DWORD processId_, PMEMORY_CONTROL_GENERAL_INFO_STRUCTURE outGeneralInfo_, 
					PMEMORY_CONTROL_HEAP_INFO_STRUCTURE outHeapInfo_, PMEMORY_CONTROL_STACK_INFO_STRUCTURE outStackInfo_ )
{
	long funcStatus = MC_FUNC_INCORRECT;

	memset(outGeneralInfo_, 0x00, sizeof(MEMORY_CONTROL_GENERAL_INFO_STRUCTURE));
	memset(outHeapInfo_,    0x00, sizeof(MEMORY_CONTROL_HEAP_INFO_STRUCTURE));
	memset(outStackInfo_,   0x00, sizeof(MEMORY_CONTROL_STACK_INFO_STRUCTURE));

	try
	{
		boost::interprocess::windows_shared_memory _mcSharedMemory(boost::interprocess::open_only, MEMORY_CONTROL_SHARED_MEMORY_NAME, boost::interprocess::read_write);
		boost::interprocess::mapped_region         _mcMappedRegion(_mcSharedMemory, boost::interprocess::read_write);

		if ( _mcMappedRegion.get_address() != NULL )
		{
			PMEMORY_CONTROL_SHARED_STRUCTURE _mcSharedStructure = reinterpret_cast<PMEMORY_CONTROL_SHARED_STRUCTURE>(_mcMappedRegion.get_address());

			for ( unsigned int i = 0; i < MAX_NUM_PROCESS_IN_SHARED_MEMORY; i++ )
			{
				if ( _mcSharedStructure->_processMemoryControl[i]._processId == processId_ )
				{
					funcStatus = MC_FUNC_CORRECT;

					memcpy_s(outGeneralInfo_,  sizeof(MEMORY_CONTROL_GENERAL_INFO_STRUCTURE), &_mcSharedStructure->_processMemoryControl[i]._generalInfo,  sizeof(MEMORY_CONTROL_GENERAL_INFO_STRUCTURE));
					memcpy_s(outHeapInfo_,     sizeof(MEMORY_CONTROL_HEAP_INFO_STRUCTURE),    &_mcSharedStructure->_processMemoryControl[i]._heapInfo,     sizeof(MEMORY_CONTROL_HEAP_INFO_STRUCTURE));
					memcpy_s(outStackInfo_,    sizeof(MEMORY_CONTROL_STACK_INFO_STRUCTURE),   &_mcSharedStructure->_processMemoryControl[i]._stackInfo,    sizeof(MEMORY_CONTROL_STACK_INFO_STRUCTURE));

					break;
				}
			}
		}
	}
	catch(...)
	{
		funcStatus = MC_FUNC_INCORRECT;
	}

	return funcStatus;
}