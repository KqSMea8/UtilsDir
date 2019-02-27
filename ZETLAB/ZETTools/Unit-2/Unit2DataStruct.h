//------------------------------------------------------------------------------
#pragma once
#include "Unit-2\Unit2Def.h"		// определение типов и кодов ошибок
//------------------------------------------------------------------------------
//		Структуры для представления данных Unit-2.1 в статических или
// динамических массивах. В Unit2DataStatic текущий результат хранится в
// FileMapping. В Unit2DataStruct результаты и параметры передаются из
// FileMapping в CUnitSrv и CUnitCln.
//------------------------------------------------------------------------------
// Для всех шаблонных функций <class T> - это long, float или double
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
//		Родительская структура Unit2Data
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
struct Unit2Data
{// Структура, содержащая свойства данных Unit. От этой структуры
// бутут созданы две дочерние структуры: для данных хранящихся в статических 
// (т.е. в FileMapping), и динамических массивах (в CUnitSrv и CUnitCln).
// Размер Unit2Data = 5 * 4 байт. = 20 байт
	long number;			// номер (код) данных
	Unit2DataType type;		// тип данных
	DWORD sizeArray;		// размер массива (если данные - массив)
	DWORD sizeDataInByte;	// размер данных (которые сейчас в структуре) в байтах
	DWORD sizeAreaInByte;	// размер области памяти в байтах

	Unit2Data()
	{
		number = 0;
		type = tdu_No;
		sizeArray = 
		sizeDataInByte = 
		sizeAreaInByte = 0;
	}
	//--------------------------------------------------------------------------
protected:
	//--------------------------------------------------------------------------
	// Копирование данных из одной структуры в другую.
	// Проверка размеров делается до вызова функции!!!
	long Copy(const Unit2Data *pDescriptSrc, const BYTE *pDataSrc,
				Unit2Data *pDescriptDst, BYTE *pDataDst)
	{
		bool flag;
		long ret;
		if (pDescriptSrc->type != tdu_No)
		{
			if (pDataSrc != NULL)
			{
				if (pDataDst != NULL)
				{
					if ( memcpy_s(pDataDst, (rsize_t)pDescriptDst->sizeAreaInByte,
						pDataSrc, (rsize_t)pDescriptSrc->sizeDataInByte) == 0 )
					{
						pDescriptDst->type = pDescriptSrc->type;
						pDescriptDst->number = pDescriptSrc->number;
						pDescriptDst->sizeArray = pDescriptSrc->sizeArray;
						pDescriptDst->sizeDataInByte = pDescriptSrc->sizeDataInByte;
						ret = NOERROR;
						flag = false;
					}
					else
					{
						ret = -ErrCopyData;
						flag = true;
					}
				}
				else
				{
					ret = -ErrNullPointer;
					flag = true;
				}
			}
			else
			{
				ret = NOERROR;	// нет данных в исходной структуре
				flag = true;
			}
		}
		else
		{
			ret = NOERROR;	// нет данных в исходной структуре
			flag = true;
		}
		// данных не было или маленький размер
		if (flag)
		{
			pDescriptDst->type = tdu_No;
			pDescriptDst->number = 0;
			pDescriptDst->sizeArray = 0;
			pDescriptDst->sizeDataInByte = 0;
		}
		return ret;
	}
	//--------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////
	//		Функции копирования данных в структуру.
	// Перед вызовом этих функций должны быть проверены: размер области данных
	// и указатель pData у структур с динамическим массивом данных
	////////////////////////////////////////////////////////////////////////////
	//--------------------------------------------------------------------------
	// Данные - текст
	long _Copy_txt(const long num, LPCTSTR text, BYTE *pData)
	{
		long ret;
		sizeArray = ((CString)text).GetLength() + 1;	// + ноль
		if ( wcscpy_s( (wchar_t*)pData, (rsize_t)sizeArray, text ) == NOERROR )
		{// копирование удачное, ошибок нет
			ret = NOERROR;
			type = tdu_txt;
			number = num;
			sizeDataInByte = sizeArray * (DWORD)sizeof(wchar_t);
		}
		else
		{// копирование не удачное, ошибка
			ret = -ErrCopyText;
			type = tdu_No;
			sizeArray = sizeDataInByte = 0;
		}
		return ret;
	}
	//--------------------------------------------------------------------------
	// Данные - произвольная структура
	long _Copy_str(const long num, const DWORD size, PVOID pointer, BYTE *pData)
	{
		long ret = (pointer != NULL) ? NOERROR : -ErrNullPointer;
		if (ret == NOERROR)
		{
			if (size > 0)
			{
				if ( memcpy_s( (PVOID)pData, (rsize_t)size,
								pointer, (rsize_t)size ) == NOERROR )
				{// копирование удачное, ошибок нет
					type = tdu_str;
					sizeArray = 1;
					sizeDataInByte = size;
					number = num;
				}
				else
				{// копирование не удачное, ошибка
					ret = -ErrCopyData;
					type = tdu_No;
					sizeArray = sizeDataInByte = 0;
				}
			}
			else ret = -ErrNullValue;
		}
		return ret;
	}
	//--------------------------------------------------------------------------
	// Данные - число
	template <class T>
	long _Copy_val(const long num, const T &val, BYTE *pData)
	{
		switch (*typeid(T).name())
		{
		case 'l': type = tdu_32s; break;
		case 'f': type = tdu_32f; break;
		case 'd': type = tdu_64f; break;
		default:  type = tdu_No;
		}
		long ret = (type != tdu_No) ? NOERROR: -ErrNoDataThisType;
		if (ret == NOERROR)
		{// тип данных допустим
			number = num;
			sizeDataInByte = sizeof(T);
			sizeArray = 1;
			*((T*)pData) = val;
		}
		return ret;
	}
	//--------------------------------------------------------------------------
	// Данные - вектор чисел
	template <class T>
	long _Copy_arr(const long num, const std::vector<T> *pVector, BYTE *pData)
	{
		switch (*typeid(T).name())
		{
		case 'l': type = tdu_32s_arr; break;
		case 'f': type = tdu_32f_arr; break;
		case 'd': type = tdu_64f_arr; break;
		default:  type = tdu_No;
		}
		long ret = (type != tdu_No) ? NOERROR: -ErrNoDataThisType;
		if (ret == NOERROR)
		{// допустимый тип данных
			sizeArray = (DWORD)pVector->size();
			sizeDataInByte = (DWORD)sizeof(T) * sizeArray;
			if (memcpy_s((PVOID)pData, (rsize_t)sizeDataInByte,
					(PVOID)pVector->cbegin()._Ptr, (rsize_t)sizeDataInByte)
					== 0)
				// копирование удачное, ошибок нет, ret уже NOERROR
				number = num;
			else
			{// копирование не удачное, ошибка
				ret = -ErrCopyData;
				type = tdu_No;
				sizeArray = sizeDataInByte = 0;
			}
		}
		return ret;
	}
	//--------------------------------------------------------------------------
	// Широкий результат типа число
	template <class T>
	long _Copy_wrs_val(const long num, const WideResult<T> *pVal, BYTE *pData)
	{
		switch (*typeid(T).name())
		{
		case 'l': type = tdu_wrs_32s; break;
		case 'f': type = tdu_wrs_32f; break;
		case 'd': type = tdu_wrs_64f; break;
		default:  type = tdu_No;
		}
		long ret = (type != tdu_No) ? NOERROR: -ErrNoDataThisType;
		if (ret == NOERROR)
		{// допустимый тип данных
			number = num;
			sizeDataInByte = sizeof(WideResult<T>);
			sizeArray = 1;
			*((WideResult<T>*)pData) = *pVal;
		}
		return ret;
	}
	//--------------------------------------------------------------------------
	// Широкий результат типа вектор чисел
	template <class T>
	long _Copy_wrs_arr(const long num,
						const WideResult<std::vector<T>> *pVal, BYTE *pData)
	{
		switch (*typeid(T).name())
		{
		case 'l': type = tdu_wrs_32s_arr; break;
		case 'f': type = tdu_wrs_32f_arr; break;
		case 'd': type = tdu_wrs_64f_arr; break;
		default:  type = tdu_No;
		}
		long ret = (type != tdu_No) ? NOERROR: -ErrNoDataThisType;
		if (ret == NOERROR)
		{// допустимый тип данных
			sizeArray = (DWORD)pVal->value.size();
			sizeDataInByte = (DWORD)( sizeof(double) + sizeof(long)) +
				(DWORD)sizeof(T) * sizeArray;
			BYTE *add = pData;
			*((double*)add) = pVal->time;
			add += sizeof(double);
			*((long*)add) = pVal->quality;
			add += sizeof(long);
			DWORD size = sizeArray * (DWORD)sizeof(T);
			if (memcpy_s((PVOID)add, (rsize_t)size,
				(PVOID)pVal->value.cbegin()._Ptr, (rsize_t)size) == 0)
				// копирование удачное, ошибок нет, ret уже NOERROR
				number = num;
			else
			{// копирование не удачное, ошибка
				type = tdu_No;
				sizeArray = sizeDataInByte = 0;
				ret = -ErrCopyData;
			}
		}
		return ret;
	}
	//--------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////
	//		Функции извлечения данных из структуры.
	// Перед вызовом этих функций должен быть проверен указатель pData
	// у структур с динамическим массивом данных
	////////////////////////////////////////////////////////////////////////////
	// Данные - текст
	long _Extract_txt(CString &str, BYTE *pData)
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
	long _Extract_unit1(double &val, BYTE *pData)
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
	long _Extract_val(T &val, BYTE *pData)
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
	// Данные - вектор чисел
	template <class T>
	long _Extract_arr(std::vector<T> *pVector, BYTE *pData)
	{
		long ret;
		if ( (type == tdu_32s_arr) || (type == tdu_32f_arr) ||
			 (type == tdu_64f_arr) )
		{// допустимый тип данных
			pVector->resize((size_t)sizeArray);
			if (sizeArray == (DWORD)pVector->size())
			{// можно копировать
				if ( memcpy_s((PVOID)pVector->cbegin()._Ptr, (rsize_t)sizeDataInByte,
								(PVOID)pData, (rsize_t)sizeDataInByte) == 0)
					ret = NOERROR;	// копирование удачное, ошибок нет
				else
				{// неудачное копирование, ошибка
					pVector->clear();
					ret = -ErrCopyData;
				}
			}
			else
			{// нет места для копирования, ошибка
				pVector->clear();
				ret = -ErrReSizeVector;
			}
		}
		else	// не допустимый тип данных, ошибка
			ret = -ErrNoDataThisType;
		return ret;
	}
	//--------------------------------------------------------------------------
	// Широкий результат типа число
	template <class T>
	long _Extract_wrs_val(WideResult<T> *pVal, BYTE *pData)
	{
		long ret;
		if ( (type == tdu_wrs_32s) || (type == tdu_wrs_32f) ||
			 (type == tdu_wrs_64f) )
		{// допустимый тип данных
			*pVal = *((WideResult<T>*)pData);
			ret = NOERROR;
		}
		else	// не допустимый тип данных, ошибка
			ret = -ErrNoDataThisType;
		return ret;
	}
	//--------------------------------------------------------------------------
	// Широкий результат типа вектор чисел
	template <class T>
	long _Extract_wrs_arr(WideResult<std::vector<T>> *pVal, BYTE *pData)
	{
		long ret;
		if ( (type == tdu_wrs_32s_arr) || (type == tdu_wrs_32f_arr) ||
			 (type == tdu_wrs_64f_arr) )
		{// допустимый тип данных
			BYTE *add = pData;
			pVal->time = *((double*)add);	// время
			add += sizeof(double);
			pVal->quality = *((long*)add);	// качество
			add += sizeof(long);
			// теперь числа в вектор
			pVal->value.resize(sizeArray);
			if (sizeArray == (DWORD)pVal->value.size())
			{// можно копировать
				rsize_t size = (rsize_t)sizeArray * (rsize_t)sizeof(T);
				if ( memcpy_s((PVOID)pVal->value.cbegin()._Ptr, size,
								(PVOID)add, size) == 0)
					ret = NOERROR;	// копирование удачное, ошибок нет
				else ret = -ErrCopyData;		// неудачное копирование, ошибка
			}
			else ret = -ErrReSizeVector;		// нет места для копирования, ошибка
		}
		else	// не допустимый тип данных, ошибка
			ret = -ErrNoDataThisType;
		if (ret < 0)
		{
			pVal->value.clear();
			pVal->quality = 0;
			pVal->time = 0;
		}
		return ret;
	}
	//--------------------------------------------------------------------------
	long GetSizeForExtract_str(DWORD *pSize)
	{
		long ret;
		ret = (type == tdu_str) ? NOERROR : -ErrNoDataThisType;
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
	long _Extract_str(PVOID pointer, BYTE *pData)
	{
		long ret;
		ret = (type == tdu_str) ? NOERROR : -ErrNoDataThisType;
		if (ret == NOERROR)
		{
			if (pointer)
			{
				if (sizeDataInByte > 0)
				{
					if ( memcpy_s( pointer, (rsize_t)sizeDataInByte,
									(PVOID)pData, (rsize_t)sizeDataInByte )
						!= 0 )
						ret = -ErrCopyData;
				}
				else
					ret = -ErrNullValue;
			}
			else
				ret = -ErrNullPointerForValue;
		}
		return ret;
	}
};
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
//		Структура для хранения данных в статическом массиве (в FileMapping)
////////////////////////////////////////////////////////////////////////////////
// sizeof(Unit2DataStatic) = sizeof(Unit2Data) +	// 5 * 4 байтов
//							sizeof(DWORD) + 8;		// 4 + 8 - это pData.
// Итого 32 байта !!!  Но т.к. это статический массив, то
// Unit2DataStatic.size может быть больше 32 байта !!!
//------------------------------------------------------------------------------
struct Unit2DataStatic : Unit2Data
{
	DWORD size;				// размер структуры
	BYTE pData[8];			// 8 - для задания размера, т.е. выравнивания под 8
	//--------------------------------------------------------------------------
	// Конструктор
	Unit2DataStatic()
	{
		size = (DWORD)( sizeof(Unit2Data) + sizeof(DWORD) + sizeof(pData) );
		pData[0] = pData[1] = pData[2] = pData[3] =
		pData[4] = pData[5] = pData[6] = pData[7] = 0;
	}
	//--------------------------------------------------------------------------
	// Проверка размера
	long TestSize(const DWORD newSize)
	{
		long ret;
		if (sizeAreaInByte >= newSize)
			ret = NOERROR;
		else
			ret = -ErrSizeAreaIsSmall;
		return ret;
	}
	//--------------------------------------------------------------------------
	// Оператор "="
	long operator = (const Unit2DataStatic &uds)
	{
		long ret = TestSize(uds.sizeDataInByte);
		if (ret == NOERROR)
			ret = Copy(&uds, uds.pData, this, pData);
		return ret;
	}
	//--------------------------------------------------------------------------
	// Оператор "="
	//long operator = (const Unit2DataStruct &uds)
	//{
	//	
	//}
	//--------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////
	//		Функции копирования данных в структуру.
	// Перед вызовом одноимённых родительских функций нужно проверять размер.
	////////////////////////////////////////////////////////////////////////////
	//--------------------------------------------------------------------------
	// Данные - текст
	long Copy_txt(const long num, LPCTSTR text)
	{
		long ret = TestSize( (DWORD)(((CString)text).GetLength() + 1) *
								(DWORD)sizeof(wchar_t) );
		if (ret == NOERROR)
			ret = _Copy_txt(num, text, pData);
		return ret;
	}
	//--------------------------------------------------------------------------
	// Произвольная структура
	long Copy_str(const long num, DWORD size, PVOID poinret)
	{
		long ret = TestSize(size);
		if (ret == NOERROR)
			ret = _Copy_str(num, size, poinret, pData);
		return ret;
	}
	//--------------------------------------------------------------------------
	// Данные - число
	template <class T>
	long Copy_val(const long num, const T &val)
	{
		long ret = TestSize( sizeof(T) );
		if (ret == NOERROR)
			ret = _Copy_val(num, val, pData);
		return ret;
	}
	//--------------------------------------------------------------------------
	// Данные - вектор чисел
	template <class T>
	long Copy_arr(const long num, const std::vector<T> *pVector)
	{
		long ret = TestSize( (DWORD)( sizeof(T) * pVector->size() ) );
		if (ret == NOERROR)
			ret =  _Copy_arr(num, pVector, pData);
		return ret;
	}
	//--------------------------------------------------------------------------
	// Широкий результат типа число
	template <class T>
	long Copy_wrs_val(const long num, const WideResult<T> *pVal)
	{
		long ret = TestSize( sizeof( WideResult<T> ) );
		if (ret == NOERROR)
			ret = _Copy_wrs_val(num, pVal, pData);
		return ret;
	}
	//--------------------------------------------------------------------------
	// Широкий результат типа вектор чисел
	template <class T>
	long Copy_wrs_arr(const long num, const WideResult<std::vector<T>> *pVal)
	{
		long ret = TestSize( (DWORD)( sizeof(T) * pVal->value.size() +
							 sizeof(long) + sizeof(double) ) );
		if (ret == NOERROR)
			ret = _Copy_wrs_arr(num, pVal, pData);
		return ret;
	}
	//--------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////
	// Функции извлечения данных из структуры
	////////////////////////////////////////////////////////////////////////////
	//--------------------------------------------------------------------------
	// Данные - текст
	long Extract_txt(CString &str)
	{
		return _Extract_txt(str, pData);
	}
	//--------------------------------------------------------------------------
	// Данные - числа, преобразованные к double. Для совместимости с UNIT-1
	long Extract_unit1(double &val)
	{
		return _Extract_unit1(val, pData);
	}
	//--------------------------------------------------------------------------
	// Данные - число
	template <class T>
	long Extract_val(T &val)
	{
		return _Extract_val(val, pData);
	}
	//--------------------------------------------------------------------------
	// Данные - вектор чисел
	template <class T>
	long Extract_arr(std::vector<T> *pVector)
	{
		return _Extract_arr(pVector, pData);
	}
	//--------------------------------------------------------------------------
	// Широкий результат типа число
	template <class T>
	long Extract_wrs_val(WideResult<T> *pVal)
	{
		return _Extract_wrs_val(pVal, pData);
	}
	//--------------------------------------------------------------------------
	// Широкий результат типа вектор чисел
	template <class T>
	long Extract_wrs_arr(WideResult<std::vector<T>> *pVal)
	{
		return _Extract_wrs_arr(pVal, pData);
	}
	//--------------------------------------------------------------------------
	// Произвольная структура
	long Extract_str(PVOID pointer)
	{
		return _Extract_str(pointer, pData);
	}
};
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
//		Структура для хранения данных в динамическом массиве
// sizeof(Unit2DataStruct) = sizeof(Unit2Data) +	// 5 * 4 байтов
//							 sizeof(BYTE *);		// 4
//		Итого 24 байта !!!
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
struct Unit2DataStruct : Unit2Data
{
	BYTE *pData;			// указатель на область памяти для данных
	//--------------------------------------------------------------------------
	// Конструктор
	Unit2DataStruct()
	{
		pData = NULL;
	}
	//--------------------------------------------------------------------------
	// Вспомогательная функция проверки размера области памяти
	long TestSize(const DWORD newSize)
	{// Проверка и увеличение (при необходимости) размера области памяти
		long ret;
		if (newSize > sizeAreaInByte)
		{// Размер памяти надо увеличивать, но вначале удалить
			if (pData)
			{
				delete [] pData;
				pData = NULL;
			}
			try
			{
				pData = new BYTE [(size_t)newSize];
				sizeAreaInByte = newSize;
				ret = NOERROR;
			}
			catch (CMemoryException*)
			{
				sizeAreaInByte = 0;
				ret = -ErrCommandNew;				
			}
		}
		else ret = NOERROR;
		return ret;
	}
	//--------------------------------------------------------------------------
	// Конструктор копирования
	Unit2DataStruct(const Unit2DataStruct &uds)
	{
		pData = NULL;
		sizeAreaInByte = 0;
		TestSize(uds.sizeAreaInByte);
		Copy(&uds, uds.pData, this, pData);
	}
	//--------------------------------------------------------------------------
	// Деструктор
	~Unit2DataStruct()
	{
		if (pData)
		{
			delete [] pData;
			pData = NULL;
		}
	}
	//--------------------------------------------------------------------------
	// Оператор "="
	long operator = (const Unit2DataStruct &uds)
	{
		long ret = TestSize(uds.sizeDataInByte);
		if (ret == NOERROR)
			ret = Copy(&uds, uds.pData, this, pData);
		return ret;
	}
	//--------------------------------------------------------------------------
	// Оператор "="
	long operator = (const Unit2DataStatic &uds)
	{
		long ret = TestSize(uds.sizeDataInByte);
		if (ret == NOERROR)
			ret = Copy(&uds, uds.pData, this, pData);
		return ret;
	}
	//--------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////
	//		Функции копирования данных в структуру.
	// Перед вызовом этих функций надо проверять размер и указатель pData.
	////////////////////////////////////////////////////////////////////////////
	//--------------------------------------------------------------------------
	// Данные - текст
	long Copy_txt(const long num, LPCTSTR text)
	{
		long ret;
		if (pData != NULL)
		{
			ret = TestSize( (DWORD) ( (size_t)(((CString)text).GetLength() + 1) * 
				sizeof(wchar_t) ) );
			if (ret == NOERROR)
				ret = _Copy_txt(num, text, pData);
		}
		else
			ret = -ErrNullPointer;
		return ret;
	}
	//--------------------------------------------------------------------------
	// Произвольная структура
	long Copy_str(const long num, DWORD size, PVOID pointer)
	{
		long ret;
		if (pData != NULL)
		{
			ret = TestSize(size);
			if (ret == NOERROR)
				ret = _Copy_str(num, size, pointer, pData);
		}
		else
			ret = -ErrNullPointer;
		return ret;
	}
	//--------------------------------------------------------------------------
	// Данные - число
	template <class T>
	long Copy_val(const long num, const T &val)
	{
		long ret;
		if (pData != NULL)
		{
			ret = TestSize( sizeof(T) );
			if (ret == NOERROR)
				ret = _Copy_val(num, val, pData);
		}
		else
			ret = -ErrNullPointer;
		return ret;
	}
	//--------------------------------------------------------------------------
	// Данные - вектор чисел
	template <class T>
	long Copy_arr(const long num, const std::vector<T> *pVector)
	{
		long ret;
		if (pData != NULL)
		{
			ret = TestSize((DWORD)( sizeof(T) * pVector->size() ) );
			if (ret == NOERROR)
				ret =  _Copy_arr(num, pVector, pData);
		}
		else
			ret = -ErrNullPointer;
		return ret;
	}
	//--------------------------------------------------------------------------
	// Широкий результат типа число
	template <class T>
	long Copy_wrs_val(const long num, const WideResult<T> *pVal)
	{
		long ret;
		if (pData != NULL)
		{
			ret = TestSize(sizeof( WideResult<T> ) );
			if (ret == NOERROR)
				ret = _Copy_wrs_val(num, pVal, pData);
		}
		else
			ret = -ErrNullPointer;
		return ret;
	}
	//--------------------------------------------------------------------------
	// Широкий результат типа вектор чисел
	template <class T>
	long Copy_wrs_arr(const long num, const WideResult<std::vector<T>> *pVal)
	{
		long ret;
		if (pData != NULL)
		{
			ret = TestSize( sizeof(T) * pVector->size() +
							sizeof(long) + sizeof(double) );
			if (ret == NOERROR)
				ret = _Copy_wrs_arr(num, pVal, pData);
		}
		else
			ret = -ErrNullPointer;
		return ret;
	}
	//--------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////
	//			Функции извлечения данных из структуры.
	// Перед вызовом этих функций надо проверять указатель pData.
	////////////////////////////////////////////////////////////////////////////
	//--------------------------------------------------------------------------
	// Данные - текст
	long Extract_txt(CString &str)
	{
		long ret;
		if (pData != NULL)
			ret = _Extract_txt(str, pData);
		else
			ret = -ErrNullPointer;
		return ret;
	}
	//--------------------------------------------------------------------------
	// Данные - числа, преобразованные к double. Для совместимости с UNIT-1
	long Extract_unit1(double &val)
	{
		long ret;
		if (pData != NULL)
			ret = _Extract_unit1(val, pData);
		else
			ret = -ErrNullPointer;
		return ret;
	}
	//--------------------------------------------------------------------------
	// Данные - число
	template <class T>
	long Extract_val(T &val)
	{
		long ret;
		if (pData != NULL)
			ret = _Extract_val(val, pData);
		else
			ret = -ErrNullPointer;
		return ret;
	}
	//--------------------------------------------------------------------------
	// Данные - вектор чисел
	template <class T>
	long Extract_arr(std::vector<T> *pVector)
	{
		long ret;
		if (pData != NULL)
			ret = _Extract_arr(pVector, pData);
		else
			ret = -ErrNullPointer;
		return ret;
	}
	//--------------------------------------------------------------------------
	// Широкий результат типа число
	template <class T>
	long Extract_wrs_val(WideResult<T> *pVal)
	{
		long ret;
		if (pData != NULL)
			ret = _Extract_wrs_val(pVal, pData);
		else
			ret = -ErrNullPointer;
		return ret;
	}
	//--------------------------------------------------------------------------
	// Широкий результат типа вектор чисел
	template <class T>
	long Extract_wrs_arr(WideResult<std::vector<T>> *pVal)
	{
		long ret;
		if (pData != NULL)
			ret = _Extract_wrs_arr(pVal, pData);
		else
			ret = -ErrNullPointer;
		return ret;
	}
	//--------------------------------------------------------------------------
	// Произвольная структура
	long Extract_str(PVOID pointer)
	{
		long ret;
		if (pData != NULL)
			ret = _Extract_str(pointer, pData);
		else
			ret = -ErrNullPointer;
		return ret;
	}
};
//------------------------------------------------------------------------------