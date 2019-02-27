//--------------------------------------------------------------------------
//	Модуль ZetWriteDTU.h
//	Версия от 15.11.2011
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//--------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include "ATLComTime.h"
#include <WriterDTU\WriterDTU.h>
#include <ZetThread\CustomZetThread.h>
#include <Buffer\ShiftBufferDATE.h>
//--------------------------------------------------------------------------
struct ParamTotal
{// структура общих параметров, хранится в контейнере потоков
	HWND hParent;
	CShiftBufferDATE *pBufDate;		// указатель на буфер дат
};
//--------------------------------------------------------------------------
struct ParamPrivat
{// структура частных параметров, хранится в каждом потоке (своя)
	CWriterDTU *pWriter;
//	int NumberChannels;		// кол-во каналов (столбцов с данными, не более 3)
//	CString pCommentary[3];	// массив имен каналов
//	CString pConversion[3];	// массив размерностей каналов
	float  *ppData[3];		// массив указателей на данные

//	float FminFiltr;		// нижняя частота фильтрации сигналов
//	float FmaxFiltr;		// верхняя частота фильтрации сигналов
//	float FreqADC;			// частота дискретизации АЦП в Гц
//	float Tadc;				// период дискретизации АЦП в сек

//	int NumberFile;			// номер файла DTU
//	CString PathFile;		// путь файла DTU
//	CString NameFile;		// полное имя файла DTU (обязательно, иначе нельзя
							// будет передавать его в сообщении)
	DATE T0;				// время начала события
	DATE T1;				// время конца события
};
//--------------------------------------------------------------------------
struct ZetThreadParamDTU	// параметры потока
{// адрес этой структуры будет передан как параметр глобальной функции потока
	struct ParamTotal  *pTotal;
	struct ParamPrivat *pPrivat;
};
//-------------------------------------------------------------------------
class CThreadDTU : public CCustomZetThread
{
private:
	ZetThreadParamDTU zParam1;

protected:
public:
	CThreadDTU();
	virtual ~CThreadDTU(void);
	virtual bool Create( CString nameThread, HWND hwnd, 
							ParamTotal *parTot );

	ParamPrivat m_ParamPrivat;
	CWriterDTU m_WriterDTU;
	// методы чтения/записи свойств -----------------	
	// методы класса --------------------------------
};
//--------------------------------------------------------------------------