//----------------------------------------------------------------------
// Файл CustomIirFiltr.cpp
// Версия от 27.02.2012
// ЗАО "ЭТМС", Зеленоград, Савостин Ю.М.
//----------------------------------------------------------------------
#include "StdAfx.h"
#include <ippFiltr\CustomIirFiltr.h>
//----------------------------------------------------------------------
CCustomIirFiltr::CCustomIirFiltr()
	: FType(tfNo)			// без фильтрации
	, Freq(0.f)
    , bInit(false)			// фильтр не
	, pFreqADC(nullptr)
	, pState(nullptr)	//		инициализирован
	, pStateBuf(nullptr)
{

}
//----------------------------------------------------------------------
CCustomIirFiltr::~CCustomIirFiltr()
{
	if (pStateBuf)
	{
		ippsFree(pStateBuf);
		pStateBuf = nullptr;
	}
}
//----------------------------------------------------------------------
bool CCustomIirFiltr::Create(
	float *pfreqADC)	// указатель на частоту дискретизации АЦП
{
	pFreqADC = pfreqADC;
	return *pFreqADC > 0.f;
}
//----------------------------------------------------------------------
bool CCustomIirFiltr::_InitFiltr(const float *pTaps, int order)
{// Инициализация структуры фильтра
	if (pStateBuf)			// вначале удаляю старую структуру
	{
		ippsFree(pStateBuf);
		pStateBuf = nullptr;
	}

	int sizeBuffer(0);
	bInit = ippsIIRGetStateSize_32f(order, &sizeBuffer) == ippStsNoErr;
	if (bInit)
		pStateBuf = ippsMalloc_8u(sizeBuffer);
	bInit = bInit & (ippsIIRInit_32f(&pState, pTaps, order, nullptr, pStateBuf) == ippStsNoErr);

	return bInit;
}
//----------------------------------------------------------------------
bool CCustomIirFiltr::_Filtration(
	float *pInpData,	// указатель на исходные данные
	float *pOutData,	// указатель на отфильтрованные данные
	int num,			// кол-во отсчётов
	bool flag_I)		// флаг типа функции
{// Функция фильтрации на все случаи жизни
	bool ret;
	IppStatus status;
	if (bInit)
	{
		if (flag_I)
			status = ippsIIR_32f_I(pInpData, num, pState);
		else
			status = ippsIIR_32f(pInpData, pOutData, num, pState);													
		ret = status == ippStsNoErr;
	}
	else
		ret = false;
	return ret;
}
//----------------------------------------------------------------------
bool CCustomIirFiltr::InitFiltr(TypeFiltr type, float freq)
{
	bool ret;
	if (type == tfUser)
	{
		FType = tfUser;
		ret = true;
	}
	else
		ret = false;
	return ret;
}
//----------------------------------------------------------------------
bool CCustomIirFiltr::Filtration(
	float *pInpData,	// указатель на исходные данные
	float *pOutData,	// указатель на отфильтрованные данные
	int num)			// кол-во отсчётов
{
	return _Filtration(pInpData, pOutData, num, false);
}
//----------------------------------------------------------------------
bool CCustomIirFiltr::Filtration_I(
	float *pInpOutData,	// указатель на исходные/выходные данные
	int num)			// кол-во отсчётов
{
	return _Filtration(pInpOutData, pInpOutData, num, true);
}
//----------------------------------------------------------------------
bool CCustomIirFiltr::ReStart()
{// Переинициализация структуры фильтра с прежними параметрами.
// Требуется при изменении частоты дискретизации АЦП или
// при перезапуске измерений. Для tfUser следует вызвать SetUserTaps().
	bool ret;
	if (bInit)
	{// параметры уже заданы
		if (FType != tfUser)
			ret = InitFiltr(FType, Freq);
		else
			ret = false;
	}
	else	// параметры ещё не заданы
		ret = false;
	return ret;
}
//----------------------------------------------------------------------
bool CCustomIirFiltr::SetUserTaps(float *ptaps, int order)
{// Инициализация фильтра tfUser. И ни каких других
	bool ret;
	if (FType == tfUser)
		ret = _InitFiltr(ptaps, order);
	else
		ret = false;
	return ret;
}
//----------------------------------------------------------------------