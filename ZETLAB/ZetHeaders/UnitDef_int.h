//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
enum Unit2DataType
{
	tdu_No,				// ��� ������
	tdu_busy,			// ������

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

	tdu_str				// �������� ������������ ��������
};
//------------------------------------------------------------------------------
enum Unit2Error
{
	// ������ �������������
	ErrCreateGUID = 1,			// ������ ��� �������� GUID
	ErrCreateFileMapping,		// ������ ��� �������� FileMapping
	ErrCreateMapView,			// ������ ��� ����������� � FileMapping
	ErrCreateEventReadParam,	// ������ ��� �������� EventReadParam
	ErrCreateEventReadResult,	// ������ ��� �������� EventReadResult
	ErrCreateEventEnd,			// ������ ��� �������� EventEnd	
	ErrCreateServer,			// ������ ��� ������� ���������� ��������

	ErrActivateServer,			// ������ ��� ���������� �������� Activate
	ErrServerNotUnit2,			// ������ ������� �� ��� UNIT-2
	ErrInCommandLine,			// � ��������� ������ ������� ��� GUID 

	// ������ ��������
	ErrServerIsLost = 20,		// ��� �������� ���� �������
	ErrServerIsHung,			// ���������� ������� �������
	ErrClientIsLost,			// ��� �������� ���� �������
	ErrClientIsHung,			// ���������� ������� �������

	// ������ ������/������
	ErrStaticAreaIsSmall = 30,
	ErrDynamicAreaIsSmall,
	ErrCopyData,				// ������ ��� �����������
	ErrReSizeVector,

	ErrWriteResult,				// ������ ��� ������ �����������
	ErrWriteParam,				// ������ ��� ������ ����������
	ErrReadResult,				// ������ ��� ������ �����������
	ErrReadParam,				// ������ ��� ������ ����������

	ErrReadyWriteResult,		// ��� ���������� ������ �����������
	ErrReadyWriteParam,			// ��� ���������� ������ ����������

	// ������ �����������/����������
	ErrSizeListResult = 40,		// �������������� ���-�� �����������
	ErrSizeListParam,			// �������������� ���-�� ����������
	ErrListParamIsEmpty,		// �������� ���������� ����
	ErrListResultIsEmpty,		// �������� ����������� ����
	ErrNoDataThisType,			// ��� ������ ������� ����

	// ����� ������
	ErrZeroLengthStr = 50,		// ������� ����� ������
	ErrNullPointerForType,		// ������� ��������� ��� ����
	ErrNullPointerForNumber,	// ������� ��������� ��� ������
	ErrNullPointerForValue,		// ������� ��������� ��� ��������
	ErrNullPointerForSize,		// ������� ��������� ��� �������
	ErrCommandNew,
	
	ErrNullPointer = 60,		// ������� ���������
	ErrNullValue,				// ������� ��������
	ErrNotNullValue,			// �� ������� ��������
};
//------------------------------------------------------------------------------
template <class T>
struct WideResult
{
	double	time;
	long	quality;
	T		value;

	WideResult()
	{
		time = 0.;
		quality = 0;
	}
};
//------------------------------------------------------------------------------
// ������� ������ ��� ������ ����� ���� ����������� ��� ������������.
//------------------------------------------------------------------------------
// ��� ���� ��������� ������� <class T> - ��� long, float ��� double
//------------------------------------------------------------------------------
struct Unit2DataStruct
{// ���������, ���������� �������� ������ (���������� � �����������) Unit-2
	Unit2DataType type;		// ��� ������
	long number;			// ����� (���) ������
	BOOL dynamicArea;		// ������������ ������� ������ (��� �����������)
	DWORD sizeArray;		// ������ ������� (���� ������ - ������)
	DWORD sizeDataInByte;	// ������ ������ (������� ������ � ���������) � ������
	DWORD sizeAreaInByte;	// ������ ������� ������ � ������
	BYTE *pData;			// ��������� �� ������� ������ ��� ������
	//--------------------------------------------------------------------------
	// �����������
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
	// ����������
	~Unit2DataStruct()
	{
		if (pData && (dynamicArea != FALSE))
		{
			delete [] pData;
			pData = NULL;
		}
	}
	//--------------------------------------------------------------------------
	// ��������������� ������� �������� ������� ������� ������
	long TestSize(const DWORD newSize)
	{// �������� � ���������� (��� �������������) ������� ������� ������
		long ret;
		if (sizeAreaInByte >= newSize)
			ret = NOERROR;
		else
		{
			if (dynamicArea != FALSE)
			{// ������������ ������� ������, ����� �����������
			//	�� ������� �������
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
			else	// ����������� ������, ����������� ������, ������
				ret = -ErrStaticAreaIsSmall;
		}
		return ret;
	}
	//--------------------------------------------------------------------------
	// ��������������� ������� ����������� ������ ���������� ������
	void CopyDescriptionOnly(const Unit2DataStruct &uds)
	{
		type = uds.type;					// ��� ������
		number = uds.number;				// ����� ������
		sizeArray = uds.sizeArray;			// ������ �������
		sizeDataInByte = uds.sizeDataInByte;// ������ ������ � ������
		sizeAreaInByte = uds.sizeAreaInByte;// ������ ������� ������ � ������
	}
	//--------------------------------------------------------------------------
	// �������� "="
	long operator = (const Unit2DataStruct &uds)
	{
		long ret;
		if (uds.type != tdu_No)
		{//������ ����
			if (uds.pData != NULL)
			{//������ ����
				ret = TestSize(uds.sizeDataInByte);
				if (ret == NOERROR)
				{// ����� ����������
					if ( memcpy_s((PVOID)pData, uds.sizeDataInByte,
									(PVOID)uds.pData, uds.sizeDataInByte) 
						== NOERROR )
					{// ������� �����������
						type = uds.type;
						number = uds.number;
						sizeArray = uds.sizeArray;
						sizeDataInByte = uds.sizeDataInByte;
						ret = NOERROR;
					}
					else
					{// �� ������� �����������
						type = tdu_No;
						sizeArray = sizeDataInByte = 0;
						ret = -ErrCopyData;
					}
				}
				else
				{// ���������� ������ (���� �����), ret ��� ����������
					type = tdu_No;
					sizeArray = sizeDataInByte = 0;
				}
			} 
			else
			{// ������ ���, ������� ��������� ��� ���������, ���� ������
				type = tdu_No;
				sizeArray = sizeDataInByte = 0;
				ret = -ErrNullPointer;
			}
		}
		else
		{// ������ ���, ������� ��������� ��� ����, ������ ���
			type = tdu_No;
			sizeArray = sizeDataInByte = 0;
			ret = NOERROR;
		}
		return ret;
	}
	////////////////////////////////////////////////////////////////////////////
	// ������� ����������� ������ � ���������
	////////////////////////////////////////////////////////////////////////////
	// ������ - �����
	long Copy_txt(const long num, LPCTSTR text)
	{
		long ret;
		sizeArray = ((CString)text).GetLength() + 1;
		sizeDataInByte = sizeArray * sizeof(wchar_t);
		ret = TestSize(sizeDataInByte);
		if (ret == NOERROR)
		{// ����� ����������
			if ( wcscpy_s( (wchar_t*)pData, sizeArray, text ) == NOERROR )
			{// ����������� �������, ������ ���
				type = tdu_txt;
				number = num;
			}
			else
			{// ����������� �� �������, ������
				type = tdu_No;
				sizeArray = sizeDataInByte = 0;
				ret = -ErrCopyData;
			}
		}
		else
		{// ������ �� ��������� ����������, ������, ret ��� ��������
			type = tdu_No;
			sizeArray = sizeDataInByte = 0;
		}
		return ret;
	}
	//--------------------------------------------------------------------------
	// ������ - �����
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
		{// ��� ������ ��������
			sizeArray = 1;
			sizeDataInByte = sizeof(T);
			ret = TestSize(sizeDataInByte);
			if (ret == NOERROR)
			{// ����� ����������
				number = num;
				*((T*)pData) = val;
			}
			else
			{// ������ �� ��������� ����������, ������, ret ��� ��������
				type = tdu_No;
				sizeArray = sizeDataInByte = 0;
			}
		} 
		else	// ������������ ��� ������, ������
			ret = -ErrNoDataThisType;
		return ret;
	}
	//--------------------------------------------------------------------------
	// ������ - ������ �����
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
		{// ���������� ��� ������
			if (pArray != NULL)
			{
				sizeArray = size_Array;
				sizeDataInByte = sizeof(T) * sizeArray;
				ret = TestSize(sizeDataInByte);
				if (ret == NOERROR)
				{// ����� ����������
					if (memcpy_s((PVOID)pData, sizeDataInByte,
								 (PVOID)pArray, sizeDataInByte) == 0)
						// ����������� �������, ������ ���, ret ��� NOERROR
						number = num;
					else
					{// ����������� �� �������, ������
						type = tdu_No;
						sizeArray = sizeDataInByte = 0;
						ret = -ErrCopyData;
					}
				}
				else
				{// ������ �� ��������� ����������, ������, ret ��� ��������
					type = tdu_No;
					sizeArray = sizeDataInByte = 0;
				}
			}
			else// ������� ���������
				ret = -ErrNullPointer;
		}
		else	// ������������ ��� ������, ������
			ret = -ErrNoDataThisType;
		return ret;
	}
	//--------------------------------------------------------------------------
	// ������� ��������� ���� �����
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
		{// ���������� ��� ������
			if (pVal != NULL)
			{
				sizeArray = 1;
				sizeDataInByte = sizeof(WideResult<T>);
				ret = TestSize(sizeDataInByte);
				if (ret == NOERROR)
				{// ����� ����������
					*((WideResult<T>*)pData) = *pVal;
					number = num;
				}
				else
				{// ��������� NULL, ���������� ������, ������
					type = tdu_No;
					sizeArray = sizeDataInByte = 0;
					ret = -ErrNullPointer;
				}
			}
			else// ������� ���������
					ret = -ErrNullPointer;
		} 
		else	// ������������ ��� ������, ������
			ret = -ErrNoDataThisType;
		return ret;
	}
	//--------------------------------------------------------------------------
	// ������� ��������� ���� ������ �����
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
		{// ���������� ��� ������
			if ((pVal != NULL) && (pVal->value != NULL))
			{
				sizeArray = size_Array;
				sizeDataInByte = sizeof(double) + sizeof(long) +
					sizeof(T) * sizeArray;
				ret = TestSize(sizeDataInByte);
				if (ret == NOERROR)
				{// ����� ����������
					BYTE *add = pData;
					*((double*)add) = pVal->time;
					add += sizeof(double);
					*((long*)add) = pVal->quality;
					add += sizeof(long);
					DWORD size = sizeArray * sizeof(T);
					if (memcpy_s((PVOID)add, size,
						(PVOID)pVal->value, size) == 0)
						// ����������� �������, ������ ���, ret ��� NOERROR
						number = num;
					else
					{// ����������� �� �������, ������
						type = tdu_No;
						sizeArray = sizeDataInByte = 0;
						ret = -ErrCopyData;
					}
				}
				else
				{// ������ �� ��������� ����������, ������, ret ��� ��������
					type = tdu_No;
					sizeArray = sizeDataInByte = 0;
				}
			}
			else
				ret = -ErrNullPointer;
		} 
		else	// ������������ ��� ������, ������
			ret = -ErrNoDataThisType;
		return ret;
	}
	////////////////////////////////////////////////////////////////////////////
	// ������� ���������� ������ �� ���������
	////////////////////////////////////////////////////////////////////////////
	// ������ - �����
	long Extract_txt(CString &str)
	{
		long ret;
		if (type == tdu_txt)
		{// ���������� ��� ������
			str = (CString)(wchar_t*)pData;
			ret = NOERROR;
		}
		else	// �� ���������� ��� ������, ������
			ret = -ErrNoDataThisType;
		return ret;
	}
	//--------------------------------------------------------------------------
	// ������ - �����, ��������������� � double. ��� ������������� � UNIT-1
	long Extract_unit1(double &val)
	{
		long ret = NOERROR;
		if (type == tdu_32s)
			val = double(*((long*)pData));

		else if (type == tdu_32f)
			val = double(*((float*)pData));

		else if (type == tdu_64f)
			val = *((double*)pData);

		else	// �� ���������� ��� ������, ������
			ret = -ErrNoDataThisType;
		return ret;
	}
	//--------------------------------------------------------------------------
	// ������ - �����
	template <class T>
	long Extract_val(T &val)
	{
		long ret;
		if ( (type == tdu_32s) || (type == tdu_32f) ||
			 (type == tdu_64f) )
		{// ���������� ��� ������
			val = *((T*)pData);
			ret = NOERROR;
		}
		else	// �� ���������� ��� ������, ������
			ret = -ErrNoDataThisType;
		return ret;
	}
	//--------------------------------------------------------------------------
	// ������ - ������ �����
	template <class T>
	long Extract_arr(T *pArray)
	{
		long ret;
		if ((type == tdu_32s_arr) || (type == tdu_32f_arr) ||
			(type == tdu_64f_arr))
		{// ���������� ��� ������
			if (pArray != NULL)
			{// ����� ����������
				try
				{
					if (memcpy_s((PVOID)pArray, sizeDataInByte,
								 (PVOID)pData, sizeDataInByte) == 0)
						ret = NOERROR;	// ����������� �������, ������ ���
					else	// ��������� �����������, ������
						ret = -ErrCopyData;
				}
				catch (CMemoryException*)
				{
					ret = -ErrCopyData;
				}
			}
			else	// ������� �������, ������
				ret = -ErrNullPointer;
		}
		else	// �� ���������� ��� ������, ������
			ret = -ErrNoDataThisType;
		return ret;
	}
	//--------------------------------------------------------------------------
	// ������� ��������� ���� �����
	template <class T>
	long Extract_wrs_val(WideResult<T> *pVal)
	{
		long ret;
		if ( (type == tdu_wrs_32s) || (type == tdu_wrs_32f) ||
			(type == tdu_wrs_64f) )
		{// ���������� ��� ������
			if (pVal != NULL)
			{
				*pVal = *((WideResult<T>*)pData);
				ret = NOERROR;
			}
			else	// ������� �������, ������
				ret = -ErrNullPointer;
		}
		else	// �� ���������� ��� ������, ������
			ret = -ErrNoDataThisType;
		return ret;
	}
	//--------------------------------------------------------------------------
	// ������� ��������� ���� ������ �����
	template <class T>
	long Extract_wrs_arr(WideResult<T*> *pVal)
	{
		long ret;
		if ( (type == tdu_wrs_32s_arr) || (type == tdu_wrs_32f_arr) ||
			 (type == tdu_wrs_64f_arr) )
		{// ���������� ��� ������
			if ((pVal != NULL) && (pVal->value != NULL) )
			{
				BYTE *add = pData;
				pVal->time = *((double*)add);	// �����
				add += sizeof(double);
				pVal->quality = *((long*)add);	// ��������
				add += sizeof(long);
				// ������ ����� � ������
				DWORD size = sizeArray * sizeof(T);
				try
				{
					if (memcpy_s((PVOID)pVal->value, size, (PVOID)add, size) == 0)
						ret = NOERROR;	// ����������� �������, ������ ���
					else   // ��������� �����������, ������
						ret = -ErrCopyData;
				}
				catch (CMemoryException*)
				{
					ret = -ErrCopyData;					
				}
			}
			else	// ������� �������, ������
				ret = -ErrNullPointer;
		}
		else	// �� ���������� ��� ������, ������
			ret = -ErrNoDataThisType;
		return ret;
	}
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------
	// ������ ������� ��������� ��� type == tdu_str
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
	// ������������ ���������
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