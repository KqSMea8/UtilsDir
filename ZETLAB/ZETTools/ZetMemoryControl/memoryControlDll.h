#ifndef MEMORY_CONTROL_DLL
#define MEMORY_CONTROL_DLL

#include <windows.h>
#include <winternl.h>

#ifdef MEMORYCONTROLDLL_EXPORTS
#define MEMORY_CONTROL_API __declspec(dllexport)
#else
#define MEMORY_CONTROL_API __declspec(dllimport)
//#pragma comment(lib, "C:\\ZETTools\\ZetMemoryControl\\MemoryControlDll.lib")
#pragma comment(lib, "ZetMemoryControl\\bin\\Win32\\MemoryControlDll.lib")
#endif

#define MC_FUNC_CORRECT    0
#define MC_FUNC_INCORRECT -1
#define MC_VALID_DATA      0
#define MC_INVALID_DATA   -1

#define MEMORY_CONTROL_SHARED_MEMORY_NAME "ZetMemoryControl"

// ===== ===== ===== ===== ===== ===== Взятие общей информации о запущенной программе ===== ========== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
struct MEMORY_CONTROL_GENERAL_INFO_STRUCTURE
{
	long _sizeOfStackReserve;	  // Максимальный размер зарезервированной памяти под стек	
	long _sizeOfStackCommit;	  // Сразу выделяемый размер стека
	long _sizeOfHeapReserve;	  // Максимальный размер кучи
	long _sizeOfHeapCommit;		  // Сразу выделяемый размер кучи

	long _sizeTotalPhysMemory;	  // Размер установленной оперативной памяти
	long _sizeAvailPhysMemory;	  // Размер доступной на данный момент оперативной

	char _reserve[100];			  // Резерв
};
typedef MEMORY_CONTROL_GENERAL_INFO_STRUCTURE* PMEMORY_CONTROL_GENERAL_INFO_STRUCTURE;

#ifdef __cplusplus
extern "C" {
#endif
	MEMORY_CONTROL_API long MC_GetGeneralInfo ( PMEMORY_CONTROL_GENERAL_INFO_STRUCTURE outGeneralInfo_ ); // Взятие информации о запущенной программе
#ifdef __cplusplus
}
#endif
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== 

// ===== ===== ===== ===== ===== ===== ПРОФИЛИРОВАНИЕ ДИНАМИЧЕСКОЙ ОБЛАСТИ ПАМЯТИ ===== ========== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
#define MAX_NUM_PROCESS_HEAPS     1000
struct MEMORY_CONTROL_HEAP_INFO_STRUCTURE
{
	long _szBusyHeapMemory;		  // Размер занятой динамической памяти в байтах
	long _numRegionsHeapMemory;	  // Количество блоков динамической памяти
	long _numBlocksHeapMemory;    // Номер последнего выделенного блока 
	long _validData;			  // Ошибки по куче

	char _reserve[100];			  // Резерв
};
typedef MEMORY_CONTROL_HEAP_INFO_STRUCTURE* PMEMORY_CONTROL_HEAP_INFO_STRUCTURE;

#ifdef __cplusplus
extern "C" {
#endif
	MEMORY_CONTROL_API long MC_GetHeapInfo ( PMEMORY_CONTROL_HEAP_INFO_STRUCTURE outHeapInfo_ ); // Взятие информации о куче
#ifdef __cplusplus
}
#endif
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== 

// ===== ===== ===== ===== ===== ===== ПРОФИЛИРОВАНИЕ СТАТИЧЕСКОЙ, ОБЛАСТИ ПАМЯТИ ===== ========== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
#define LEN_HISTORY_BUSY_STACK_SIZE           32
#define MY_DEFINE_THREAD_BASIC_INFORMATION    0
// +++++++ Определения для определения характеристик стека +++++++
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
	long  _szBusyStack;											// Размер стека потока
	long  _szBusyStackHistory[LEN_HISTORY_BUSY_STACK_SIZE];		// История размера стека потока
	long  _validData;											// Ошибки по стеку

	char _reserve[100];											// Резерв
};
typedef MEMORY_CONTROL_STACK_INFO_STRUCTURE* PMEMORY_CONTROL_STACK_INFO_STRUCTURE;

#ifdef __cplusplus
extern "C" {
#endif
	MEMORY_CONTROL_API long MC_GetStackInfo        ( PMEMORY_CONTROL_STACK_INFO_STRUCTURE outStackInfo_ );	// Взятие общего размера выделенного стека
#ifdef __cplusplus
}
#endif
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====




// ===== ===== ===== ===== ===== ===== РАБОТА С ОБЩЕЙ ОБЛАСТЬЮ ПАМЯТИ БИБЛИОТЕКИ MC ===== ========== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
#define MAX_NUM_PROCESS_IN_SHARED_MEMORY 100

struct MEMORY_CONTROL_PROCESS_SHARED_STRUCTURE
{
	DWORD                                 _processId;		// Идентификатор контролируемого процесса
	MEMORY_CONTROL_GENERAL_INFO_STRUCTURE _generalInfo;		// Общая информация о программе
	MEMORY_CONTROL_HEAP_INFO_STRUCTURE    _heapInfo;		// Информация о куче
	MEMORY_CONTROL_STACK_INFO_STRUCTURE   _stackInfo;		// Информация о стеке

	char _reserve[100];					// Резерв
};

struct MEMORY_CONTROL_SHARED_STRUCTURE
{
	long _maxNumProcessSharedMemoryControl;
	MEMORY_CONTROL_PROCESS_SHARED_STRUCTURE _processMemoryControl[MAX_NUM_PROCESS_IN_SHARED_MEMORY];	// Информация о состоянии памяти для запущенных процессов
};
typedef MEMORY_CONTROL_PROCESS_SHARED_STRUCTURE* PMEMORY_CONTROL_PROCESS_SHARED_STRUCTURE;
typedef MEMORY_CONTROL_SHARED_STRUCTURE* PMEMORY_CONTROL_SHARED_STRUCTURE;

#ifdef __cplusplus
extern "C" {
#endif

	MEMORY_CONTROL_API long MC_TestCreatedSharedMemory( void );    // Проверка созданности общей памяти

	MEMORY_CONTROL_API long MC_ReserveCell        (DWORD processId_ );		 // Занять ячейку в памяти MemoryControlSharedMemory
		
	MEMORY_CONTROL_API long MC_FreeCell           (DWORD processId_ );		 // Освободить ячейку в памяти MemoryControlShredMemory

	MEMORY_CONTROL_API long MC_PutProcessMemoryInfo ( DWORD                               processId_,		// Засунуть информацию о процессе в разделяемую память                           
												   PMEMORY_CONTROL_GENERAL_INFO_STRUCTURE inGeneralInfo_,
												   PMEMORY_CONTROL_HEAP_INFO_STRUCTURE    inHeapInfo_,
												   PMEMORY_CONTROL_STACK_INFO_STRUCTURE   inStackInfo_ );

	MEMORY_CONTROL_API long MC_GetProcessMemoryInfo ( DWORD                               processId_,		// Взять информацию о просессе из разделяемой памяти                            
												   PMEMORY_CONTROL_GENERAL_INFO_STRUCTURE outGeneralInfo_,
												   PMEMORY_CONTROL_HEAP_INFO_STRUCTURE    outHeapInfo_,
												   PMEMORY_CONTROL_STACK_INFO_STRUCTURE   outStackInfo_ );
	
#ifdef __cplusplus
}
#endif
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====

#endif	// MEMORY_CONTROL_DLL