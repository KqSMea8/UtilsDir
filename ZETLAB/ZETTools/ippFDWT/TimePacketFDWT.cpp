//--------------------------------------------------------------------------
//	Файл TimePacketDWT.cpp
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//--------------------------------------------------------------------------
//		Класс предназначен для расчёта коэффициентов дискретного вейвлет
// преобразования (Fast Discrete Wavelet Transform) одномерных сигналов в
// соответствии с пакетным алгоритмом.
//		Класс является наследником базового класса CCustomDWT.
//		В классе используются функции библитеки IPP 7.0.
//--------------------------------------------------------------------------
#include "StdAfx.h"
#include "math.h"
#include "TimePacketFDWT.h"
//--------------------------------------------------------------------------
CTimePacketFDWT::CTimePacketFDWT(void) : CCustomPacketFDWT()
{
	pppBuf = NULL;	// двумерный массив коэффициентов FDWT
	FSizePageInLastLevel = 1;
	m_bAbsTypeDataLastLevel = false;
}
//--------------------------------------------------------------------------
CTimePacketFDWT::~CTimePacketFDWT(void)
{
	DeleteBuf();
}
//--------------------------------------------------------------------------
long CTimePacketFDWT::Create(TypeWavelet typeWavelet, int level,
		bool bneedInverse, int sizePageInLastLevel)
{
	FSizePageInLastLevel = sizePageInLastLevel;
	long ret = CCustomPacketFDWT::Create(typeWavelet, level,
											false, INFINITE);
	SetpSizePage();
	if ( !CreateBuf() )
		ret |= wtrvError;
	return ret;
}
//--------------------------------------------------------------------------
void CTimePacketFDWT::SetpSizePage()
{// Задание массива m_pSizePage[32], содержащего размеры страниц данных для
// всех уровней. Исходные данные - это размер FSizePageInLastLevel для
// последнего уровня FLevel.
	int size = FSizePageInLastLevel;
	ippsZero_32s(m_pSizePage, 32);
	for(int i=FLevel; i>=0; i--)
	{
		m_pSizePage[i] = size;
		size *= 2;
	}
}
//--------------------------------------------------------------------------
bool CTimePacketFDWT::CreateBuf()
{// Для 0 (исходный массив) тоже создаю буфер. Буферы c инициализацией,
//	поэтому массивы m_pNumBand[] и m_pSizePage[] должны быть уже определены
	int i, n;
	bool ret;
	if (!pppBuf)
	{
		nLevelForBuf = FLevel;
		pppBuf = new float** [FLevel+1];
		for(i=0; i<=FLevel; i++)
		{
			pppBuf[i] = new float* [ m_pNumBand[i] ];
			for(n=0; n<m_pNumBand[i]; n++)
			{
				pppBuf[i][n] = new float [ m_pSizePage[i] ];
				ippsZero_32f( pppBuf[i][n], m_pSizePage[i] );
			}
		}
		m_ppBandData = pppBuf[FLevel];
		// буфер по одной точке с каждой полосы последего уровня
		m_pDataLastLevel = new float [ m_pNumBand[FLevel] ];
		ret = true;
	}
	else
		ret = false;
	return ret;
}
//--------------------------------------------------------------------------
void CTimePacketFDWT::DeleteBuf()
{
	if (pppBuf)
	{
		for(int i=0; i<=nLevelForBuf; i++)
		{
			for(int n=0; n<m_pNumBand[i]; n++)
				delete [] pppBuf[i][n];
			delete [] pppBuf[i];
		}
		delete [] pppBuf;
		pppBuf = NULL;

	}
	m_ppBandData = NULL;

	delete[] m_pDataLastLevel;
	m_pDataLastLevel = NULL;
}
//--------------------------------------------------------------------------
void CTimePacketFDWT::NewLevel()
{// Виртуальная функция, вызываемая при изменении уровня разложения.
// В данном классе следует удалить буферы для страниц данных и массив
// m_pSizePage[]. И создать всё удалённое с новыми размерами
	CCustomPacketFDWT::NewLevel();
	DeleteBuf();
	SetpSizePage();
	CreateBuf();
}
//--------------------------------------------------------------------------
/*void CTimePacketFDWT::NewLengthSignal()
{
}*/
//--------------------------------------------------------------------------
bool CTimePacketFDWT::Forward(
	float *pData)		// указатель на новый блок исходных данных
{//	Прямое преобразование. Т.к. в классе выполняется потоковая обработка, то
// линии задержки теряют смысл.
	int i, n;
	bool ret;
	if (bInitForward)
	{
		// копирую исходный сигнал во внутренний буфер --------------------------
		ret = ippsCopy_32f(pData, pppBuf[0][0], m_pSizePage[0]) ==
														ippStsNoErr;
		// FDWT --------------------------------
		int n2, n2p;	// n2 = 2 * n; n2p = 2 * n +1;
		int m1, m2;
		for(i=0; i<FLevel; i++)
		{
			n2 = 0, n2p = 1;
			for(n=0; n<m_pNumBand[i]; n++)
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
				ret = ret &&
					( ippsWTFwd_32f( pppBuf[i][n],
									pppBuf[i+1][m1], pppBuf[i+1][m2],
									m_pSizePage[i+1], pppFwdState[i][n]->pState )
						== ippStsNoErr );
				n2 += 2; n2p += 2;
			}
		}	
		if (ret)
		{	// массив данных последнего уровня
			for(i=0; i<m_pNumBand[FLevel]; i++)
			{
				float a = *pppBuf[FLevel][i];
				if (m_bAbsTypeDataLastLevel)
					a = fabs(a);
				m_pDataLastLevel[i] = a;
			}
		}
		else
			ippsSet_32f(0.f, m_pDataLastLevel, m_pNumBand[FLevel]);
	}
	else
		ret = false;
	return ret;
}
//--------------------------------------------------------------------------
bool CTimePacketFDWT::ReStart()
{// Обнуление массивов коэффициентов и переинициализация структур.
// Функция вызывается перед расчётом нового массива
	bool ret;
	if ( _Init() )
	{
		if (pppBuf)
		{// если массивы определены
			ret = true;
			for(int i=0; i<=nLevelForBuf; i++)
				for(int n=0; n<m_pNumBand[i]; n++)
					ret = ret &&
						( ippsZero_32f( pppBuf[i][n], m_pSizePage[i] ) ==
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