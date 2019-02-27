//----------------------------------------------------------------------
// Файл IirFiltrEnvelope.cpp
// Версия от 05.03.2012
// ЗАО "ЭТМС", Зеленоград, Антонов А.Ю.
//----------------------------------------------------------------------
#include "StdAfx.h"
#include <ippFiltr\IirFiltrEnvelope.h>
//----------------------------------------------------------------------
CIirFiltrEnvelope::CIirFiltrEnvelope()
	: time(0.f)
	, bInit(false)
	, bNeed(false)
	, pFreqADC(nullptr)
{	
	pFiltrInternal1 = pFiltrInternal2 = nullptr;
	
}
//----------------------------------------------------------------------
CIirFiltrEnvelope::~CIirFiltrEnvelope()
{
	if (pFiltrInternal1)
	{ 
		delete pFiltrInternal1;
		pFiltrInternal1 = nullptr;
	}
	if (pFiltrInternal2)
	{ 
		delete pFiltrInternal2;
		pFiltrInternal2 = nullptr;
	}
}
//----------------------------------------------------------------------
bool CIirFiltrEnvelope::Create(
	float *pfreqADC)	// указатель на частоту дискретизации АЦП
{
	bool flag;
	pFiltrInternal1 = new CIirFiltrInternal();
	pFiltrInternal2 = new CIirFiltrInternal();
	flag = pFiltrInternal1->Create(pfreqADC) &&
			pFiltrInternal2->Create(pfreqADC);
	if (!flag)
	{
		delete pFiltrInternal1;
		pFiltrInternal1 = nullptr;
		delete pFiltrInternal2;
		pFiltrInternal2 = nullptr;
	}
	else
	{
		pFiltrInternal1->SetAutoOrder(false);
		pFiltrInternal2->SetAutoOrder(false);
	}
	return flag;
}
//----------------------------------------------------------------------
bool CIirFiltrEnvelope::InitFiltr(TypeFiltr type, float fTime)
{	// новый параметры фильтра
	if (type == tfEnvelope)
	{
		FType = tfEnvelope;
		// определяю флаги необходимости фильтров, они
		// используются в других функциях
		bNeed = DefineNeedFiltr(fTime);
		if ( bNeed )
		{
			time = fTime;
			bInit = pFiltrInternal1->InitFiltr(tfLowPass, 1.f / time) &&
					pFiltrInternal2->InitFiltr(tfLowPass, 1.f / time);
		}
		else
			bInit = true;
	}
	else
		bInit = false;
	return bInit;
}
//----------------------------------------------------------------------
bool CIirFiltrEnvelope::DefineNeedFiltr( float time )
{
	bool ret;
	if ( time > 0.f )
		ret = true;		// фильтр нужен
	else
		ret = false;	// фильтр не нужен
	return ret;
}
//----------------------------------------------------------------------
bool CIirFiltrEnvelope::_Filtration(float *pInpOutData, int num)
{
	bool ret;
	ret = pFiltrInternal1->Filtration_I(pInpOutData, num);
	ret = ret && pFiltrInternal2->Filtration_I(pInpOutData, num);
	ret = ret && ippsThreshold_32f_I(pInpOutData, num, 0.f, ippCmpLess)
												== ippStsNoErr;
	ret = ret && ippsSqrt_32f_I(pInpOutData, num) == ippStsNoErr;
	return ret;
}
//----------------------------------------------------------------------
bool CIirFiltrEnvelope::Filtration(float *pInpData, float *pOutData, int num)
{
	bool ret;
	if (bInit)
	{
		if (bNeed)
		{
			ret = ippsSqr_32f(pInpData, pOutData, num) == ippStsNoErr;
			ret = ret && _Filtration(pOutData, num);
		}
		else
			ret = ippsCopy_32f(pInpData, pOutData, num) == ippStsNoErr;
	}
	else
		ret = false;
	return ret;
}
//----------------------------------------------------------------------
bool CIirFiltrEnvelope::Filtration_I(float *pInpOutData, int num)
{
	bool ret(bInit);
	if (bInit)
	{	
		if (bNeed)
		{
			ret = ippsSqr_32f_I(pInpOutData, num) == ippStsNoErr;
			ret = ret && _Filtration(pInpOutData, num);
		}
	}
	return ret;
}
//----------------------------------------------------------------------
bool CIirFiltrEnvelope::ReStart()
{
	return bInit && pFiltrInternal1->ReStart() &&
		pFiltrInternal2->ReStart();
}
//----------------------------------------------------------------------
float CIirFiltrEnvelope::PriorValue(float time)
{
	return 1.f / pFiltrInternal1->PriorValue(1.f / time);
}
//----------------------------------------------------------------------