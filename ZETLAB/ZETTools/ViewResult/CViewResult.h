//------------------------------------------------------------------------------
#pragma once
#include "afxwin.h"
#include <ZetViewResult\include\ZetViewResult.h>
#include "StructsService.h"
//------------------------------------------------------------------------------
#define  SIZE_RESULT (SIZE_STRING_FOR_VALUE + SIZE_NAME_UNIT)
//------------------------------------------------------------------------------
// Оболочка над структурой отображения STRUCT_VIEW_RESALT  и
// функциями библиотеки ZetViewResul.dll
//
// Класс CViewResultParam предназначен для решения задач отображения только
// статистически значимых разрядов результата, у которого единица измрения
// не меняется.
//
// Класс CViewResultTransform предназначен для решения задач отображения только
// статистически значимых разрядов результата, у которого единица измрения
// может меняется в слудующих случаях:
// - изменение самой единицы, например: Па <=> атм;
// - измернение кратной/дольной единицы, например: мВ <=> В <=> кВ.
//
//------------------------------------------------------------------------------
class CViewResultParam
{
private:
protected:
	DWORD m_numAver;	// кол-во отсчётов исходного сигнала при усреднении
	LPCHANNEL_STRUCT pChanStructInit;

public:
	STRUCT_VIEW_RESALT m_param;		// Структура параметров отображения

	CViewResultParam();
	~CViewResultParam() {}
	CViewResultParam(const CViewResultParam & val);
	CViewResultParam & operator = (const CViewResultParam & val);
	
	// Инициализация структуры отображения по структуре параметров канала
	bool InitParam(LPCHANNEL_STRUCT par, const DWORD numAver, bool bBindingPoint, BYTE indxPointExt = 0);

	// Новое значение
	virtual bool NewValue(const float newVal);

	// Перезапуск
	virtual void ReStart();

	// Определение типа привязки (есть/нет) положения десятичной "точки"
	bool GetBindingPoint();

	// Задание типа привязки (есть/нет) положения десятичной "точки"
	void SetBindingPoint(const bool bOnOff);
	
	// Получение внутреннего индекса (счёт с 0) положения десятичной "точки",
	// рассчитанного самой dll
	BYTE GetIndxPointSelf();

	// Получение внешнего индекса (счёт с 0) положения десятичной "точки",
	// заданного извне
	BYTE GetIndxPointExtern();

	// Задание внешнего индекса (счёт с 0) положения десятичной "точки"
	void SetIndxPointExtern(BYTE indx);

};
//------------------------------------------------------------------------------
class CViewResultTransform : public CViewResultParam
{
private:
protected:
	double m_k;		// коэффициент единицы измерения результата
	STRUCT_UNIT_CONVERT_SERVICE m_convert;	// параметры преобразования единицы

public:

	CViewResultTransform(LPCHANNEL_STRUCT pChanStruct = nullptr);
	~CViewResultTransform() {}
	CViewResultTransform(const CViewResultTransform & val);
	CViewResultTransform & operator = (const CViewResultTransform & val);

	CHANNEL_STRUCT m_сhanStructView;		// параметры канала отображения

	// Инициализация структуры отображения по структуре параметров канала
	bool InitParamView(LPCHANNEL_STRUCT par, const DWORD numAver, bool bBindingPoint, BYTE indxPointExt = 0);

	// Задание типа преобразования единицы измерения
	bool InitTransform(
		const TypeTransformUnit ttu,	// тип преобразования единицы
		LPCWCH lpwMeasSrc,				// размерность исходного канала
		LPCWCH lpwMeasDst);				// размерность выходного канала

	// Изменение единицы измерения канала отображения
	bool ChangeUnit(
		const TypeTransformUnit ttu,	// тип преобразования единицы
		LPCWCH lpwMeasDst);				// размерность выходного канала

	virtual void ReStart();

	// Новое значение
	virtual bool NewValue(const float newVal, bool & bEventPreUnit);
};
//------------------------------------------------------------------------------