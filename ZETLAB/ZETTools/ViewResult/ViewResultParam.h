//------------------------------------------------------------------------------
#pragma once
#include "afxwin.h"
#include <ZetViewResult\include\ZetViewResult.h>
//------------------------------------------------------------------------------
// Оболочка над структурой ViewResultParam и
// функциями библиотеки ZetViewResul.dll
class CViewResultParam
{
private:
protected:

public:
	// Структура параметров отображения
	STRUCT_VIEW_RESALT m_param;

	CViewResultParam();
	~CViewResultParam() {}
	CViewResultParam(const CViewResultParam & val);
	CViewResultParam & operator = (const CViewResultParam & val);
	
	// Инициализация по структуре параметров канала
	bool Init(LPCHANNEL_STRUCT par, const DWORD numAver, bool bBindingPoint = false);

	// Инициализация по структуре, являющейся наследником структуры параметров канала
	bool Init(PVOID par, const DWORD numAver, bool bBindingPoint = false);

	// Новое значение
	void NewValue(const float newVal);

	void ReStart();

	// Задание индекса (счёт с 0) положения десятичной "точки"
	void SetIndxPoint(BYTE indx);

	// Получение индекса (счёт с 0) положения десятичной "точки",
	// рассчитанного самой dll
	BYTE GetIndxPointSelf();

	// Получение индекса (счёт с 0) положения десятичной "точки",
	// заданной извне
	BYTE GetIndxPointExtern();
};
//------------------------------------------------------------------------------