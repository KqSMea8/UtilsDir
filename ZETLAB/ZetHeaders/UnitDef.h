//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include "typeinfo.h"
#include <vector>
#include <ZetDef.h>
#include <WideResult\WideResult.h>
//------------------------------------------------------------------------------
enum Unit2DataType
{// Типы данных (результатов и параметров) Unit-2.
// Измерять порядок позиций НЕЛЬЗЯ! Новые типы добавлять ТОЛЬКО в конец!
	tdu_No,				// нет данных
	tdu_busy,			// занято

	tdu_txt,			// wchar_t[SizeString]

	tdu_32s,			// long
	tdu_32f,			// float
	tdu_64f,			// double

	tdu_32s_arr,		// std::vector<long>
	tdu_32f_arr,		// std::vector<float>
	tdu_64f_arr,		// std::vector<double>

	tdu_wrs_32s,		// WideResult<long>
	tdu_wrs_32f,		// WideResult<float>
	tdu_wrs_64f,		// WideResult<double>

	tdu_wrs_32s_arr,	// WideResult<std::vector<long>>
	tdu_wrs_32f_arr,	// WideResult<std::vector<float>>
	tdu_wrs_64f_arr,	// WideResult<std::vector<double>>

	tdu_str				// передача произвольных структур
};
//------------------------------------------------------------------------------
enum Unit2Error
{// Коды ошибок в Unit-2.
// Реальные значения должны быть отрицательными, поэтому использовать надо так:
//	long ret = -ErrNewGUID;

//	NOERROR = 0					// ошибок нет, определено в WinError.h
	// ошибки инициализации
	ErrCreateGUID = 1,			// ошибка при создании GUID
	ErrCreateFileMapping,		// ошибка при создании FileMapping
	ErrCreateMapView,			// ошибка при подключении к FileMapping
	ErrCreateEventReadParam,	// ошибка при создании EventReadParam
	ErrCreateEventReadResult,	// ошибка при создании EventReadResult
	ErrCreateEventEnd,			// ошибка при создании EventEnd	
	ErrCreateServer,			// ошибка при запуске серверного процесса

	ErrActivateServer,			// ошибка при выполнении сервером Activate
	ErrServerNotUnit2,			// сервер запущен не под UNIT-2
	ErrInCommandLine,			// в командной строке сервера нет GUID 

	// ошибки программ
	ErrServerIsLost = 20,		// нет главного окна сервера
	ErrServerIsHung,			// приложение сервера зависло
	ErrClientIsLost,			// нет главного окна клиента
	ErrClientIsHung,			// приложение клиента зависло

	// ошибки чтения/записи
	ErrStaticAreaIsSmall = 30,
	ErrDynamicAreaIsSmall,
	ErrCopyData,			// ошибка при копировании
	ErrReSizeVector,

	ErrWriteResult,				// ошибка при записи результатов
	ErrWriteParam,				// ошибка при записи параметров
	ErrReadResult,				// ошибка при чтении результатов
	ErrReadParam,				// ошибка при чтении параметров

	ErrReadyWriteResult,		// нет готовности записи результатов
//	ErrReadyReadResult,			// нет готовности чтения результатов
	ErrReadyWriteParam,			// нет готовности записи параметров
//	ErrReadyReadParam,			// нет готовности чтения параметров

	// ошибки результатов/параметров
	ErrSizeListResult = 40,		// несоответствие кол-ва результатов
	ErrSizeListParam,			// несоответствие кол-ва параметров
	ErrListParamIsEmpty,		// перечень параметров пуст
	ErrListResultIsEmpty,		// перечень результатов пуст
	ErrNoDataThisType,			// нет данных данного типа

	// общие ошибки
	ErrZeroLengthStr = 50,		// нулевая длина строки
	ErrNullPointerForType,		// нулевой указатель для типа
	ErrNullPointerForNumber,	// нулевой указатель для номера
	ErrNullPointerForValue,		// нулевой указатель для значения
	ErrNullPointerForSize,		// нулевой указатель для размера
	ErrCommandNew,
	
	ErrNullPointer = 60,		// нулевой указатель
	ErrNullValue,				// нулевое значение
	ErrNotNullValue,			// не нулевое значение
};
//------------------------------------------------------------------------------
// Область памяти для данных может быть статической или динамической.
//------------------------------------------------------------------------------
// Для всех шаблонных функций <class T> - это long, float или double
//------------------------------------------------------------------------------
struct Unit2DataStruct
{// Структура, содержащая свойства данных (параметров и результатов) Unit-2
	Unit2DataType type;		// тип данных
	long number;			// номер (код) данных (не может быть равен 0)
	BOOL dynamicArea;		// динамическая область памяти (или статическая)
	DWORD sizeArray;		// размер массива (если данные - массив)
	DWORD sizeDataInByte;	// размер данных (которые сейчас в структуре) в байтах
	DWORD sizeAreaInByte;	// размер области памяти в байтах
	BYTE *pData;			// указатель на область памяти для данных
	//--------------------------------------------------------------------------
	// Конструктор
	Unit2DataStruct()
	{
		type = tdu_No;
		number = 0;
		sizeArray = 
		sizeDataInByte = 
		sizeAreaInByte = 0;
		dynamicArea = TRUE;
		pData = NULL;
	}
	//--------------------------------------------------------------------------
	// Деструктор
	~Unit2DataStruct()
	{
		if (pData && (dynamicArea != FALSE))
		{
			delete [] pData;
			pData = NULL;
		}
	}
	//--------------------------------------------------------------------------
	// Вспомогательная функция проверки размера области памяти
	long TestSize(const DWORD newSize)
	{// Проверка и увеличение (при необходимости) размера области памяти
		long ret;
		if (sizeAreaInByte >= newSize)
			ret = NOERROR;
		else
		{
			if (dynamicArea != FALSE)
			{// динамическая область памяти, будем увеличивать
			//	но вначале удалить
				if (pData)
				{
					delete [] pData;
					pData = NULL;
				}
				try
				{
					pData = new BYTE [newSize];
					sizeAreaInByte = newSize;
					ret = NOERROR;
				}
				catch (CMemoryException*)
				{
					sizeAreaInByte = 0;
					ret = -ErrCommandNew;					
				}
			} 
			else	// статическая память, увеличивать нельзя, ошибка
				ret = -ErrStaticAreaIsSmall;
		}
		return ret;
	}
	//--------------------------------------------------------------------------
	// Вспомогательная функция копирования только параметров данных
	void CopyDescriptionOnly(const Unit2DataStruct &uds)
	{
		type = uds.type;					// тип данных
		number = uds.number;				// номер данных
		sizeArray = uds.sizeArray;			// размер массива
		sizeDataInByte = uds.sizeDataInByte;// размер данных в байтах
		sizeAreaInByte = uds.sizeAreaInByte;// размер области памяти в байтах
	}
	//--------------------------------------------------------------------------
	// Оператор "="
	long operator = (const Unit2DataStruct &uds)
	{
		long ret;
		if (uds.type != tdu_No)
		{//данные есть
			if (uds.pData != NULL)
			{//данные есть
				ret = TestSize(uds.sizeDataInByte);
				if (ret == NOERROR)
				{// можно копировать
					if ( memcpy_s((PVOID)pData, uds.sizeDataInByte,
									(PVOID)uds.pData, uds.sizeDataInByte) 
						== NOERROR )
					{// удачное копирование
						type = uds.type;
						number = uds.number;
						sizeArray = uds.sizeArray;
						sizeDataInByte = uds.sizeDataInByte;
						ret = NOERROR;
					}
					else
					{// не удачное копирование
						type = tdu_No;
						sizeArray = sizeDataInByte = 0;
						ret = -ErrCopyData;
					}
				}
				else
				{// копировать нельзя (мало места), ret уже определено
					type = tdu_No;
					sizeArray = sizeDataInByte = 0;
				}
			} 
			else
			{// данных нет, размеры оставляем без изменения, есть ошибка
				type = tdu_No;
				sizeArray = sizeDataInByte = 0;
				ret = -ErrNullPointer;
			}
		}
		else
		{// данных нет, размеры оставляем как есть, ошибок нет
			type = tdu_No;
			sizeArray = sizeDataInByte = 0;
			ret = NOERROR;
		}
		return ret;
	}
	////////////////////////////////////////////////////////////////////////////
	// Функции копирования данных в структуру
	////////////////////////////////////////////////////////////////////////////
	// Данные - текст
	long Copy_txt(const long num, LPCTSTR text)
	{
		long ret;
		sizeArray = ((CString)text).GetLength() + 1;
		sizeDataInByte = sizeArray * sizeof(wchar_t);
		ret = TestSize(sizeDataInByte);
		if (ret == NOERROR)
		{// можно копировать
			if ( wcscpy_s( (wchar_t*)pData, sizeArray, text ) == NOERROR )
			{// копирование удачное, ошибок нет
				type = tdu_txt;
				number = num;
			}
			else
			{// копирование не удачное, ошибка
				type = tdu_No;
				sizeArray = sizeDataInByte = 0;
				ret = -ErrCopyData;
			}
		}
		else
		{// размер не позволяет копировать, ошибка, ret уже определён
			type = tdu_No;
			sizeArray = sizeDataInByte = 0;
		}
		return ret;
	}
	//--------------------------------------------------------------------------
	// Данные - число
	template <class T>
	long Copy_val(const long num, const T &val)
	{
		long ret;
		switch (*typeid(T).name())
		{
		case 'l': type = tdu_32s; break;
		case 'f': type = tdu_32f; break;
		case 'd': type = tdu_64f; break;
		default:  type = tdu_No;
		}
		if (type != tdu_No)
		{// тип данных допустим
			sizeArray = 1;
			sizeDataInByte = sizeof(T);
			ret = TestSize(sizeDataInByte);
			if (ret == NOERROR)
			{// можно копировать
				number = num;
				*((T*)pData) = val;
			}
			else
			{// размер не позволяет копировать, ошибка, ret уже определён
				type = tdu_No;
				sizeArray = sizeDataInByte = 0;
			}
		} 
		else	// недопустимый тип данных, ошибка
			ret = -ErrNoDataThisType;
		return ret;
	}
	//--------------------------------------------------------------------------
	// Данные - массив чисел
	template <class T>
	long Copy_arr(const long num, const DWORD size_Array, const T *pArray)
	{
		long ret;
		switch (*typeid(T).name())
		{
		case 'l': type = tdu_32s_arr; break;
		case 'f': type = tdu_32f_arr; break;
		case 'd': type = tdu_64f_arr; break;
		default:  type = tdu_No;
		}
		if (type != tdu_No)
		{// допустимый тип данных
			if (pArray != NULL)
			{
				sizeArray = size_Array;
				sizeDataInByte = sizeof(T) * sizeArray;
				ret = TestSize(sizeDataInByte);
				if (ret == NOERROR)
				{// можно копировать
					if (memcpy_s((PVOID)pData, sizeDataInByte,
								 (PVOID)pArray, sizeDataInByte) == 0)
						// копирование удачное, ошибок нет, ret уже NOERROR
						number = num;
					else
					{// копирование не удачное, ошибка
						type = tdu_No;
						sizeArray = sizeDataInByte = 0;
						ret = -ErrCopyData;
					}
				}
				else
				{// размер не позволяет копировать, ошибка, ret уже определён
					type = tdu_No;
					sizeArray = sizeDataInByte = 0;
				}
			}
			else// нулевой указатель
				ret = -ErrNullPointer;
		}
		else	// недопустимый тип данных, ошибка
			ret = -ErrNoDataThisType;
		return ret;
	}
	//--------------------------------------------------------------------------
	// Широкий результат типа число
	template <class T>
	long Copy_wrs_val(const long num, const WideResult<T> *pVal)
	{
		long ret;
		switch (*typeid(T).name())
		{
		case 'l': type = tdu_wrs_32s; break;
		case 'f': type = tdu_wrs_32f; break;
		case 'd': type = tdu_wrs_64f; break;
		default:  type = tdu_No;
		}
		if (type != tdu_No)
		{// допустимый тип данных
			if (pVal != NULL)
			{
				sizeArray = 1;
				sizeDataInByte = sizeof(WideResult<T>);
				ret = TestSize(sizeDataInByte);
				if (ret == NOERROR)
				{// можно копировать
					*((WideResult<T>*)pData) = *pVal;
					number = num;
				}
				else
				{// указатель NULL, копировать нельзя, ошибка
					type = tdu_No;
					sizeArray = sizeDataInByte = 0;
					ret = -ErrNullPointer;
				}
			}
			else// нулевой указатель
					ret = -ErrNullPointer;
		} 
		else	// недопустимый тип данных, ошибка
			ret = -ErrNoDataThisType;
		return ret;
	}
	//--------------------------------------------------------------------------
	// Широкий результат типа массив чисел
	template <class T>
	long Copy_wrs_arr(const long num, const DWORD size_Array,
						const WideResult<T*> *pVal)
	{
		long ret;
		switch (*typeid(T).name())
		{
		case 'l': type = tdu_wrs_32s_arr; break;
		case 'f': type = tdu_wrs_32f_arr; break;
		case 'd': type = tdu_wrs_64f_arr; break;
		default:  type = tdu_No;
		}
		if (type != tdu_No)
		{// допустимый тип данных
			if ((pVal != NULL) && (pVal->value != NULL))
			{
				sizeArray = size_Array;
				sizeDataInByte = sizeof(double) + sizeof(long) +
					sizeof(T) * sizeArray;
				ret = TestSize(sizeDataInByte);
				if (ret == NOERROR)
				{// можно копировать
					BYTE *add = pData;
					*((double*)add) = pVal->time;
					add += sizeof(double);
					*((long*)add) = pVal->quality;
					add += sizeof(long);
					DWORD size = sizeArray * sizeof(T);
					if (memcpy_s((PVOID)add, size,
						(PVOID)pVal->value, size) == 0)
						// копирование удачное, ошибок нет, ret уже NOERROR
						number = num;
					else
					{// копирование не удачное, ошибка
						type = tdu_No;
						sizeArray = sizeDataInByte = 0;
						ret = -ErrCopyData;
					}
				}
				else
				{// размер не позволяет копировать, ошибка, ret уже определён
					type = tdu_No;
					sizeArray = sizeDataInByte = 0;
				}
			}
			else
				ret = -ErrNullPointer;
		} 
		else	// недопустимый тип данных, ошибка
			ret = -ErrNoDataThisType;
		return ret;
	}
	// Широкий результат типа вектор чисел
	//template <class T>
	//long Copy_wrs_arr(const long num, const WideResult<std::vector<T>> *pVal)
	//{
	//	long ret;
	//	switch (*typeid(T).name())
	//	{
	//	case 'l': type = tdu_wrs_32s_arr; break;
	//	case 'f': type = tdu_wrs_32f_arr; break;
	//	case 'd': type = tdu_wrs_64f_arr; break;
	//	default:  type = tdu_No;
	//	}
	//	if (type != tdu_No)
	//	{// допустимый тип данных
	//		sizeArray = (DWORD)pVal->value.size();
	//		sizeDataInByte = sizeof(double) + sizeof(long) +
	//			sizeof(T) * sizeArray;
	//		ret = TestSize(sizeDataInByte);
	//		if (ret == NOERROR)
	//		{// можно копировать
	//			BYTE *add = pData;
	//			*((double*)add) = pVal->time;
	//			add += sizeof(double);
	//			*((long*)add) = pVal->quality;
	//			add += sizeof(long);
	//			DWORD size = sizeArray * sizeof(T);
	//			if (memcpy_s((PVOID)add, size,
	//				(PVOID)pVal->value.cbegin()._Ptr, size) == 0)
	//				// копирование удачное, ошибок нет, ret уже NOERROR
	//				number = num;
	//			else
	//			{// копирование не удачное, ошибка
	//				type = tdu_No;
	//				sizeArray = sizeDataInByte = 0;
	//				ret = -ErrCopyData;
	//			}
	//		}
	//		else
	//		{// размер не позволяет копировать, ошибка, ret уже определён
	//			type = tdu_No;
	//			sizeArray = sizeDataInByte = 0;
	//		}
	//	}
	//	else	// недопустимый тип данных, ошибка
	//		ret = -ErrNoDataThisType;
	//	return ret;
	//}
	////////////////////////////////////////////////////////////////////////////
	// Функции извлечения данных из структуры
	////////////////////////////////////////////////////////////////////////////
	// Данные - текст
	long Extract_txt(CString &str)
	{
		long ret;
		if (type == tdu_txt)
		{// допустимый тип данных
			str = (CString)(wchar_t*)pData;
			ret = NOERROR;
		}
		else	// не допустимый тип данных, ошибка
			ret = -ErrNoDataThisType;
		return ret;
	}
	//--------------------------------------------------------------------------
	// Данные - числа, преобразованные к double. Для совместимости с UNIT-1
	long Extract_unit1(double &val)
	{
		long ret = NOERROR;
		if (type == tdu_32s)
			val = double(*((long*)pData));

		else if (type == tdu_32f)
			val = double(*((float*)pData));

		else if (type == tdu_64f)
			val = *((double*)pData);

		else	// не допустимый тип данных, ошибка
			ret = -ErrNoDataThisType;
		return ret;
	}
	//--------------------------------------------------------------------------
	// Данные - число
	template <class T>
	long Extract_val(T &val)
	{
		long ret;
		if ( (type == tdu_32s) || (type == tdu_32f) ||
			 (type == tdu_64f) )
		{// допустимый тип данных
			val = *((T*)pData);
			ret = NOERROR;
		}
		else	// не допустимый тип данных, ошибка
			ret = -ErrNoDataThisType;
		return ret;
	}
	//--------------------------------------------------------------------------
	// Данные - массив чисел
	template <class T>
	long Extract_arr(T *pArray)
	{
		long ret;
		if ((type == tdu_32s_arr) || (type == tdu_32f_arr) ||
			(type == tdu_64f_arr))
		{// допустимый тип данных
			if (pArray != NULL)
			{// можно копировать
				try
				{
					if (memcpy_s((PVOID)pArray, sizeDataInByte,
								 (PVOID)pData, sizeDataInByte) == 0)
						ret = NOERROR;	// копирование удачное, ошибок нет
					else	// неудачное копирование, ошибка
						ret = -ErrCopyData;
				}
				catch (CMemoryException*)
				{
					ret = -ErrCopyData;
				}
			}
			else	// нулевой указаль, ошибка
				ret = -ErrNullPointer;
		}
		else	// не допустимый тип данных, ошибка
			ret = -ErrNoDataThisType;
		return ret;
	}
	//--------------------------------------------------------------------------
	// Широкий результат типа число
	template <class T>
	long Extract_wrs_val(WideResult<T> *pVal)
	{
		long ret;
		if ( (type == tdu_wrs_32s) || (type == tdu_wrs_32f) ||
			(type == tdu_wrs_64f) )
		{// допустимый тип данных
			if (pVal != NULL)
			{
				*pVal = *((WideResult<T>*)pData);
				ret = NOERROR;
			}
			else	// нулевой указаль, ошибка
				ret = -ErrNullPointer;
		}
		else	// не допустимый тип данных, ошибка
			ret = -ErrNoDataThisType;
		return ret;
	}
	//--------------------------------------------------------------------------
	// Широкий результат типа вектор чисел
	template <class T>
	long Extract_wrs_arr(WideResult<T*> *pVal)
	{
		long ret;
		if ( (type == tdu_wrs_32s_arr) || (type == tdu_wrs_32f_arr) ||
			 (type == tdu_wrs_64f_arr) )
		{// допустимый тип данных
			if ((pVal != NULL) && (pVal->value != NULL) )
			{
				BYTE *add = pData;
				pVal->time = *((double*)add);	// время
				add += sizeof(double);
				pVal->quality = *((long*)add);	// качество
				add += sizeof(long);
				// теперь числа в массив
				DWORD size = sizeArray * sizeof(T);
				try
				{
					if (memcpy_s((PVOID)pVal->value, size, (PVOID)add, size) == 0)
						ret = NOERROR;	// копирование удачное, ошибок нет
					else   // неудачное копирование, ошибка
						ret = -ErrCopyData;
				}
				catch (CMemoryException*)
				{
					ret = -ErrCopyData;					
				}
			}
			else	// нулевой указаль, ошибка
				ret = -ErrNullPointer;
		}
		else	// не допустимый тип данных, ошибка
			ret = -ErrNoDataThisType;
		return ret;
	}
	//--------------------------------------------------------------------------
	//// Широкий результат типа вектор чисел
	//template <class T>
	//long Extract_wrs_arr(WideResult<std::vector<T>> *pVal)
	//{
	//	long ret;
	//	if ((type == tdu_wrs_32s_arr) || (type == tdu_wrs_32f_arr) ||
	//		(type == tdu_wrs_64f_arr))
	//	{// допустимый тип данных
	//		BYTE *add = pData;
	//		pVal->time = *((double*)add);	// время
	//		add += sizeof(double);
	//		pVal->quality = *((long*)add);	// качество
	//		add += sizeof(long);
	//		// теперь числа в вектор
	//		pVal->value.resize(sizeArray);
	//		if (sizeArray == (DWORD)pVal->value.size())
	//		{// можно копировать
	//			DWORD size = sizeArray * sizeof(T);
	//			if (memcpy_s((PVOID)pVal->value.cbegin()._Ptr, size,
	//				(PVOID)add, size) == 0)
	//				ret = NOERROR;	// копирование удачное, ошибок нет
	//			else
	//			{// неудачное копирование, ошибка
	//				pVal->value.clear();
	//				ret = -ErrCopyData;
	//			}
	//		}
	//		else// нет места для копирования, ошибка
	//		{
	//			pVal->value.clear();
	//			ret = -ErrReSizeVector;
	//		}
	//	}
	//	else	// не допустимый тип данных, ошибка
	//		ret = -ErrNoDataThisType;
	//	return ret;
	//}
	//--------------------------------------------------------------------------
	// Чтение размера структуры при type == tdu_str
	long GetSizeForExtract_str(DWORD *pSize)
	{
		long ret = (type == tdu_str) ? NOERROR : -ErrNoDataThisType;
		if (ret == NOERROR)
		{
			if (pSize)
				*pSize = sizeDataInByte;
			else
				ret = -ErrNullPointerForSize;
		}
		return ret;
	}
	//--------------------------------------------------------------------------
	// произвольная структура
	long Extract_str(PVOID pointer)
	{
		long ret = (type == tdu_str) ? NOERROR : -ErrNoDataThisType;
		if (ret == NOERROR)
		{
			if (pointer)
			{
				if ( memcpy_s(pointer, sizeDataInByte,
							 (PVOID)pData, sizeDataInByte) != 0)
					ret = -ErrCopyData;
			}
			else
				ret = -ErrNullPointer;
		}
		return ret;
	}
};
//------------------------------------------------------------------------------