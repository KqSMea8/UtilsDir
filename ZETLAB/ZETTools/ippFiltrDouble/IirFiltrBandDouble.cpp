//----------------------------------------------------------------------
// Файл CIirFiltrBandDouble.cpp
// Версия от 03.05.2012
// ЗАО "ЭТМС", Зеленоград, Савостин Ю.М.
//----------------------------------------------------------------------
#include "StdAfx.h"
#include <ippFiltrDouble\IirFiltrBandDouble.h>
//----------------------------------------------------------------------
CIirFiltrBandDouble::CIirFiltrBandDouble()
	: nTemp(0)
	, pTemp(nullptr)
	, FType(tfNo)
	, bNeedHi(false)
	, bNeedLo(false)
	, bInit(false)
	, m_countLpf(1)
	, m_countHpf(1)
{
	pFiltrLoPass = pFiltrHiPass = nullptr;
	Ffmin = Ffmax = 0.;
	
}
//----------------------------------------------------------------------
CIirFiltrBandDouble::~CIirFiltrBandDouble()
{
	if (nullptr != pFiltrLoPass)
	{ 
		delete[] pFiltrLoPass;
		pFiltrLoPass = nullptr;
	}
	if (nullptr != pFiltrHiPass)
	{ 
		delete[] pFiltrHiPass;
		pFiltrHiPass = nullptr;
	}
	if (pTemp)
	{ 
		delete [] pTemp;
		pTemp = nullptr;
	}
}
//----------------------------------------------------------------------
bool CIirFiltrBandDouble::Create(
	double *pfreqADC)	// указатель на частоту дискретизации АЦП
{
	m_countLpf = 1;
	m_countHpf = 1;

	bool flag;
	if (pFiltrLoPass != nullptr)
	{
		delete[] pFiltrLoPass;
		pFiltrLoPass = nullptr;
	}
	pFiltrLoPass = new CIirFiltrInternalDouble[m_countLpf];
	if (pFiltrHiPass != nullptr)
	{
		delete[] pFiltrHiPass;
		pFiltrHiPass = nullptr;
	}
	pFiltrHiPass = new CIirFiltrInternalDouble[m_countHpf];
	flag = pFiltrLoPass->Create(pfreqADC) &&
			pFiltrHiPass->Create(pfreqADC);
	//pFiltrLoPass->SetAutoOrder(false);
	//pFiltrHiPass->SetAutoOrder(false);
	if (!flag)
	{
		delete[] pFiltrLoPass;
		pFiltrLoPass = nullptr;
		delete[] pFiltrHiPass;
		pFiltrHiPass = nullptr;
	}
	return flag;
}

bool CIirFiltrBandDouble::Create(double* pfreqADC, int countLpf, int countHpf)
{
	if (countLpf < 1)
		countLpf = 1;
	if (countLpf > 9)
		countLpf = 9;
	if (countHpf < 1)
		countHpf = 1;
	if (countHpf > 9)
		countHpf = 9;
	m_countLpf = countLpf;
	m_countHpf = countHpf;
	bool flag(true);
	if (pFiltrLoPass != nullptr)
	{
		delete[]pFiltrLoPass;
		pFiltrLoPass = nullptr;
	}
	pFiltrLoPass = new CIirFiltrInternalDouble[m_countLpf];
	if (pFiltrHiPass != nullptr)
	{
		delete[]pFiltrHiPass;
		pFiltrHiPass = nullptr;
	}
	pFiltrHiPass = new CIirFiltrInternalDouble[m_countHpf];
	for (int i = 0; i < m_countLpf; ++i)
		flag &= pFiltrLoPass[i].Create(pfreqADC);
	//for (int i = 0; i < m_countLpf; ++i)
	//	pFiltrLoPass[i].SetAutoOrder(false);
	for (int i = 0; i < m_countHpf; ++i)
		flag &= pFiltrHiPass[i].Create(pfreqADC);
	//for (int i = 0; i < m_countHpf; ++i)
	//	pFiltrHiPass[i].SetAutoOrder(false);
	if (!flag)
	{
		delete[] pFiltrLoPass;
		pFiltrLoPass = nullptr;
		delete[] pFiltrHiPass;
		pFiltrHiPass = nullptr;
	}
	return flag;
}

//----------------------------------------------------------------------
// Определяю - нужен фильтр или нет
// Возвращаемое значение: true - фильтр нужен, false - фильтр не нужен
bool CIirFiltrBandDouble::DefineNeedFiltr(
	double freq)			// запрошенная частота среза фильтра или 0
{
	bool ret;
	if ( freq > 0. )
		ret = true;		// фильтр нужен
	else
		ret = false;	// фильтр не нужен
	return ret;
}
//----------------------------------------------------------------------
double CIirFiltrBandDouble::GetFmin()
{
	double f_ret;
	if (FType == tfBandPass)
		f_ret = pFiltrHiPass->GetFreq() / freqCorrection(pFiltrHiPass->GetOrder(), m_countHpf);
	else
		f_ret = pFiltrLoPass->GetFreq() * freqCorrection(pFiltrLoPass->GetOrder(), m_countLpf);
	return f_ret;
}
//----------------------------------------------------------------------
double CIirFiltrBandDouble::GetFmax()
{
	double f_ret;
	if (FType == tfBandPass)
		f_ret = pFiltrLoPass->GetFreq() * freqCorrection(pFiltrLoPass->GetOrder(), m_countLpf);
	else
		f_ret = pFiltrHiPass->GetFreq() / freqCorrection(pFiltrHiPass->GetOrder(), m_countHpf);
	return f_ret;
}
//----------------------------------------------------------------------
// Функция фильтрации. Возвращает true, если требуемые (один или два)
// фильтры проинициализированы и выполнена сама фильтрация
bool CIirFiltrBandDouble::Filtration(
	double *pInpData,		// указатель на исходные данные
	double *pOutData,		// указатель на отфильтрованные данные
	int num)				// кол-во отсчётов
{
	bool ret;
	if (bInit)
	{
		if ( bNeedHi && bNeedLo )
		{// ФВЧ и ФНЧ нужны оба
			if (FType == tfBandPass)
			{
				ret = pFiltrHiPass->Filtration(pInpData, pOutData, num);
				for (int i = 1; i < m_countHpf; ++i)
					ret &= pFiltrHiPass[i].Filtration_I(pOutData, num);
				for (int i = 0; i < m_countLpf; ++i)
					ret &= pFiltrLoPass[i].Filtration_I(pOutData, num);
			}
			else
			{// FType = tfBandStop
				if (num > nTemp)
				{
					delete [] pTemp;
					pTemp = nullptr;
					pTemp = new double [num];
					nTemp = num;
				}

				ret = pFiltrLoPass->Filtration(pInpData, pTemp, num) &&
					pFiltrHiPass->Filtration(pInpData, pOutData, num);
				for (int i = 1; i < m_countHpf; ++i)
					ret &= pFiltrHiPass->Filtration_I(pOutData, num);
				for (int i = 1; i < m_countLpf; ++i)
					ret &= pFiltrLoPass->Filtration_I(pTemp, num);
				ret &= ippsAdd_64f_I(pTemp, pOutData, num) == ippStsNoErr;
			}
		}
		else if (bNeedHi)	// нужен только ФВЧ
		{
			ret = pFiltrHiPass->Filtration(pInpData, pOutData, num);
			for (int i = 1; i < m_countHpf; ++i)
				ret &= pFiltrHiPass[i].Filtration_I(pOutData, num);
		}
		else if (bNeedLo)	// нужен только ФНЧ
		{
			ret = pFiltrLoPass->Filtration(pInpData, pOutData, num);
			for (int i = 1; i < m_countLpf; ++i)
				ret &= pFiltrLoPass[i].Filtration_I(pOutData, num);
		}
		else
			ret = ippsCopy_64f(pInpData, pOutData, num) == ippStsNoErr;
	}
	else
		ret = false;
	return ret;
}
//----------------------------------------------------------------------
bool CIirFiltrBandDouble::Filtration_I(double *pInpOutData, int num)
{
	bool ret;
	if (bInit)
	{
		if ( bNeedHi && bNeedLo )
		{// ФВЧ и ФНЧ нужны оба
			if (FType == tfBandPass)
			{
				ret = true;
				for (int i = 0; i < m_countHpf; ++i)
					ret &= pFiltrHiPass[i].Filtration_I(pInpOutData, num);
				for (int i = 0; i < m_countLpf; ++i)
					ret &= pFiltrLoPass[i].Filtration_I(pInpOutData, num);
			}
			else
			{// FType = tfBandStop
				if (num > nTemp)
				{
					delete [] pTemp;
					pTemp = nullptr;
					pTemp = new double [num];
					nTemp = num;
				}
				ret = pFiltrHiPass->Filtration(pInpOutData, pTemp, num);
				for (int i = 1; i < m_countHpf; ++i)
					ret &= pFiltrHiPass[i].Filtration_I(pTemp, num);
				for (int i = 0; i < m_countLpf; ++i)
					ret &= pFiltrLoPass[i].Filtration_I(pInpOutData, num);
				ret &= ippsAdd_64f_I(pTemp, pInpOutData, num) == ippStsNoErr;
			}
		}
		else if (bNeedHi)	// нужен только ФВЧ
		{
			ret = true;
			for (int i = 0; i < m_countHpf; ++i)
				ret &= pFiltrHiPass[i].Filtration_I(pInpOutData, num);
		}

		else if (bNeedLo)	// нужен только ФНЧ
		{
			ret = true;
			for (int i = 0; i < m_countLpf; ++i)
				ret &= pFiltrLoPass[i].Filtration_I(pInpOutData, num);
		}
		else
			ret = true;
	}
	else
		ret = false;
	return ret;
}
//----------------------------------------------------------------------
bool CIirFiltrBandDouble::InitFiltr(
	TypeFiltr type,		// тип фильтра
	double fmin,			// частота среза или 0
	double fmax)			// частота среза или 0
{// Инициализация структур ФВЧ и ФНЧ
	bool retHi, retLo;
	if ( type == tfBandPass )
	{
		FType = tfBandPass;
		// определяю флаги необходимости фильтров, они
		// используются в других функциях
		bNeedHi = DefineNeedFiltr(fmin);
		bNeedLo = DefineNeedFiltr(fmax);

		if (bNeedHi)
		{
			retHi = true;
			double freqMin(fmin);
			int ord(0);
			if (m_countHpf > 0)
			{
				if (pFiltrHiPass[0].InitFiltr(tfHighPass, freqMin))
				{
					ord = pFiltrHiPass[0].GetOrder();
					freqMin *= freqCorrection(ord, m_countHpf);
				}
			}
			for (int i = 0; i < m_countHpf; ++i)
				retHi &= pFiltrHiPass[i].InitFiltr(tfHighPass, ord, freqMin);
		}
		else
			retHi = true;
		if (bNeedLo)
		{
			retLo = true;
			double freqMax(fmax);
			int ord(0);
			if (m_countLpf > 0)
			{
				if (pFiltrLoPass[0].InitFiltr(tfLowPass, freqMax))
				{
					ord = pFiltrLoPass[0].GetOrder();
					freqMax /= freqCorrection(ord, m_countLpf);
				}
			}
			for (int i = 0; i < m_countLpf; ++i)
				retLo &= pFiltrLoPass[i].InitFiltr(tfLowPass, ord, freqMax);
		}
		else
			retLo = true;
		bInit = retHi && retLo;
	}
	else if ( type == tfBandStop )
	{
		FType = tfBandStop;
		bNeedHi = DefineNeedFiltr(fmax);
		bNeedLo = DefineNeedFiltr(fmin);

		if (bNeedHi)
		{
			retHi = true;
			double freqMax(fmax);
			int ord(0);
			if (m_countHpf > 0)
			{
				if (pFiltrHiPass[0].InitFiltr(tfHighPass, freqMax))
				{
					ord = pFiltrHiPass[0].GetOrder();
					freqMax *= freqCorrection(ord, m_countHpf);
				}
			}
			for (int i = 0; i < m_countHpf; ++i)
				retHi &= pFiltrHiPass[i].InitFiltr(tfHighPass, ord, freqMax);
		}
		else
			retHi = true;
		if (bNeedLo)
		{
			retLo = true;
			double freqMin(fmin);
			int ord(0);
			if (m_countLpf > 0)
			{
				if (pFiltrLoPass[0].InitFiltr(tfLowPass, freqMin))
				{
					ord = pFiltrLoPass[0].GetOrder();
					freqMin /= freqCorrection(ord, m_countLpf);
				}
			}
			for (int i = 0; i < m_countLpf; ++i)
				retLo &= pFiltrLoPass[i].InitFiltr(tfLowPass, ord, freqMin);
		}
		else
			retLo = true;
		bInit = retHi && retLo;
	}
	else
		bInit = false;
	return bInit;
}
//----------------------------------------------------------------------
double CIirFiltrBandDouble::PriorValueFmin(double freq)
{
	double f_ret;
	if ( FType == tfBandPass )
		f_ret = pFiltrHiPass->PriorValue(freq);
	else
		f_ret = pFiltrLoPass->PriorValue(freq);
	return f_ret;
}
//----------------------------------------------------------------------
double CIirFiltrBandDouble::PriorValueFmax(double freq)
{
	double f_ret;
	if ( FType == tfBandPass )
		f_ret = pFiltrLoPass->PriorValue(freq);
	else
		f_ret = pFiltrHiPass->PriorValue(freq);
	return f_ret;
}
//----------------------------------------------------------------------
bool CIirFiltrBandDouble::ReStart()
{
	bool ret = true;
	for (int i = 0; i < m_countLpf; ++i)
		ret &= pFiltrLoPass[i].ReStart();
	for (int i = 0; i < m_countHpf; ++i)
		ret &= pFiltrHiPass[i].ReStart();
	return bInit && ret;
}
//----------------------------------------------------------------------
int CIirFiltrBandDouble::GetOrderLPF()
{
	return pFiltrLoPass->GetOrder();
}
//----------------------------------------------------------------------
int CIirFiltrBandDouble::GetOrderHPF()
{
	return pFiltrHiPass->GetOrder();
}
//----------------------------------------------------------------------
void CIirFiltrBandDouble::SetFreqMinInHz(double freq)
{
	for (int i = 0; i < m_countHpf; ++i)
		pFiltrHiPass[i].SetFreqMinInHz(freq);
	for (int i = 0; i < m_countLpf; ++i)
		pFiltrLoPass[i].SetFreqMinInHz(freq);
}
//----------------------------------------------------------------------
const double* CIirFiltrBandDouble::getTapsDoubleLPF()
{
	return pFiltrLoPass->getTapsDouble();
}
//----------------------------------------------------------------------
const double* CIirFiltrBandDouble::getTapsDoubleHPF()
{
	return pFiltrHiPass->getTapsDouble();
}
//----------------------------------------------------------------------