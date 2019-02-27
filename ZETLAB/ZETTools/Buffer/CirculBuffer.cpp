//------------------------------------------------------------------------------
//	Модуль CCirculBuffer.cpp
//	Версия 27.07.2011
//------------------------------------------------------------------------------
//	Класс предназначен для создания циклических буферов, ориентированных
// на работу с цифровыми фильтрами. О фильтре буфер знает только его длину.
// Буфер может быть внешним или внутренним, т.е. создаваемым внутри класса.
//	Каждой точке в буфере соответствует значение длинного индекса (для
// временной синхронизации данных).
//	Чтение данных из буфера:
// 1. Для отображения - по значению длинного индекса, который является
//	  номером (счёт с 0) отсчёта в исходном сигнале (т.е. как бы по времени).
// 2. Для фильтрации - массив последних записанных точек. Размерность массива
//	  равна длине используемого фильтра.
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "CirculBuffer.h"
//------------------------------------------------------------------------------
CCirculBuffer::CCirculBuffer()
{
	bCreateData = false;
	pGlobalAddres = NULL;
	pDataForFiltr = NULL;
}
//------------------------------------------------------------------------------
CCirculBuffer::~CCirculBuffer()
{
	if (bCreateData)
	{
		delete [] pGlobalAddres;
		pGlobalAddres = nullptr;
	}
	if (pDataForFiltr)
	{
		delete [] pDataForFiltr;
		pDataForFiltr = nullptr;
	}
}
//------------------------------------------------------------------------------
bool CCirculBuffer::Create(
	int size,			// размер буфера в отсчётах
	int delta,			// приращение длинного индекса
	int indexStart,		// начальное значение длинного индекса
	int lengthFiltr,	// длина фильтра
	float *addBuf)		// NULL если буфер надо создавать внутри класса
{
	bool ret = true;
	FSize = size;
	sizeT_Buffer = sizeFloat * FSize;

	// работа с самим буфером
	if (addBuf)
	{
		bCreateData = false;
		pGlobalAddres = addBuf;
	}
	else
	{
		bCreateData = true;
		pGlobalAddres = new float [FSize];
		if (pGlobalAddres)
			Clear(true, delta, indexStart);
		else
			ret = false;
	}
	// создаю маленький буфер, размером в длину фильтра
	if (ret)
	{
		FLengthFiltr = lengthFiltr;
		pDataForFiltr = new float [FLengthFiltr];
		if (!pDataForFiltr)
			ret = false;
	}
	return ret;
}
//------------------------------------------------------------------------------
float CCirculBuffer::GetPointFromGlobalIndex(long long int index)
{// чтение одной точки из буфера по значению длинного индекса
	float ret;
	if ( ( FIndexStart <= index ) && ( index < FIndexEnd ) )
	{
		if ( (FDelta) && (FSize) )
		{
		long long int iCur = (index - FIndexStart0) / FDelta;
		int n = iCur % FSize;
		ret = pGlobalAddres[n];
		}
		else
			ret = 0.f;
	}
	else
		ret = 0.f;
//	if (FDelta==8)
//		int nn = 17;
	return ret;
}
//------------------------------------------------------------------------------
float* CCirculBuffer::GetDataForFiltr()
{//В маленький буфер помещаю данные из большого для фильтрации текущих данных.
	// Текущие данные - это последние n точек, записанные в большой буфер, где
	// n = FLengthFiltr
	for(int i=0; i<FLengthFiltr; i++)
	{
		int n = FCurIndexFiltr + i;
		if ( n >= FSize )
			n -= FSize;
		pDataForFiltr[i] = pGlobalAddres[n];
	}
	return pDataForFiltr;
}
//------------------------------------------------------------------------------
void CCirculBuffer::WriteOnce(float data)
{//	Добавление одной точки в буфер
	//if (FDelta==16)
	//	int nn = 17;
	if (FBufferFull)			// если буфер полон, то первый точки
		FIndexStart += FDelta;	// начинают выпадать
	FIndexEnd += FDelta;		// это всегда
	pGlobalAddres[FCurIndex++] = data;
	if ( FCurIndex == FSize )
	{
		FBufferFull = true;
		FCurIndex = 0;
	}

	// индекс для данных фильтра ---------------------------------
	if (FFiltrEnable)
	{// точек в буфере больше, чем длина фильтра
		FCurIndexFiltr++;
		if ( FCurIndexFiltr == FSize )
			FCurIndexFiltr = 0;
	}
	else
	{// Точек мало, проверяю сколько. Длина буфера больше FLengthFiltr
		if ( FCurIndex == FLengthFiltr )	// именно ==, т.к. FCurIndex++
			FFiltrEnable = true;			//			уже было!
	}
}
//------------------------------------------------------------------------------
void CCirculBuffer::Clear(bool bInitData, int delta, int indexStart)
{// Инициализация буфера. Первоначальное задание переменных
	// стартовыми значениями и обнуление данных в буфере
	FCurIndex = 0;
	FCurIndexFiltr = 0;
	FFiltrEnable = false;
	FBufferFull = false;
	m_bDecimation = false;		// true НЕЛЬЗЯ !!!
	if (bInitData)
		FDelta = delta;
	// номер первой точки с поправкой на фильтр
	if (bInitData)
		FIndexStart0 = indexStart;
	FIndexStart = FIndexEnd = FIndexStart0;
	//FIndexEnd -= FDelta;
	ZeroMemory(pGlobalAddres, sizeT_Buffer);
}
//------------------------------------------------------------------------------
void CCirculBuffer::NewFiltr(int newLengthFiltr)
{// Новый фильтр (например: замена материнского вейвлета, который может иметь
	// прежнюю длину)
	//if(FLengthFiltr != newLengthFiltr) )
	{
		if (pDataForFiltr)
		{
			delete [] pDataForFiltr;
			pDataForFiltr = nullptr;
		}
		FLengthFiltr = newLengthFiltr;
		pDataForFiltr = new float [FLengthFiltr];
	}
	Clear();
}
//------------------------------------------------------------------------------