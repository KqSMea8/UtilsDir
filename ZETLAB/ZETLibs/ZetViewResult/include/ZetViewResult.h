//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#ifdef ZETVIEWRESULT_EXPORTS
#define VIEWRESULT_API __declspec(dllexport)
#else
#define VIEWRESULT_API __declspec(dllimport)
#endif
//-----------------------------------------------------------------------------
#include <ZetErrorCodes.h>					// коды ошибок
#include <chaninfo.h>						// CHANNEL_STRUCT
#include <ViewResult\StructViewResult.h>	// STRUCT_VIEW_RESALT
#include <ViewResult\TypeTransform.h>		// TypeTransform...
#include <ViewResult\StructUnit.h>			// структуры единиц измерения
//-----------------------------------------------------------------------------
#define DEFAULT_VALUE_ALFA 0.5f

#define FLAG_START_ON	0x1
#define FLAG_POINT_ON	0x2
#define FLAG_INIT_ON	0x4
//-----------------------------------------------------------------------------
// Инициализация структуры для отображения значений какого-либо канала.
// pChanPar - указатель на структуру параметров канала;
// numberAver - кол-во отсчётов, учавствующих в определении одного значения;
// bindingPoint - требуется выравнивание десятичной точки;
// indxPoint - индекс десятичной точки (если 0, то определяется в DLL)
// pParam - указатель на структуру параметров отображения.
//
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - инициализация выполнена успешно.
_Check_return_ VIEWRESULT_API
long zvInitParam(_In_ LPCHANNEL_STRUCT pChanPar,
_In_ const DWORD numberAver,
_In_ const BYTE bindingPoint,
_In_ const BYTE indxPoint,
_Inout_ STRUCT_VIEW_RESALT * pParam);
//-----------------------------------------------------------------------------
// Задание нового значения и его преобразование в строку. Результат в
// pParam->sValueCur.
//
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - выполнено успешно.
_Check_return_ VIEWRESULT_API
long zvNewValue(
_In_ const float newVal, _Inout_ STRUCT_VIEW_RESALT * pParam);
//-----------------------------------------------------------------------------
// Преобразование структуры для нового потока значений без изменения параметров.
//
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - выполнено успешно.
_Check_return_  VIEWRESULT_API
long zvReStart(_Inout_ STRUCT_VIEW_RESALT * pParam);
//-----------------------------------------------------------------------------
// Задание индекса (счёт от 0) положения десятичной точки.
//
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - выполнено успешно.
_Check_return_  VIEWRESULT_API
long zvSetIndxPoint(_In_ const BYTE indx, _Inout_ STRUCT_VIEW_RESALT * pParam);
//-----------------------------------------------------------------------------
// Задание типа выравнивания (есть/нет) положения десятичной точки
// для структрцры после её инициализации.
// binding - 0 выравнивание не требуется, !0 выравнивание требуется;
// pParam - указатель на структуру параметров отображения.
//
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - инициализация выполнена успешно.
_Check_return_ VIEWRESULT_API
long zvSetBindingPoint(
_In_ const BYTE binding,
_Inout_ STRUCT_VIEW_RESALT * pParam);
//-----------------------------------------------------------------------------
// Преобразование канала
// code - код преобразования
// dt - время усреднения, секунды, или мультипликативный коэфф. преобразования
// df - частотное разрешение или аддитивный коэфф. преобразования
// numPoints - кол-во точек исходного канала, требуемых для получения одного отсчета выходного канала
// lpwNewName - новоое название канала (для ttc_unit и ttc_preUnit) или nullptr
// lpwNewMeasure - новая размерность канала
// pChanDst - структура выходного канала
// pChanSrc - структура исходного канала
//
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - выполнено успешно.
_Check_return_  VIEWRESULT_API
long zvTransformChannel(
	_In_ const TypeTransformChannel code,
	_In_ const double dt,
	_In_ const double df,
	_In_ const uint64_t numPoints,
	_Inout_opt_ LPCWCH lpwNewName,
	_Inout_opt_ LPCWCH lpwNewMeasure,
	_Inout_ LPCHANNEL_STRUCT pChanDst,
	_Inout_ const LPCHANNEL_STRUCT pChanSrc);
//-----------------------------------------------------------------------------
// Преобразование канала
// code - код преобразования
// dt - время усреднения, секунды, или мультипликативный коэфф. преобразования
// df - частотное разрешение или аддитивный коэфф. преобразования
// numPoints - кол-во точек исходного канала, требуемых для получения одного отсчета выходного канала
// lpwNewName - новоое название канала (для ttc_unit и ttc_preUnit) или nullptr
// lpwNewMeasure - новая размерность канала
// pChanDst - структура выходного канала
// pChanSrc - структура исходного канала
//
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - выполнено успешно.
_Check_return_  VIEWRESULT_API
long zvTransformChannel_I(
	_In_ const TypeTransformChannel code,
	_In_ const double dt,
	_In_ const double df,
	_In_ const uint64_t numPoints,
	_Inout_opt_ LPCWCH lpwNewName,
	_Inout_opt_ LPCWCH lpwNewMeasure,
	_Inout_ LPCHANNEL_STRUCT pChanSrcDst);
//-----------------------------------------------------------------------------
// Получение количества групп единиц измерения, поддерживаемых DLL
//
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - выполнено успешно.
_Check_return_  VIEWRESULT_API
long zvGetNumberGroupUnit(_Inout_ DWORD * pNumber);
//-----------------------------------------------------------------------------
// Получение количества типов единиц измерения, поддерживаемых DLL
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - выполнено успешно.
_Check_return_  VIEWRESULT_API
long zvGetNumberTypeUnit(_Inout_ DWORD * pNumber);
//-----------------------------------------------------------------------------
// Получение имени одной группы единиц измерения.
// ndxGroup - индекс группы (от 0)
// lpwBuf - буфер, в который будет копироваться имя
// pSizeBuf - размер этого буфера
//
// Если lpwBuf = nullptr, то в
// pSizeBuf будет записан размер, необходимый для копирования имени этой группы.
//
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - выполнено успешно.
_Check_return_  VIEWRESULT_API
long zvGetNameGroupUnit(
	_In_ DWORD indxGroup,
	_Inout_ LPWCH lpwBuf,
	_Inout_ DWORD * pSizeBuf);
//-----------------------------------------------------------------------------
// Инициализация преобразования единиц:
// - определение исходной единицы (по заданному имени), её поиск среди
//   поддерживаемых данной DLL единиц;
// - определение выходной единицы (по типу преобразования и входной единице
//   или по заданному имени), её поиск среди
//   поддерживаемых данной DLL единиц;
// - определение параметров преобразования единиц.
//
// Параметры функции:
// ttu - тип преобразования;
// lpwBufSrc - наименование входной единицы (обязательно);
// lpwBufDst - наименование выходной единицы (или nullptr или L"");
// psuc - параметры преобразования.
//
// Возвращаемое значение:
// отрицательное значение - код ошибки;
// 0 - выполнено успешно.
_Check_return_  VIEWRESULT_API
long zvInitTransformUnits(
	_In_ const TypeTransformUnit ttu,
	_Inout_ LPCWCH lpwBufSrc,
	_Inout_ LPCWCH lpwBufDst,
	_Inout_ STRUCT_UNIT_CONVERT * psuc);
//-----------------------------------------------------------------------------
// Изменение выходной единицы при преобразовании единиц. Новая выходная
// единица определяется по типу преобразования или по заданному имени.
//
// Параметры функции:
// ttu - тип преобразования;
// lpwBufDstUnit - наименование выходной единицы (или 0);
// psuc - параметры преобразования.
//
// Возвращаемое значение:
// отрицательное значение - код ошибки
// 0 - выполнено успешно.
_Check_return_  VIEWRESULT_API
long zvChangeUnit(
	_In_ const TypeTransformUnit ttu,
	_Inout_ LPCWCH lpwBufDstUnit,
	_Inout_ STRUCT_UNIT_CONVERT * psuc);
//-----------------------------------------------------------------------------
// Реализации изменения выходной единицы после выполнения функций
// zvInitTransformUnits или zvChangeUnit.
//
// Параметры функции:
// psuc - параметры преобразования;
// pChanStruct - параметры выходного канала;
// pViewParam - параметры отображения.
//
// Возвращаемое значение:
// отрицательное значение - код ошибки
// 0 - выполнено успешно.
_Check_return_  VIEWRESULT_API
long zvRealizationUnit(
	_In_ const STRUCT_UNIT_CONVERT * psuc,
	_Inout_ CHANNEL_STRUCT * pChanStruct,
	_Inout_ STRUCT_VIEW_RESALT * pViewParam);
//-----------------------------------------------------------------------------
// Выполнение преобразования значения из одной единицы в другую. И
// определение события о преобразовании кратной/дольной единицы.
//
// Параметры функции:
// valSrc - величина с исходной единицей;
// pValDst - величина с выходной единицей;
// pbEventPreUnit - флаг, что было преобразование кратной/дольной единицы;
// pFactorEventPreUnit - коэффициент преобразования ( = ValDst /valSrc );
// psuc - параметры преобразования.
//
// Если есть уверенность, что структура psuc создана правильно, и преобразование
// кратной/долной единицы не требуется, то можно обойтить без этой функции, написав:
// *pValDst = (float)(psuc->convert.k * valSrc + psuc->convert.a);
//
// Возвращаемое значение:
// отрицательное значение - код ошибки
// 0 - выполнено успешно.
_Check_return_  VIEWRESULT_API
long zvTransformValue(
	_In_ const float valSrc,
	_Inout_ float * pValDst,
	_Inout_ BYTE * pbEventPreUnit,
	_Inout_ double * pFactorEventPreUnit,
	_Inout_ STRUCT_UNIT_CONVERT * psuc);
//-----------------------------------------------------------------------------
// Функция реализации преобразования кратной/дольной единицы после
// выполнения функции zvTransformValue при pbEvent = 1.
//
// Параметры функции:
// psuc - параметры преобразования;
// pChanStruct - параметры выходного канала;
// pViewParam - параметры отображения.
//
// Возвращаемое значение:
// отрицательное значение - код ошибки
// 0 - выполнено успешно.
_Check_return_  VIEWRESULT_API
long zvRealizationPreUnit(
	_In_ const STRUCT_UNIT_CONVERT * psuc,
	_Inout_ CHANNEL_STRUCT * pChanStruct,
	_Inout_ STRUCT_VIEW_RESALT * pViewParam);
//-----------------------------------------------------------------------------
// Копирование усредненного значения результата с размерностью (у значения только
// статистически значимые разряды) во внешний буфер в соответствии с правилами
// ГОСТ 8.417-2002 в части наличия пробела между числом и размерностью.
//
// Параметры функции:
// lpwBuf - буфер для копирования;
// sizeBuf - размер этого буфера;
// psuc - параметры преобразования;
// pViewParam - параметры отображения.
// 
// Возвращаемое значение:
// отрицательное значение - код ошибки
// 0 - выполнено успешно.
_Check_return_  VIEWRESULT_API
long zvCopyStringResult(
	_Inout_ LPWCH lpwBuf,
	_In_ const DWORD sizeBuf,
	_In_ const STRUCT_UNIT_CONVERT * psuc,
	_In_ const STRUCT_VIEW_RESALT * pViewParam);
//-----------------------------------------------------------------------------
// Получение мультипликативного коэффициента преобразования единиц, при
// выполнении преобразования в соответствии с правилами, записанными в файле
// "$ZetConfigDir\ConvertUnits.xml"
// Параметры функции:
// unitSrc - буфер с входной размерностью единицы;
// unitDst - буфер для выходной размерности единицы;
// sizeUnitDst - размер буфера unitDst;
// k - мультипликативный коэффициент преобразования единиц.
//
// Пример кода для случая, когда в ConvertUnits.xml есть строка
// <Units	UnitSrc="МПа"	UnitDst="атм"/>
// -------------------------------------------------
// WCHAR unitSrc[SIZE_NAME_UNIT] = { L"МПа" };
// WCHAR unitDst[SIZE_NAME_UNIT] = { 0 };
// float k(0.f);
// if (0 == zvConvertUnitByXML(unitSrc, unitDst, SIZE_NAME_UNIT, k))
//{		результат:
//		unitDst содержит строку L"атм"
//		k равно 10.1971617
//}
_Check_return_  VIEWRESULT_API
long zvConvertUnitByXML(
	_Inout_ LPCWSTR unitSrc,
	_Inout_ LPWSTR unitDst,
	_In_ const DWORD sizeUnitDst,
	_Out_ float & k);
//-----------------------------------------------------------------------------