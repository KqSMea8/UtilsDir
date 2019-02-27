//----------------------------------------------------------------------
// Файл CustomIirFiltrDouble.cpp
// Версия от 03.05.2012
// ЗАО "ЭТМС", Зеленоград, Савостин Ю.М.
//----------------------------------------------------------------------
#include "StdAfx.h"
#include <ippFiltrDouble\CustomIirFiltrDouble.h>
//----------------------------------------------------------------------
CCustomIirFiltrDouble::CCustomIirFiltrDouble()
	: FType(tfNo)	// без фильтрации
	, Freq(0.)
	, bInit(false)	// фильтр не
	, pState(nullptr)	//		инициализирован
	, pStateBuf(nullptr)
	, pFreqADC(nullptr)
{

}
//----------------------------------------------------------------------
CCustomIirFiltrDouble::~CCustomIirFiltrDouble()
{
	if (pStateBuf)
	{
		ippsFree(pStateBuf);
		pStateBuf = nullptr;
	}
}
//----------------------------------------------------------------------
bool CCustomIirFiltrDouble::Create(
	double *pfreqADC)	// указатель на частоту дискретизации АЦП
{
	pFreqADC = pfreqADC;
	return *pFreqADC > 0.;
}
//----------------------------------------------------------------------
bool CCustomIirFiltrDouble::_InitFiltr(const double *pTaps, int order)
{// Инициализация структуры фильтра
	if (pStateBuf)			// вначале удаляю старую структуру
	{
		ippsFree(pStateBuf);	
		pStateBuf = nullptr;
	}

	int sizeBuffer(0);
	bInit = ippsIIRGetStateSize_64f(order, &sizeBuffer) == ippStsNoErr;
	if (bInit)
		pStateBuf = ippsMalloc_8u(sizeBuffer);
	bInit = bInit & (ippsIIRInit_64f(&pState, pTaps, order, nullptr, pStateBuf) == ippStsNoErr);

	return bInit;
}
//----------------------------------------------------------------------
void CCustomIirFiltrDouble::SetFreqMinInHz(double freq)
{
	if ((pFreqADC != nullptr) && (0. < freq) && (freq < *pFreqADC * 0.5))
	{
		FreqMinHz = freq;
		ReStart();
	}
}
//------------------------------------------------------------------------------
bool CCustomIirFiltrDouble::_Filtration(
	double *pInpData,	// указатель на исходные данные
	double *pOutData,	// указатель на отфильтрованные данные
	int num,			// кол-во отсчётов
	bool flag_I)		// флаг типа функции
{// Функция фильтрации на все случаи жизни
	bool ret;
	IppStatus status;
	if (bInit && pState)
	{
		if (flag_I)
			status = ippsIIR_64f_I(pInpData, num, pState);
		else
			status = ippsIIR_64f(pInpData, pOutData, num, pState);													
		ret = status == ippStsNoErr;
	}
	else
		ret = false;
	return ret;
}
//----------------------------------------------------------------------
bool CCustomIirFiltrDouble::InitFiltr(TypeFiltr type, double freq)
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
bool CCustomIirFiltrDouble::Filtration(
	double *pInpData,	// указатель на исходные данные
	double *pOutData,	// указатель на отфильтрованные данные
	int num)			// кол-во отсчётов
{
	return _Filtration(pInpData, pOutData, num, false);
}
//----------------------------------------------------------------------
bool CCustomIirFiltrDouble::Filtration_I(
	double *pInpOutData,	// указатель на исходные/выходные данные
	int num)			// кол-во отсчётов
{
	return _Filtration(pInpOutData, pInpOutData, num, true);
}
//----------------------------------------------------------------------
bool CCustomIirFiltrDouble::ReStart()
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
bool CCustomIirFiltrDouble::SetUserTaps(double *ptaps, int order)
{// Инициализация фильтра tfUser. И ни каких других
	bool ret;
	if (FType == tfUser)
		ret = _InitFiltr(ptaps, order);
	else
		ret = false;
	return ret;
}
//----------------------------------------------------------------------
//------------------------------------------------------------------------------
void CCustomIirFiltrDouble::_BilinearTransformation(
	TransferFunction tfFunction, double dFrequency, TransferFunction &tfReturn)
{
	int iOrder(max(tfFunction.numerator.getOrder(), tfFunction.denomenator.getOrder()));
	int iOrder_1(iOrder + 1);
	vector <double> numerator, denomenator, y, x, z, e;

	for (int i = 0; i < iOrder_1; ++i)
		numerator.push_back(tfFunction.numerator.getCoeff(iOrder - i).real());
	for (int i = 0; i < iOrder_1; ++i)
		denomenator.push_back(tfFunction.denomenator.getCoeff(iOrder - i).real());

	x.resize((iOrder_1)* (iOrder_1));
	z.resize(iOrder_1);
	e.resize(iOrder_1);

	//числитель
	double k = dFrequency / 2.;
	for (int i = 0; i < iOrder_1; ++i)
	{
		k = k / tan(dFrequency * M_PI);
		y.push_back(numerator[iOrder - i] * k);
	}
	sub(x.data(), z.data(), e.data(), y.data(), 0, iOrder);
	for (int i = 0; i < iOrder + 1; ++i)
		tfReturn.numerator.setCoeff(i, e[i]);

	//знаменатель
	y.clear();
	k = dFrequency / 2.;
	for (int i = 0; i < iOrder_1; ++i)
	{
		k = k / tan(dFrequency * M_PI);
		y.push_back(denomenator[iOrder - i] * k);
	}
	sub(x.data(), z.data(), e.data(), y.data(), 1, iOrder);
	for (int i = 0; i < iOrder + 1; ++i)
		tfReturn.denomenator.setCoeff(i, e[i]);

	tfReturn.numerator /= tfReturn.denomenator.getCoeff(0);
	tfReturn.denomenator /= tfReturn.denomenator.getCoeff(0);
}
//------------------------------------------------------------------------------
void CCustomIirFiltrDouble::sub(double* x1, double* z, double* e, double* y, int t, int n)
{
	double** x;
	int n_1(n + 1);
	x = new double*[n_1];
	for (int i = 0; i < n_1; ++i)
		x[i] = new double[n_1];

	for (int i = 0; i < n_1; ++i)
	{
		for (int j = 0; j < n_1; ++j)
			x[i][j] = x1[j + i * n_1];
	}
	if (t == 0)
	{
		z[0] = 1;
		z[1] = 0;
		x[0][0] = 1;
		for (int i = 1; i < n_1; ++i)
		{
			for (int j = 1; j < i + 1; ++j)
				x[0][j] = z[j] + z[j - 1];
			for (int j = 1; j < i + 1; ++j)
				z[j] = x[0][j];
			z[i] = 1;
		}
		x[0][n] = 1;
		for (int i = 1; i < n_1; ++i)
			x[i][0] = 1;
		for (int j = 1; j < n_1; ++j)
		{
			for (int i = 1; i < n_1; ++i)
				x[i][j] = x[i - 1][j] - x[i - 1][j - 1] - x[i][j - 1];
		}
	}
	for (int j = 0; j < n_1; ++j)
	{
		e[j] = 0;
		for (int i = 0; i < n_1; ++i)
			e[j] += y[i] * x[i][j];
	}
	for (int i = 0; i < n_1; ++i)
	{
		for (int j = 0; j < n_1; ++j)
			x1[j + i * (n + 1)] = x[i][j];
	}

	for (int i = 0; i < n_1; ++i)
	{
		delete[]x[i];// нельзя делать x[i] = nullptr; Программа падает
	}
	delete[]x;
	x = nullptr;
}