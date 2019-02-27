//--------------------------------------------------------------------------
//	Модуль CPacketDWT.cpp
//	Версия от 16.12.2011
//--------------------------------------------------------------------------
//		Класс предназначен для расчёта коэффициентов дискретного вейвлет
// преобразования (Fast Discrete Wavelet Transform) одномерных сигналов в
// соответствии с пакетным алгоритмом.
//		Класс является наследником базового класса CCustomDWT.
//		В классе используются функции библитеки IPP 7.0.
//--------------------------------------------------------------------------
#include "StdAfx.h"
#include "PacketFDWT.h"
//--------------------------------------------------------------------------
CPacketFDWT::CPacketFDWT(void) : CCustomFDWT()
{
	pppBuf = NULL;
	pppFwdState = NULL;
}
//--------------------------------------------------------------------------
CPacketFDWT::~CPacketFDWT(void)
{
	DeleteAll(FLevel);
}
//--------------------------------------------------------------------------
bool CPacketFDWT::Create(TypeWavelet typeWavelet,
						int lengthSignal, int level, int sizePageSignal)
{// здесь важен порядок:
//	1. параметры базового класса и массив m_pSize[]
//	2. размеры страниц (массив m_pSizePage[]) и 
//	   кол-во буферов для каждого уровня (массив m_pNumBuf[])
//	3. создание буферов, структур для FDWT
	bool ret = CCustomFDWT::Create(typeWavelet, lengthSignal, level);
	if (ret)
	{		
		SetNumBuf();
		ret = SetSizePageSignal(sizePageSignal);
		CreateAll();
	}
	return ret;
}
//--------------------------------------------------------------------------
void CPacketFDWT::SetNumBuf()
{// задаю кол-во буферов для каждого уровня. 0 (исходный сигнал) - это 1.
	ippsZero_32s(m_pNumBuf, 32);
	int n = 1;
	for(int i=0; i<=FLevel; i++)
	{
		m_pNumBuf[i] = n;
		n *= 2;
	}
}
//--------------------------------------------------------------------------
bool CPacketFDWT::SetSizePageSignal(int size)
{// определяю размеры страниц, т.е. массив m_pSizePage[]
//	Возвращаю true если заданное число установлено. Иначе
//	false, но массив всё равно задаю
	bool ret;
	int numPage;
	int n = 1 << FLevel;
	if ( ( n > m_pSize[0] ) || ( size < n ) )
	{
		int m = int( size / n ) * n;
		if ( (size - m) == 0 )	// число кратно n
			numPage = size;
		else					// число не кратно n
		{
			if (m == 0)
				m = 1;
			numPage = n * m;
		}
	
		m_pSizePage[0] = numPage;	// размер страницы исходного сигнала
		for(n=1; n<=FLevel; n++ )
		{
			numPage /= 2;
			m_pSizePage[n] = numPage;
		}
		ret = m_pSizePage[0] == size;
	}
	else
		ret = false;
	return ret;
}
//--------------------------------------------------------------------------
void CPacketFDWT::DeleteAll(int level)
{
	if ( pppBuf )
	{// буферы есть для всех уровней
		for(int i=0; i<=level; i++)
		{
			for(int n=0; n<m_pNumBuf[i]; n++)
				delete pppBuf[i][n];
			delete [] pppBuf[i];
		}
		delete [] pppBuf;
	}
	delete [] m_ppBandData;

	if ( pppFwdState )
	{// структур для самого большого уровня не надо
		for(int i=0; i<level; i++)
		{
			for(int n=0; n<m_pNumBuf[i]; n++)
				ippsWTFwdFree_32f( pppFwdState[i][n] );
			delete [] pppFwdState[i];
		}
		delete [] pppFwdState;
	}
}
//--------------------------------------------------------------------------
bool CPacketFDWT::CreateAll()
{// Для 0 (исходный массив) тоже создаю буфер
	int i, n;
	bool ret = true;
	pppBuf = new CShiftBuffer <float>** [FLevel+1];
	for(i=0; i<=FLevel; i++)
	{
		pppBuf[i] = new CShiftBuffer <float>* [ m_pNumBuf[i] ];
		for(n=0; n<m_pNumBuf[i]; n++)
		{
			pppBuf[i][n] = new CShiftBuffer <float>;
			ret = ret && pppBuf[i][n]->Create( m_pSize[i], m_pSizePage[i] );
		}
	}
	m_pSignal = pppBuf[0][0];

	m_ppBandData = new float* [ m_pNumBuf[FLevel] ];
	for(n=0; n<m_pNumBuf[FLevel]; n++)
		m_ppBandData[n] = pppBuf[FLevel][n]->GetGlobalAddres();

	// для высшего уровня структуры не нужны
	pppFwdState = new IppsWTFwdState_32f** [FLevel];
	for(i=0; i<FLevel; i++)
		pppFwdState[i] = new IppsWTFwdState_32f* [ m_pNumBuf[i] ];

	return ret;
}
//--------------------------------------------------------------------------
bool CPacketFDWT::_InitForward()
{
	bool ret = true;
	for(int i=0; i<FLevel; i++)
	{
		for(int n=0; n<m_pNumBuf[i]; n++)
		{	
			curStatus = ippsWTFwdInitAlloc_32f( &pppFwdState[i][n],
									pTabs_fwdLo, FLengthFilter, -1,
									pTabs_fwdHi, FLengthFilter, -1 );
			ret = ret && (curStatus == ippStsNoErr);
		}
	}
	return ret;
}
//--------------------------------------------------------------------------
bool CPacketFDWT::InitForward(int sizePageSignal)
{
	bool ret;
	if ( SetSizePageSignal(sizePageSignal) )
		ret = _InitForward();
	else
		ret = false;
	return ret;
}
//--------------------------------------------------------------------------
bool CPacketFDWT::InitForwardOne(int size)
{
	int n = 1 << FLevel;
	int m = int(size / n) * n;

	if ( size > m_pSize[0] )
	{
	}

	return _InitForward();
}
//--------------------------------------------------------------------------
bool CPacketFDWT::ReStartInitForward()
{
	return _InitForward();
}
//--------------------------------------------------------------------------
bool CPacketFDWT::Forward(
	float *pData)		// указатель на новый блок исходных данных
{//	потоковый вызов, задержка не нужна, адреса последних страниц	
	bool ret = true;
	m_pSignal->NewData(pData);
	// FDWT исходного сигнала
	pppBuf[1][0]->ShiftData();
	pppBuf[1][1]->ShiftData();
	curStatus = ippsWTFwd_32f(pData,
		pppBuf[1][0]->GetPointerLastPage(),
		pppBuf[1][1]->GetPointerLastPage(),
		m_pSizePage[1], pppFwdState[0][0]);
	ret = ret && (curStatus == ippStsNoErr);

	int i1 = 2, n2, n2p;	// i1 = i + 1; n2 = 2 * n; n2p = 2 * n +1;
	int m1, m2;
	for(int i=1; i<FLevel; i++)
	{
		n2 = 0, n2p = 1;
		for(int n=0; n<m_pNumBuf[i]; n++)
		{
			pppBuf[i1][n2] ->ShiftData();
			pppBuf[i1][n2p]->ShiftData();
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
			curStatus = ippsWTFwd_32f(
				pppBuf[i][n]->GetPointerLastPage(),
				pppBuf[i1][m1]->GetPointerLastPage(),
				pppBuf[i1][m2]->GetPointerLastPage(),
				m_pSizePage[i1], pppFwdState[i][n] );

			n2 += 2; n2p += 2;
			ret = ret && (curStatus == ippStsNoErr);
		}
		i1++;
	}
	return ret;
}
//--------------------------------------------------------------------------
bool CPacketFDWT::ForwardOne(
	float *pSignal)		// указатель на массив исходных данных
{// одноразовый вызов, задаю задержку, адреса самих буферов
	// при src[12] и FLengthFilter = 4
	// ippsWTFwdSetDlyLine_32f( state, &src[10], &src[10]);
	// ippsWTFwd_32f (src, low, high, 6, state);
	int n = m_pSize[0] - FLengthFilter + 2;
/*	curStatus1 = ippsWTFwdSetDlyLine_32f(pFwdState, pSignal+n, pSignal+n);
	curStatus  = ippsWTFwd_32f(pSignal, pBufA->GetGlobalAddres(),
					pBufD->GetGlobalAddres(), m_pTree[1], pFwdState);*/
	return (curStatus == ippStsNoErr) && (curStatus1 == ippStsNoErr);
}
//--------------------------------------------------------------------------
/*bool CPacketFDWT::InitInverse()
{
	curStatus1 = ippsWTInvInitAlloc_32f(&pInvState,
		pTabs_invLo, FLengthFilter, 0,
		pTabs_invHi, FLengthFilter, 0);
	curStatus  = ippsWTInvSetDlyLine_32f( pInvState,
		pBufA->GetGlobalAddres(), pBufD->GetGlobalAddres() );

	return (curStatus == ippStsNoErr) && (curStatus1 == ippStsNoErr);
}*/
//--------------------------------------------------------------------------
/*bool CPacketFDWT::InitInverseOne()
{
	return InitInverse();
}*/
//--------------------------------------------------------------------------
/*bool CPacketFDWT::Inverse(float *pSignal)
{
	int n = ( FLengthFilter - 1 ) / 2;
	curStatus = ippsWTInv_32f(
		pBufA->GetGlobalAddres() + n,
		pBufD->GetGlobalAddres() + n,
		pBufA->GetSize() - n, pSignal, pInvState );

	return curStatus == ippStsNoErr;
}*/
//--------------------------------------------------------------------------
/*bool CPacketFDWT::InverseOne(float *pSignal)
{	// при dst[12] и FLengthFilter = 4
	// ippsWTInv_32f(low+1, high+1, 5, dst, state);
	// ippsWTInv_32f(low, high, 1, dst+10, state);
	int n = ( FLengthFilter - 1 ) / 2;
	curStatus1 = ippsWTInv_32f(
		pBufA->GetGlobalAddres() + n,
		pBufD->GetGlobalAddres() + n,
		pBufA->GetSize() - n, pSignal, pInvState );
	curStatus = ippsWTInv_32f(
		pBufA->GetGlobalAddres(),
		pBufD->GetGlobalAddres(),
		n, pSignal + m_pTree[0] - 2*n, pInvState );
	
	return (curStatus == ippStsNoErr) && (curStatus1 == ippStsNoErr);
}*/
//--------------------------------------------------------------------------
/*float* CPacketFDWT::BandPointerData(int band)
{
	float *add;
	if ( (band >= 0) && (band < m_pNumBuf[FLevel]) )
		add = pppBuf[FLevel][band]->GetGlobalAddres();
	else
		add = NULL;
	return add;
}*/
//--------------------------------------------------------------------------
float CPacketFDWT::BandMiddleFreq(float *pFreqADC, int band)
{
	float df = BandWidth(pFreqADC);
	return df * ( (float)band + 0.5f ); 
}
//--------------------------------------------------------------------------
void CPacketFDWT::BandMiddleFreqAll(float *pFreqADC, float *pFreqBand)
{
	float df = BandWidth(pFreqADC);
	float f = df * 0.5f;
	for(int i=0; i<m_pNumBuf[FLevel]; i++)
	{
		pFreqBand[i] = f;
		f += df;
	}
}
//--------------------------------------------------------------------------