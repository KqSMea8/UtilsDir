//----------------------------------------------------------------------
// Файл IirFiltrEnvelopeDouble.cpp
// Версия от 03.05.2012
// ЗАО "ЭТМС", Зеленоград, Антонов А.Ю.
//----------------------------------------------------------------------
#include "StdAfx.h"
#include <ippFiltrDouble\IirFiltrEnvelopeDouble.h>
//----------------------------------------------------------------------
CIirFiltrEnvelopeDouble::CIirFiltrEnvelopeDouble()
	: time(0.)
	, bNeed(false)
	, bInit(false)
	, pFreqADC(nullptr)
	, FType(tfNo)
{	
	pFiltrInternal1 = pFiltrInternal2 = nullptr;

}
//----------------------------------------------------------------------
CIirFiltrEnvelopeDouble::~CIirFiltrEnvelopeDouble()
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
bool CIirFiltrEnvelopeDouble::Create(
	double *pfreqADC)	// указатель на частоту дискретизации АЦП
{
	bool flag;
	pFiltrInternal1 = new CIirFiltrInternalDouble();
	pFiltrInternal2 = new CIirFiltrInternalDouble();
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
bool CIirFiltrEnvelopeDouble::InitFiltr(TypeFiltr type, double fTime)
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
bool CIirFiltrEnvelopeDouble::DefineNeedFiltr( double time )
{
	bool ret = false;
	if ( time > 0.f )
		ret = true;		// фильтр нужен
	
	return ret;
}
//----------------------------------------------------------------------
bool CIirFiltrEnvelopeDouble::_Filtration(double *pInpOutData, int num)
{
	bool ret;
	ret = pFiltrInternal1->Filtration_I(pInpOutData, num);
	ret = ret && pFiltrInternal2->Filtration_I(pInpOutData, num);
	ret = ret && ippsThreshold_64f_I(pInpOutData, num, 0., ippCmpLess)
												== ippStsNoErr;
	ret = ret && ippsSqrt_64f_I(pInpOutData, num) == ippStsNoErr;
	return ret;
}
//----------------------------------------------------------------------
bool CIirFiltrEnvelopeDouble::Filtration(double *pInpData, double *pOutData, int num)
{
	bool ret = false;
	if (bInit)
	{
		if (bNeed)
		{
			ret = ippsSqr_64f(pInpData, pOutData, num) == ippStsNoErr;
			ret = ret && _Filtration(pOutData, num);
		}
		else
			ret = ippsCopy_64f(pInpData, pOutData, num) == ippStsNoErr;
	}
	
	return ret;
}
//----------------------------------------------------------------------
bool CIirFiltrEnvelopeDouble::Filtration_I(double *pInpOutData, int num)
{
	bool ret = false;
	if (bInit)
	{	
		if (bNeed)
		{
			ret = ippsSqr_64f_I(pInpOutData, num) == ippStsNoErr;
			ret = ret && _Filtration(pInpOutData, num);		
		}
	}
	
	return ret;
}
//----------------------------------------------------------------------
bool CIirFiltrEnvelopeDouble::ReStart()
{
	return bInit && pFiltrInternal1->ReStart() &&
		pFiltrInternal2->ReStart();
}
//----------------------------------------------------------------------
double CIirFiltrEnvelopeDouble::PriorValue(double time)
{
	return 1. / pFiltrInternal1->PriorValue(1. / time);
}
//----------------------------------------------------------------------