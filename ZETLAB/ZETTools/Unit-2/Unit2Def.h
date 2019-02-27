//------------------------------------------------------------------------------
#pragma once
#pragma once
#include <vector>
#include "ZetDef.h"
#include "WideResult\WideResult.h"
//------------------------------------------------------------------------------
#define Wait_TestHung_mSec		1000	// ����� �������� ��� ������������, 1 ���
#define Timer_Interval_mSec		3000	// �������� �������, 3 ���
#define Wait_Max_mSec			10000	// ����� ������������� ��������, 10 ���
//------------------------------------------------------------------------------
enum Unit2DataType
{// ���� ������ (����������� � ����������) Unit-2.
	// �������� ������� ������� ������! ����� ���� ��������� ������ � �����!
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

	tdu_str				// ������������ ���������
};
//------------------------------------------------------------------------------
enum Unit2Error
{// ���� ������ � Unit-2.1
	// �������� �������� ������ ���� ��������������, ������� ������������ ���� ���:
	//	long ret = -ErrNewGUID;

	//	NOERROR = 0					// ������ ���, ���������� � WinError.h
	// ������ ������ ��������
	ErrFindServerExe = 1,
	ErrResource_PARAM_SIZE_MAX,
	ErrResource_RESULT_SIZE_MAX,
	ErrResource_PARAM_TYPE,
	ErrResource_RESULT_TYPE,

	// ������ �������������
	ErrCreateGUID = 10,			// ������ ��� �������� GUID
	ErrCreateFileMapping,		// ������ ��� �������� FileMapping
	ErrCreateMapView,			// ������ ��� ����������� � FileMapping
	ErrCreateEventReadParam,	// ������ ��� �������� EventReadParam
	ErrCreateEventReadResult,	// ������ ��� �������� EventReadResult
	ErrCreateEventEnd,			// ������ ��� �������� EventEnd	
	ErrCreateServer,			// ������ ��� ������� ���������� ��������

	// ������ ��������
	ErrActivateServer = 30,			// ������ ��� ���������� �������� Activate
	ErrServerNotUnit2,			// ������ ������� �� ��� UNIT-2
	ErrInCommandLine,			// � ��������� ������ ������� ��� GUID 

	ErrServerIsLost,		// ��� �������� ���� �������
	ErrServerIsHung,			// ���������� ������� �������
	ErrClientIsLost,			// ��� �������� ���� �������
	ErrClientIsHung,			// ���������� ������� �������

	// ������ ������/������
	ErrSizeAreaIsSmall = 40,
	ErrStaticAreaIsSmall,
	ErrDynamicAreaIsSmall,
	ErrCopyData,				// ������ ��� �����������
	ErrCopyText,
	ErrReSizeVector,

	ErrWriteResult = 50,		// ������ ��� ������ �����������
	ErrWriteParam,				// ������ ��� ������ ����������
	ErrReadResult,				// ������ ��� ������ �����������
	ErrReadParam,				// ������ ��� ������ ����������
	ErrNoParamWithThisNumber,	// ��� ��������� � ����� �������
	ErrTypeParam,				// �������� ��� ���������

	ErrReadyWriteResult,		// ��� ���������� ������ �����������
//	ErrReadyReadResult,			// ��� ���������� ������ �����������
	ErrReadyWriteParam,			// ��� ���������� ������ ����������
//	ErrReadyReadParam,			// ��� ���������� ������ ����������

	// ������ �����������/����������
	ErrSizeListResult = 60,		// �������������� ���-�� �����������
	ErrSizeListParam,			// �������������� ���-�� ����������
	ErrListParamIsEmpty,		// �������� ���������� ����
	ErrListResultIsEmpty,		// �������� ����������� ����
	ErrNoDataThisType,			// ��� ������ ������� ����
	ErrNoParamWithNumber,		// ��� ���������� � ����� �������

	// ����� ������
	ErrZeroLengthStr = 100,		// ������� ����� ������
	ErrNullPointerForType,		// ������� ��������� ��� ����
	ErrNullPointerForNumber,	// ������� ��������� ��� ������
	ErrNullPointerForValue,		// ������� ��������� ��� ��������
	ErrNullPointerForSize,		// ������� ��������� ��� �������
	ErrCommandNew,				// ������ ��� ��������� ������ �������� new

	ErrNullPointer = 110,		// ������� ���������
	ErrNullValue,				// ������� ��������
	ErrNotNullValue,			// �� ������� ��������
};
//------------------------------------------------------------------------------