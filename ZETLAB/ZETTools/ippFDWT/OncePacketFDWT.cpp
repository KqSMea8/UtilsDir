//--------------------------------------------------------------------------
//	Файл OncePacketDWT.cpp
//	Версия от 29.08.2014
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//--------------------------------------------------------------------------
#include "StdAfx.h"
#include "math.h"
#include "OncePacketFDWT.h"
//--------------------------------------------------------------------------
COncePacketFDWT::COncePacketFDWT(void) : CCustomPacketFDWT()
{
	m_pSignal = NULL;
	m_pSignalInv = NULL;
	pppBuf = NULL;
	sizeSignalInv = 0;
	m_bDlyLineForward = true;
	pValThreshold = NULL;
}
//--------------------------------------------------------------------------
COncePacketFDWT::~COncePacketFDWT(void)
{
	DeleteBuf();
	if (m_pSignalInv)
	{ 
		delete [] m_pSignalInv;
		m_pSignalInv = nullptr;
	}
	if (pValThreshold)
	{ 
		delete [] pValThreshold;
		pValThreshold = nullptr;
	}
}
//--------------------------------------------------------------------------
long COncePacketFDWT::Create(TypeWavelet typeWavelet, int level,
	bool bneedInverse, int sizeSignal)
{
	long ret = CCustomPacketFDWT::Create(typeWavelet, level,
				bneedInverse, sizeSignal);
	if ( !CreateBuf() )
		ret |= wtrvError;
	return ret;
}
//--------------------------------------------------------------------------
bool COncePacketFDWT::CreateBuf()
{// Буферы c обнулением, поэтому массив m_pSize[] должен быть уже определен
	int i, n;
	bool ret;
	if (!pppBuf)
	{
		nLevelForBuf = FLevel;
		pppBuf = new float** [FLevel+1];
		for(i=0; i<=FLevel; ++i)
		{
			pppBuf[i] = new float* [ m_pNumBand[i] ];
			for(n=0; n<m_pNumBand[i]; ++n)
			{
				pppBuf[i][n] = new float [ m_pSize[i] ];
				ippsZero_32f( pppBuf[i][n], m_pSize[i] );
			}
		}
		m_pSignal = pppBuf[0][0];
		m_ppBandData = pppBuf[FLevel];
		ret = true;
	}
	else
		ret = false;
	return ret;
}
//--------------------------------------------------------------------------
void COncePacketFDWT::DeleteBuf()
{
	if (pppBuf)
	{
		for(int i=0; i<=nLevelForBuf; ++i)
		{
			for(int n=0; n<m_pNumBand[i]; ++n)
				delete [] pppBuf[i][n];
			delete [] pppBuf[i];
		}
		delete [] pppBuf;
		pppBuf = nullptr;
	}
	m_ppBandData = nullptr;
	m_pSignal = nullptr;
}
//--------------------------------------------------------------------------
void COncePacketFDWT::NewLevel()
{// Переопределение виртуальной функции, реализующей побочные свойства
// при изменении уровня разложения: новое кол-во двухмерных массивов
// коэффициентов FDWT
	CCustomPacketFDWT::NewLevel();
	DeleteBuf();
	CreateBuf();
}
//--------------------------------------------------------------------------
void COncePacketFDWT::NewLengthSignal()
{// Переопределение виртуальной функции, реализующей побочные свойства
// при изменении длины сигнала: новые размеры двухмерных массивов
// коэффициентов FDWT
	CCustomPacketFDWT::NewLengthSignal();
	DeleteBuf();
	CreateBuf();
}
//--------------------------------------------------------------------------
bool COncePacketFDWT::ReStart()
{// Обнуление массивов коэффициентов и переинициализация структур.
// Функция вызывается перед расчётом нового массива
	bool ret;
	if ( _Init() )
	{
		if (pppBuf)
		{// если массивы определены
			ret = true;
			for(int i=0; i<=nLevelForBuf; ++i)
				for(int n=0; n<m_pNumBand[i]; ++n)
					ret = ret &&
						( ippsZero_32f( pppBuf[i][n], m_pSize[i] ) ==
							ippStsNoErr );
		}
		else
			ret = false;
	}
	else
		ret = false;
	return ret;
}
//--------------------------------------------------------------------------
bool COncePacketFDWT::Forward(
	float *pData)		// указатель на исходные данные
{//	Прямое преобразование
	bool ret;
	if (bInitForward)
	{
		int nn, nFwdOffs;	// длина линии задержки для прямого FDWT
		nFwdOffs = FLengthFilter + fwdFltOffs - 1;
		// копирую исходный сигнал во внутренний буфер --------------------------
		ret = ippsCopy_32f(pData, m_pSignal, FLengthSignalClient) ==
														ippStsNoErr;
		if ( FLengthSignalClient < FLengthSignal )		//FLengthSignal=m_pSize[0]
			ret = ret && ( ippsZero_32f( m_pSignal + FLengthSignalClient,
				FLengthSignal - FLengthSignalClient ) == ippStsNoErr );

		// FDWT --------------------------------
		int n2, n2p;	// n2 = 2 * n; n2p = 2 * n +1;
		int m1, m2;
		for(int i=0; i<FLevel; ++i)
		{
			n2 = 0, n2p = 1;
			nn = m_pSize[i] - nFwdOffs;
			for(int n=0; n<m_pNumBand[i]; ++n)
			{
				if ( n & 1 )
				{// нечётный номер
					m1 = n2p;
					m2 = n2;
				}
				else
				{// чётный номер
					m1 = n2;
					m2 = n2p;
				}
				if (m_bDlyLineForward)
				{
					if (nn > 0)
						ret = ret && ( ippsWTFwdSetDlyLine_32f( pppFwdState[i][n]->pState,
							pppBuf[i][n]+nn, pppBuf[i][n]+nn ) == ippStsNoErr );
					else
						ret = false;
				}
				ret = ret &&
					( ippsWTFwd_32f( pppBuf[i][n],
									pppBuf[i+1][m1], pppBuf[i+1][m2],
									m_pSize[i+1], pppFwdState[i][n]->pState )
						== ippStsNoErr );
				n2 += 2; n2p += 2;
			}
		}
	}
	else
		ret = false;
	return ret;
}
//--------------------------------------------------------------------------
bool COncePacketFDWT::Inverse()
{// Обратное преобразование, результат в m_pSignalInv
	bool ret;
	if (FbNeedInverse)
	{
		if ( sizeSignalInv != m_pSize[0] )
		{
			sizeSignalInv = m_pSize[0];
			if (m_pSignalInv)
			{ 
				delete [] m_pSignalInv;
				m_pSignalInv = nullptr;
			}
			m_pSignalInv = new float [sizeSignalInv];
		}
		ret = _Inverse(m_pSignalInv);
	}
	else
		ret = false;
	return ret;
}
//--------------------------------------------------------------------------
bool COncePacketFDWT::Inverse_I()
{// Обратное преобразование, результат в m_pSignal
	bool ret;
	if (FbNeedInverse)
		ret = _Inverse(m_pSignal);
	else
		ret = false;
	return ret;
}
//--------------------------------------------------------------------------
bool COncePacketFDWT::_Inverse(float *pData)
{// Приватная функция - обратное FDWT на все случаи
	bool ret;
	int n1, n2, n2p, m1, m2, num, num2;
	if (bInitInverse)
	{
		ret = true;
		num = ( FLengthFilter - invFltOffs - 1 ) / 2;	//для db2 num=1
		num2 = num * 2;
		// Вначале все уровни, кроме первого, а потом отдельно первый уровень,
		// т.к. pData может быть либо m_pSignal (== pppBuf[0][0]),
		// либо m_pSignalInv(!= pppBuf[0][0])
		for(int i=FLevel; i>1; --i)
		{
			n2 = 0; n2p = 1;
			for(int n=0; n<m_pNumBand[i]; n+=2)
			{
				n1 = n / 2;
				if ( n1 & 1 )
				{// нечётный номер
					m1 = n2p;
					m2 = n2;
				}
				else
				{// чётный номер
					m1 = n2;
					m2 = n2p;
				}
				ret = ret && ( ippsWTInvSetDlyLine_32f( pppInvState[i-1][n1]->pState,
								pppBuf[i][m1], pppBuf[i][m2] )
									== ippStsNoErr );
				ret = ret &&
					( ippsWTInv_32f( pppBuf[i][m1]+num, pppBuf[i][m2]+num,
									 m_pSize[i]-num, pppBuf[i-1][n1],
									 pppInvState[i - 1][n1]->pState)
						== ippStsNoErr );
				ret = ret &&
					( ippsWTInv_32f( pppBuf[i][m1], pppBuf[i][m2], num,
									 pppBuf[i-1][n1]+(m_pSize[i-1] - num2),
									 pppInvState[i - 1][n1]->pState)
						== ippStsNoErr );
				n2 += 2; n2p += 2;
			}
		}
		// для db2 и 12 точек
		// ippsWTInvSetDlyLine_32f( state, low, high);
		// ippsWTInv_32f(&low[1], &high[1], 5, dst, state);
		// ippsWTInv_32f(low, high, 1, &dst[10], state);
		ret = ret && (ippsWTInvSetDlyLine_32f(pppInvState[0][0]->pState,
							pppBuf[1][0], pppBuf[1][1] ) == ippStsNoErr );
		ret = ret && ( ippsWTInv_32f( pppBuf[1][0]+num, pppBuf[1][1]+num,
			m_pSize[1] - num, pData, pppInvState[0][0]->pState)
														== ippStsNoErr );
		ret = ret && ( ippsWTInv_32f( pppBuf[1][0], pppBuf[1][1], num,
			pData + m_pSize[0] - num2, pppInvState[0][0]->pState)
														== ippStsNoErr );
	}
	else
		ret = false;
	return ret;
}
//--------------------------------------------------------------------------
// Расчёт пороговых значений для коэффициентов нижнего уровня разложения
bool COncePacketFDWT::CalcThreshold(// задаётся интервал исходного сигнала,
						// на котором определяются пороги	
	const int n0,		// начальный индекс интервала
	const int n1,		// конечный индекс интервала
	float *pThreshold)	// адрес массива порогов, если NULL, то
{						// массив внутренний
	float *padd(nullptr);
	// определяюсь с массивом -----------------------
	if (pThreshold)				// внешний массив
		padd = pThreshold;
	else
	{	// внутренний массив
		bool bMassNo;
		if (pValThreshold)
		{// массив есть
			if (nSizeMassThreshold != FQuantityBand)
			{
				delete [] pValThreshold;
				pValThreshold = nullptr;
				nSizeMassThreshold = FQuantityBand;
				bMassNo = true;
			}
			else
				bMassNo = false;
		}
		else
			bMassNo = true;	// массива пока нет
		if (bMassNo)
			pValThreshold = new float [FQuantityBand];
		padd = pValThreshold;
	}
	// Расчёт пороговых значений -------------------------
	int i0, i1;
	float mean, sko;
	bool ret = n1 > n0;
	if (ret)
	{
		i0 = n0 >> FLevel;
		i1 = n1 >> FLevel;
		ret = (i0 < m_pSize[FLevel]) && (i1 < m_pSize[FLevel]);
		if (ret)
		{
			int num = i1 - i0;
			float *pabs = new float [num];
			for(int n=0; n<FQuantityBand; ++n)
			{
				ippsAbs_32f(m_ppBandData[n] + i0, pabs, num);
				ippsMeanStdDev_32f(pabs, num, &mean, &sko, ippAlgHintFast);
				ippsMax_32f(pabs, num, &mean);
				padd[n] = mean + 2 * sko;
			}
			delete [] pabs;
			pabs = nullptr;
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
bool COncePacketFDWT::CalcForCompression(
	const float percent,		// проценты! от 0 до 100
	float *pValueThreshold)
{
	// Расчёт порогового значения -------------------------
	bool ret = (0.f < percent) && (percent < 100.f);
	if (ret)
	{
		float _max = 0.f, _min = FLT_MAX;
		int num = m_pSize[FLevel];
		float *pabs = new float [num];
		for(int n=0; n<FQuantityBand; ++n)
		{
			float _mi, _ma;
			if (ret)
			{
				ret = (ippsAbs_32f(m_ppBandData[n], pabs, num) == ippStsNoErr) &&
					  (ippsMinMax_32f(pabs, num, &_mi, &_ma) == ippStsNoErr);
				_max = max(_max, _ma);
				_min = min(_min, _mi);
			}
			else
				break;
		}
		delete [] pabs;
		pabs = nullptr;
		if (ret)
		{
			_min += (_max - _min) * percent * 0.01f;
			if (pValueThreshold)
			{
				*pValueThreshold = _min;
			}
			else
			{
				if (pValThreshold == NULL)
					pValThreshold = new float [FQuantityBand];
				else
				{
					if (nSizeMassThreshold != FQuantityBand)
					{
						delete [] pValThreshold;
						pValThreshold = nullptr;
						pValThreshold = new float [FQuantityBand];
						nSizeMassThreshold = FQuantityBand;
					}
				}
				ippsSet_32f(_min, pValThreshold, FQuantityBand);
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
bool COncePacketFDWT::CalcForSmoothing()
{
	bool ret = true;
	int sizeBand = m_pSize[FLevel];
	for (int i=1; i<FQuantityBand; ++i)
		ippsZero_32f(m_ppBandData[i], sizeBand);
	return ret;
}
//------------------------------------------------------------------------------
bool COncePacketFDWT::FactorsUnderThreshold(float *pThreshold)
{
	bool ret(true);
	float *padd(nullptr);
	if (pThreshold)
		padd = pThreshold;
	else
	{
		if (pValThreshold)
			padd = pValThreshold;
		else
			ret = false;
	}
	if (ret)
	{
		for(int n=0; n<FQuantityBand; ++n)
		{
			float thresh = padd[n];
			float *pAdd = m_ppBandData[n];
			for(int m=0; m<m_pSize[FLevel]; ++m)
				if ( fabs(pAdd[m]) <= thresh )
					pAdd[m] = 0.f;
		}
	}
	return ret;
}
//------------------------------------------------------------------------------