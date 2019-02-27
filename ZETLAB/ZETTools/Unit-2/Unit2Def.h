//------------------------------------------------------------------------------
#pragma once
#pragma once
#include <vector>
#include "ZetDef.h"
#include "WideResult\WideResult.h"
//------------------------------------------------------------------------------
#define Wait_TestHung_mSec		1000	// время ожидания при тестировании, 1 сек
#define Timer_Interval_mSec		3000	// интервал таймера, 3 сек
#define Wait_Max_mSec			10000	// время максимального ожидания, 10 сек
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

	tdu_str				// произвольная структура
};
//------------------------------------------------------------------------------
enum Unit2Error
{// Коды ошибок в Unit-2.1
	// Реальные значения должны быть отрицательными, поэтому использовать надо так:
	//	long ret = -ErrNewGUID;

	//	NOERROR = 0					// ошибок нет, определено в WinError.h
	// ошибки чтения ресурсов
	ErrFindServerExe = 1,
	ErrResource_PARAM_SIZE_MAX,
	ErrResource_RESULT_SIZE_MAX,
	ErrResource_PARAM_TYPE,
	ErrResource_RESULT_TYPE,

	// ошибки инициализации
	ErrCreateGUID = 10,			// ошибка при создании GUID
	ErrCreateFileMapping,		// ошибка при создании FileMapping
	ErrCreateMapView,			// ошибка при подключении к FileMapping
	ErrCreateEventReadParam,	// ошибка при создании EventReadParam
	ErrCreateEventReadResult,	// ошибка при создании EventReadResult
	ErrCreateEventEnd,			// ошибка при создании EventEnd	
	ErrCreateServer,			// ошибка при запуске серверного процесса

	// ошибки программ
	ErrActivateServer = 30,			// ошибка при выполнении сервером Activate
	ErrServerNotUnit2,			// сервер запущен не под UNIT-2
	ErrInCommandLine,			// в командной строке сервера нет GUID 

	ErrServerIsLost,		// нет главного окна сервера
	ErrServerIsHung,			// приложение сервера зависло
	ErrClientIsLost,			// нет главного окна клиента
	ErrClientIsHung,			// приложение клиента зависло

	// ошибки чтения/записи
	ErrSizeAreaIsSmall = 40,
	ErrStaticAreaIsSmall,
	ErrDynamicAreaIsSmall,
	ErrCopyData,				// ошибка при копировании
	ErrCopyText,
	ErrReSizeVector,

	ErrWriteResult = 50,		// ошибка при записи результатов
	ErrWriteParam,				// ошибка при записи параметров
	ErrReadResult,				// ошибка при чтении результатов
	ErrReadParam,				// ошибка при чтении параметров
	ErrNoParamWithThisNumber,	// нет параметра с таким номером
	ErrTypeParam,				// неверный тип параметра

	ErrReadyWriteResult,		// нет готовности записи результатов
//	ErrReadyReadResult,			// нет готовности чтения результатов
	ErrReadyWriteParam,			// нет готовности записи параметров
//	ErrReadyReadParam,			// нет готовности чтения параметров

	// ошибки результатов/параметров
	ErrSizeListResult = 60,		// несоответствие кол-ва результатов
	ErrSizeListParam,			// несоответствие кол-ва параметров
	ErrListParamIsEmpty,		// перечень параметров пуст
	ErrListResultIsEmpty,		// перечень результатов пуст
	ErrNoDataThisType,			// нет данных данного типа
	ErrNoParamWithNumber,		// нет параметров с таким номером

	// общие ошибки
	ErrZeroLengthStr = 100,		// нулевая длина строки
	ErrNullPointerForType,		// нулевой указатель для типа
	ErrNullPointerForNumber,	// нулевой указатель для номера
	ErrNullPointerForValue,		// нулевой указатель для значения
	ErrNullPointerForSize,		// нулевой указатель для размера
	ErrCommandNew,				// ошибка при выделении памяти командой new

	ErrNullPointer = 110,		// нулевой указатель
	ErrNullValue,				// нулевое значение
	ErrNotNullValue,			// не нулевое значение
};
//------------------------------------------------------------------------------