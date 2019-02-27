//------------------------------------------------------------------------------
#pragma once
#include <ZetTools\include\ZetTools.h>
//------------------------------------------------------------------------------
/* —труктура данных, передаваема€ из Corr.exe в UNIT-2.
	—оздавать экземпл€ры этой структуры не следует!
	ѕервое поле - это об€зательно размер данных в структуре.
	size - это Ќ≈ sizeof(StructForUNIT2)!!!
	size = см. в TestSize		
*/ 
//------------------------------------------------------------------------------
#define SizeItemsInByte (sizeof(StructSpectrForUNIT2))
#define SizeOneArrayInByte(sizeArray) (sizeof(float) * sizeArray)
#define SizeAllArraysInByte(sizeArray, nArray) (SizeOneArrayInByte(sizeArray) * nArray)
//------------------------------------------------------------------------------
// ‘лаг bOldData используетс€ в SCADA-компоненте
struct StructSpectrForUNIT2
{
	DWORD size;				// размер всех данных этой структуры
	DWORD sizeOneArray;		// размер одного массива в отсчетах

	DWORD indxKit;			// индекс контейнера
	BYTE numArray;			// кол-во массивов, данные которых есть в структуре
	bool bOldData;			// данные ушли в интерфейс компонента или нет
	bool bGraf_Max;
	bool bGraf_Aver;

	CZetTime ztTime;		// врем€ данных

	StructSpectrForUNIT2();
	// «адание значени€ пол€ size структуры
	void SetSizeOfStruct();

	// “естирование полей структуры, св€занных с размерами.
	// ¬озвращаемое значение - успешность теста.
	bool TestSize(DWORD * pShift = nullptr);

	// “естирование структуры на предмет размера и получение указател€
	// на массив с индексом indxArray (счет с 0), содержащийс€ в структуре.
	// ¬озвращаемое значение - успешность теста.
	// ≈сли тест не успешный, то указатель приравниваетс€ nullptr.
	bool GetPointers(const int indxArray, float ** ppDataArray);
};
//------------------------------------------------------------------------------