#ifndef MEMORY_CONTROL_DLL
#define MEMORY_CONTROL_DLL

#include <windows.h>
#include <winternl.h>

#ifdef MEMORYCONTROLDLL_EXPORTS
#define MEMORY_CONTROL_API __declspec(dllexport)
#else
#define MEMORY_CONTROL_API __declspec(dllimport)
#pragma comment(lib, "ZetMemoryControl\\bin\\Win32\\MemoryControlDll.lib")
#endif

#define MC_FUNC_CORRECT    0
#define MC_FUNC_INCORRECT -1
#define MC_VALID_DATA      0
#define MC_INVALID_DATA   -1

#define MEMORY_CONTROL_SHARED_MEMORY_NAME "ZetMemoryControl"

// ===== ===== ===== ===== ===== ===== ������ ����� ���������� � ���������� ��������� ===== ========== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
struct MEMORY_CONTROL_GENERAL_INFO_STRUCTURE
{
	long _sizeOfStackReserve;	  // ������������ ������ ����������������� ������ ��� ����	
	long _sizeOfStackCommit;	  // ����� ���������� ������ �����
	long _sizeOfHeapReserve;	  // ������������ ������ ����
	long _sizeOfHeapCommit;		  // ����� ���������� ������ ����

	long _sizeTotalPhysMemory;	  // ������ ������������� ����������� ������
	long _sizeAvailPhysMemory;	  // ������ ��������� �� ������ ������ �����������

	char _reserve[100];			  // ������
};
typedef MEMORY_CONTROL_GENERAL_INFO_STRUCTURE* PMEMORY_CONTROL_GENERAL_INFO_STRUCTURE;

#ifdef __cplusplus
extern "C" {
#endif
	MEMORY_CONTROL_API long MC_GetGeneralInfo ( PMEMORY_CONTROL_GENERAL_INFO_STRUCTURE outGeneralInfo_ ); // ������ ���������� � ���������� ���������
#ifdef __cplusplus
}
#endif
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== 

// ===== ===== ===== ===== ===== ===== �������������� ������������ ������� ������ ===== ========== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
#define MAX_NUM_PROCESS_HEAPS     1000
struct MEMORY_CONTROL_HEAP_INFO_STRUCTURE
{
	long _szBusyHeapMemory;		  // ������ ������� ������������ ������ � ������
	long _numRegionsHeapMemory;	  // ���������� ������ ������������ ������
	long _numBlocksHeapMemory;    // ����� ���������� ����������� ����� 
	long _validData;			  // ������ �� ����

	char _reserve[100];			  // ������
};
typedef MEMORY_CONTROL_HEAP_INFO_STRUCTURE* PMEMORY_CONTROL_HEAP_INFO_STRUCTURE;

#ifdef __cplusplus
extern "C" {
#endif
	MEMORY_CONTROL_API long MC_GetHeapInfo ( PMEMORY_CONTROL_HEAP_INFO_STRUCTURE outHeapInfo_ ); // ������ ���������� � ����
#ifdef __cplusplus
}
#endif
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== 

// ===== ===== ===== ===== ===== ===== �������������� �����������, ������� ������ ===== ========== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
#define LEN_HISTORY_BUSY_STACK_SIZE           32
#define MY_DEFINE_THREAD_BASIC_INFORMATION    0
// +++++++ ����������� ��� ����������� ������������� ����� +++++++
typedef ULONG myKPRIORITY;

typedef struct _myCLIENT_ID {
	HANDLE UniqueProcess;
	HANDLE UniqueThread;
} myCLIENT_ID, *myPCLIENT_ID;

typedef struct _myTHREAD_BASIC_INFORMATION 
{
	NTSTATUS    ExitStatus; 
	PVOID       TebBaseAddress; 
	myCLIENT_ID ClientId; 
	KAFFINITY   AffinityMask; 
	myKPRIORITY Priority; 
	myKPRIORITY BasePriority;
} myTHREAD_BASIC_INFORMATION, *myPTHREAD_BASIC_INFORMATION;

typedef NTSTATUS (WINAPI *NtQueryInfoThread)(HANDLE, THREADINFOCLASS, PVOID, ULONG, PULONG);

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


struct MEMORY_CONTROL_STACK_INFO_STRUCTURE
{
	long  _szBusyStack;											// ������ ����� ������
	long  _szBusyStackHistory[LEN_HISTORY_BUSY_STACK_SIZE];		// ������� ������� ����� ������
	long  _validData;											// ������ �� �����

	char _reserve[100];											// ������
};
typedef MEMORY_CONTROL_STACK_INFO_STRUCTURE* PMEMORY_CONTROL_STACK_INFO_STRUCTURE;

#ifdef __cplusplus
extern "C" {
#endif
	MEMORY_CONTROL_API long MC_GetStackInfo        ( PMEMORY_CONTROL_STACK_INFO_STRUCTURE outStackInfo_ );	// ������ ������ ������� ����������� �����
#ifdef __cplusplus
}
#endif
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====




// ===== ===== ===== ===== ===== ===== ������ � ����� �������� ������ ���������� MC ===== ========== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
#define MAX_NUM_PROCESS_IN_SHARED_MEMORY 100

struct MEMORY_CONTROL_PROCESS_SHARED_STRUCTURE
{
	DWORD                                 _processId;		// ������������� ��������������� ��������
	MEMORY_CONTROL_GENERAL_INFO_STRUCTURE _generalInfo;		// ����� ���������� � ���������
	MEMORY_CONTROL_HEAP_INFO_STRUCTURE    _heapInfo;		// ���������� � ����
	MEMORY_CONTROL_STACK_INFO_STRUCTURE   _stackInfo;		// ���������� � �����

	char _reserve[100];					// ������
};

struct MEMORY_CONTROL_SHARED_STRUCTURE
{
	long _maxNumProcessSharedMemoryControl;
	MEMORY_CONTROL_PROCESS_SHARED_STRUCTURE _processMemoryControl[MAX_NUM_PROCESS_IN_SHARED_MEMORY];	// ���������� � ��������� ������ ��� ���������� ���������
};
typedef MEMORY_CONTROL_PROCESS_SHARED_STRUCTURE* PMEMORY_CONTROL_PROCESS_SHARED_STRUCTURE;
typedef MEMORY_CONTROL_SHARED_STRUCTURE* PMEMORY_CONTROL_SHARED_STRUCTURE;

#ifdef __cplusplus
extern "C" {
#endif

	MEMORY_CONTROL_API long MC_TestCreatedSharedMemory( void );    // �������� ����������� ����� ������

	MEMORY_CONTROL_API long MC_ReserveCell        (DWORD processId_ );		 // ������ ������ � ������ MemoryControlSharedMemory
		
	MEMORY_CONTROL_API long MC_FreeCell           (DWORD processId_ );		 // ���������� ������ � ������ MemoryControlShredMemory

	MEMORY_CONTROL_API long MC_PutProcessMemoryInfo ( DWORD                               processId_,		// �������� ���������� � �������� � ����������� ������                           
												   PMEMORY_CONTROL_GENERAL_INFO_STRUCTURE inGeneralInfo_,
												   PMEMORY_CONTROL_HEAP_INFO_STRUCTURE    inHeapInfo_,
												   PMEMORY_CONTROL_STACK_INFO_STRUCTURE   inStackInfo_ );

	MEMORY_CONTROL_API long MC_GetProcessMemoryInfo ( DWORD                               processId_,		// ����� ���������� � �������� �� ����������� ������                            
												   PMEMORY_CONTROL_GENERAL_INFO_STRUCTURE outGeneralInfo_,
												   PMEMORY_CONTROL_HEAP_INFO_STRUCTURE    outHeapInfo_,
												   PMEMORY_CONTROL_STACK_INFO_STRUCTURE   outStackInfo_ );
	
#ifdef __cplusplus
}
#endif
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====

#endif	// MEMORY_CONTROL_DLL