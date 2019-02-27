//--------------------------------------------------------------------------
//	Модуль CCirculBuffer.h
//	Версия 27.07.2011
//--------------------------------------------------------------------------
#pragma once
#include "afx.h"
//--------------------------------------------------------------------------
class CCirculBuffer
{
private:
	int FCurIndex;			// текущий индекс для записи
	int FCurIndexFiltr;		// текущий индекс для чтения с учётом фильтра
	int FDelta;				// приращение длинного индекса
	int FIndexStart0;		// начальное значение длинного индекса
	int FLengthFiltr;		// длина используемого фильтра
	int FSize;				// размер буфера в точках
	int sizeT_Buffer;		// размер буфера в байтах
	bool FBufferFull;		// буфер полон
	bool FFiltrEnable;		// буфер содержит точек > длины фильтра
	bool bCreateData;		// буфер создан внутри класса

	float* pGlobalAddres;		// адрес буфера
	float* pDataForFiltr;		// адрес массива данных для фильтрации
	long long int FIndexStart;	// индекс первой точки
	long long int FIndexEnd;	// индекс последней точки

protected:

public:
	CCirculBuffer();
	virtual ~CCirculBuffer();
	bool Create(int size, int delta, int indexStart,
				int lengthFiltr, float *addBuf = NULL);

	bool m_bDecimation;

	// функции свойств
	int GetCurIndex() { return FCurIndex; }
	bool GetBufferFull() { return FBufferFull; }

	bool GetFiltrEnable() { return FFiltrEnable; }

	int GetSize() { return FSize; }

	// функции класса
	float GetPointFromGlobalIndex(long long int index);
	float* GetDataForFiltr();
	void WriteOnce(float data);
	void NewFiltr(int newLengthFiltr);
	void Clear(bool bInitData = false, int delta = 0, int indexStart = 0);
};
//--------------------------------------------------------------------------