// ������ ���������� ���������� ��� �������� ���������� ���������� �� ��������� ���������.
// 07.11.2016 - ������ ���������� ���������� ������ � ����� ����� ���������� ������.

#ifndef ZET_VIEW_MEMORY_CONTROL
#define ZET_VIEW_MEMORY_CONTROL

#define ZVMC_NOT_FIND_CELL_ERROR       -1				// ��� ������ � ����� �� ���������� ����������� ������ � ���������� ���������� �� �����������
#define ZVMC_MAX_COMPONENT_NAME_LENGTH 100				// ������������ ����� ������������ ����������
#define ZVMC_MAX_COMPONENT_NUMBER      500				// ������������ ���������� ������ ����� �����������

#ifdef ZETVIEWMEMORYCONTROL_EXPORTS
	#define ZETVIEWMEMORYCONTROL_API __declspec(dllexport)
#else
	#define ZETVIEWMEMORYCONTROL_API __declspec(dllimport)
#endif


// ����� ������� ������ ��� �������� ������ �� ����������
// ������� ��������: componentName_  - ������������ ����������
// �������� ��������: ����� ������������������ ������ ��� ������ �� ����� ����������
#ifdef __cplusplus
extern "C" {
#endif
ZETVIEWMEMORYCONTROL_API long zvmcFindComponentCell(const wchar_t* componentName_);

// ������������� ������ ��� �������� ������ �� ����������
// ������ ������� ������������������, ��� ��� ������������� ���� ����� �� ������� ZetView � ������� ���������� ������ ����������
// ������� ��������: componentName_  - ������������ ����������
// �������� ��������: ����� ������������������ ������ ��� ������ �� ����� ����������
ZETVIEWMEMORYCONTROL_API long zvmcInitComponentCell(const wchar_t* componentName_);

// ����������������� ���������� ���������� ������
// ������� ��������: componentCellNumber_ - ����� ������������������ ������ ��� ������ �� ����� ����������
ZETVIEWMEMORYCONTROL_API void zvmcIncComponentAllocatedHeapMemory(long componentCellNumber_, size_t size_);

// ����������������� ���������� ���������� ������
// ������� ��������: componentCellNumber_ - ����� ������������������ ������ ��� ������ �� ����� ����������
ZETVIEWMEMORYCONTROL_API void zvmcDecComponentAllocatedHeapMemory(long componentCellNumber_, size_t size_);

// ������������ ������ ��� �������� ������ �� ����������
// ������� ��������: componentCellNumber_ - ����� ������������������ ������ ��� ������ �� ����� ����������
ZETVIEWMEMORYCONTROL_API void zvmcFreeComponentCell(const wchar_t* componentName_);

// ������ ���������� � ���������� ���������� ������ ������ �� ����������
// ������� ��������: componentName_ - ��� ����������
// �������� ��������: ���������� ���������� ������ ������
ZETVIEWMEMORYCONTROL_API unsigned long zvmcGetNumberAllocatedHeapMemoryBlocks(const wchar_t* componentName_);

// ������ ���������� � ���������� ���������� ������ �� ����������
// ������� ��������: componentName_ - ��� ����������
// �������� ��������: ���������� ���������� ������
ZETVIEWMEMORYCONTROL_API unsigned long zvmcGetNumberAllocatedHeapMemorySize(const wchar_t* componentName_);
#ifdef __cplusplus
}
#endif

#endif