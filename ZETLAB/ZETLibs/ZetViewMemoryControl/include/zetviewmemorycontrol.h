// Данная библиотека необходима для хранения отладочной информации по состоянию компонент.
// 07.11.2016 - хранит количество выделенных блоков и общий обьем выделенной памяти.

#ifndef ZET_VIEW_MEMORY_CONTROL
#define ZET_VIEW_MEMORY_CONTROL

#define ZVMC_NOT_FIND_CELL_ERROR       -1				// Код ошибки в случе не нахождения необходимой ячейки в отладочной информации по компонентам
#define ZVMC_MAX_COMPONENT_NAME_LENGTH 100				// Максимальная длина наименования компонента
#define ZVMC_MAX_COMPONENT_NUMBER      500				// Максимальное количество разных типов компонентов

#ifdef ZETVIEWMEMORYCONTROL_EXPORTS
	#define ZETVIEWMEMORYCONTROL_API __declspec(dllexport)
#else
	#define ZETVIEWMEMORYCONTROL_API __declspec(dllimport)
#endif


// Поиск текущей ячейки для хранения данных по компоненту
// Входные значения: componentName_  - наименование компонента
// Выходные значения: номер зарегестрированной ячейки для данных от этого компонента
#ifdef __cplusplus
extern "C" {
#endif
ZETVIEWMEMORYCONTROL_API long zvmcFindComponentCell(const wchar_t* componentName_);

// Инициализация ячейки для хранения данных по компоненту
// Данная функция потоконезащищенная, так как подразумевает свой вызов из проекта ZetView в функции добавления нового компонента
// Входные значения: componentName_  - наименование компонента
// Выходные значения: номер зарегестрированной ячейки для данных от этого компонента
ZETVIEWMEMORYCONTROL_API long zvmcInitComponentCell(const wchar_t* componentName_);

// Инкрементирование количества выделенных блоков
// Входные значения: componentCellNumber_ - номер зарегестрированной ячейки для данных от этого компонента
ZETVIEWMEMORYCONTROL_API void zvmcIncComponentAllocatedHeapMemory(long componentCellNumber_, size_t size_);

// Декрементирование количества выделенных блоков
// Входные значения: componentCellNumber_ - номер зарегестрированной ячейки для данных от этого компонента
ZETVIEWMEMORYCONTROL_API void zvmcDecComponentAllocatedHeapMemory(long componentCellNumber_, size_t size_);

// Освобождение ячейки для хранения данных по компоненту
// Входные значения: componentCellNumber_ - номер зарегестрированной ячейки для данных от этого компонента
ZETVIEWMEMORYCONTROL_API void zvmcFreeComponentCell(const wchar_t* componentName_);

// Взятие информации о количестве выделенных блоков памяти по компоненте
// Входные значения: componentName_ - имя компонента
// Выходные значения: количество выделенных блоков памяти
ZETVIEWMEMORYCONTROL_API unsigned long zvmcGetNumberAllocatedHeapMemoryBlocks(const wchar_t* componentName_);

// Взятие информации о количестве выделенной памяти по компоненте
// Входные значения: componentName_ - имя компонента
// Выходные значения: количество выделенной памяти
ZETVIEWMEMORYCONTROL_API unsigned long zvmcGetNumberAllocatedHeapMemorySize(const wchar_t* componentName_);
#ifdef __cplusplus
}
#endif

#endif