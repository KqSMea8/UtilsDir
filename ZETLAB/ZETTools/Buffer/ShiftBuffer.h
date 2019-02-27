//------------------------------------------------------------------------------
//	Шаблонный класс CShiftBuffer предназначен для реализации сдвиговых буферов
//	Версия 1.1 от 19.04.2011
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
//------------------------------------------------------------------------------
template <class T>
class CShiftBuffer
{
private:
	int FSize;				// кол-во элементов в буфере
	int FSizePage;			// кол-во элементов в странице буфера

	bool bCreateData;		// буфер внутренний
	int NumberPage;			// кол-во страниц в буфере
	int NumberPage_1;		// кол-во страниц в буфере - 1
	T* pGlobalAddres;		// адрес буфера
	T* pPointerFirstPage;	// адрес первой страницы (счёт с 0)
	T* pPointerLastPage;	// адрес последней страницы
	T* pPointerLastElement;	// адрес последнего элемента
	int sizeT;				// размер в байтах одного элемента
	int sizeT_Page;			// размер в байтах страницы
	int sizeT_NumberPage_1;	// размер в байтах буфера без одной страницы
	int sizeT_Buffer;		// размер в байтах всего буфера

protected:

public:
	CShiftBuffer<T>();
	virtual ~CShiftBuffer();
	bool Create(int size, int sizePage, T* addBuf = NULL);

	// функции свойств
	T* GetGlobalAddres() { return pGlobalAddres; }
	T* GetPointerLastPage() { return pPointerLastPage; }
	T* GetPointerLastElement() { return pPointerLastElement; }

	int GetSize() { return FSize; }
	bool SetSize(int size);			// новый размер буфера

	int GetSizePage() { return FSizePage; }
	bool SetSizePage(int val);		// расчет всех переменных класса

	int GetSizeInByte() { return sizeT_Buffer; }
	int GetSizePageInByte() { return sizeT_Page; }

	// функции класса
	void Clear();					// обнуление всего буфера
	void NaN();						// заливка NaN-ми
	void CopyNewData(T *addLastPage);// только копирование новой порции
	void NewData(T *addLastPage);	// сдвиг данных и копирование новой порции
	void ShiftData();		// только сдвиг данных в буфере для новой порции
};
//------------------------------------------------------------------------------
template <class T>
CShiftBuffer<T>::CShiftBuffer()
{
	sizeT = sizeof(T);				// размер в байтах одного элемента буфера
}
//------------------------------------------------------------------------------
template <class T>
CShiftBuffer<T>::~CShiftBuffer()
{
	if (bCreateData)
	{ 
		delete pGlobalAddres;
		pGlobalAddres = nullptr;
	}
}
//------------------------------------------------------------------------------
template <class T>
bool CShiftBuffer<T>::Create(int size, int sizePage, T* addBuf)
{
	bool ret = true;
	FSize = size;				// кол-во элементов в буфере

	if (addBuf)
	{
		bCreateData = false;	// буфер внешний
		pGlobalAddres = addBuf;
	}
	else
	{
		bCreateData = true;		// буфер внутренний
		try
		{
			pGlobalAddres = new T [FSize];
		}
		catch (CMemoryException*)
		{
			pGlobalAddres = NULL;
			ret = false;			
		}
	}

	if (ret)
		SetSizePage(sizePage);
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
bool CShiftBuffer<T>::SetSize(int size)
{
	bool ret = true;
	if ( FSize != size )
	{
		if (bCreateData)
		{	// внутренний буфер
			delete pGlobalAddres;
			pGlobalAddres = nullptr;
			try
			{
				pGlobalAddres = new T [FSize];
			}
			catch (CMemoryException*)
			{
				pGlobalAddres = NULL;
				ret = false;			
			}
		}
		FSize = size;
		if (ret)
			SetSizePage(FSizePage);		// обязательно
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
bool CShiftBuffer<T>::SetSizePage(int val)
{// Считаю, что размер страницы задан правильно, т.е. кратно размеру буфера
	// устанавливаю все переменные класса и обнуляю буфер !!!
	bool ret;
	if (val > 0)
	{
		FSizePage = val;
		NumberPage_1 = NumberPage = FSize / FSizePage;
		NumberPage_1--;
		pPointerLastElement = pGlobalAddres + FSize - 1;
		pPointerLastPage = pPointerLastElement - FSizePage + 1;
		pPointerFirstPage = pGlobalAddres + FSizePage;
		sizeT_Page = sizeT * FSizePage;
		sizeT_NumberPage_1 = sizeT_Page * NumberPage_1;
		sizeT_Buffer = sizeT_Page + sizeT_NumberPage_1;
		Clear();
		ret = true;
	}
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
void CShiftBuffer<T>::Clear()
{	
	ZeroMemory(pGlobalAddres, sizeT_Buffer);
}
//------------------------------------------------------------------------------
template <class T>
void CShiftBuffer<T>::NaN()
{
	memset(pGlobalAddres, 0xFF, sizeT_Buffer);
}
//------------------------------------------------------------------------------
template <class T>
void CShiftBuffer<T>::CopyNewData(T *addLastPage)
{	// новые данные копируются в последнюю порцию
	memcpy_s(pPointerLastPage, sizeT_Page, addLastPage, sizeT_Page);
}	
//------------------------------------------------------------------------------
template <class T>
void CShiftBuffer<T>::NewData(T *addLastPage)
{	// вначале сдвигаю данные в буфере (влево на одну порцию),
	// поток на освободившееся место последней порции копирую новые данные
	ShiftData();
	CopyNewData(addLastPage);
}
//------------------------------------------------------------------------------
template <class T>
void CShiftBuffer<T>::ShiftData()
{	// сдвигаю данные в буфере (влево на одну порцию), и больше ничего не делаю
	memcpy_s(pGlobalAddres, sizeT_NumberPage_1,
				pPointerFirstPage, sizeT_NumberPage_1);
}
//------------------------------------------------------------------------------