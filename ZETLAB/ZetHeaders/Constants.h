//--------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <vector>
#include "ATLComTime.h"
#include <mathZet.h>
#include <Quality.h>
//--------------------------------------------------------------------------
const COLORREF ColorFont  = RGB(1, 203, 247);
const COLORREF ColorOk    = RGB(23, 23, 23);
const COLORREF ColorBad   = RGB(120, 120, 120);
const COLORREF ColorError = RGB(200, 200, 200);
const COLORREF ColorCalibr = RGB(250, 80, 80);
//------------------------------------------------------------------------------
// ���������, ������������ �� ������ ����������
//----------------------------------------------------------------------
// ��������� ���������
//---------------------
const CString Str_n = L"\n";
const CString Str_t = L"\t";
const CString StrStars = L"*****";
const CString FormatForDTU = L"%.6f";
const CString g_sCreate(L"-createprocess");
const CString g_sCreateFormat(g_sCreate + L"_%u_%d");
//----------------------------------------------------------------------
// ��������� ���������
//---------------------
const float fOne = 1.f;
const float fZero = 0.f;
const float g = 9.807f;
const double constD_180_pi = 180. / M_PI;
const double constD_pi_180 = M_PI / 180.;
const float const_180_pi = float(constD_180_pi);
const float const_pi_180 = float(constD_pi_180);
const float REarthInkm = 6371.3f; // ������� ������ �����

// ������� ��� ����� �����
const double SecAsPartOfDay = 1. / (24. * 60. * 60.);
//----------------------------------------------------------------------
// �����, ������������ � ��������
//--------------------------------
const unsigned long ColorCrs = RGB(32,  32,  192);	// ������
const unsigned long ColorDig = RGB(64,  64,    0);	// ����� �� ����
const unsigned long ColorFon = RGB(240, 240, 240);	// ���
const unsigned long ColorGrd = RGB(192, 192,   0);	// �����
const unsigned long ColorLeg = RGB(128,   0,   0);	// �������
const unsigned long ColorGrf = RGB(172,  22,  39);	// ������
//------------------------------------------------------------------------------
// ��������� �������, ����������� ���������
template <class T>		// T - ��� ����� ����� �����.
T Factorial(T n)		// �� ����� ���� ������������
{
	T ret = 1;			// ������, ��� 1!, ������ 1, ���� ��������
	if ( n > 1 )
	{
		for(T i=2; i<=n; i++)
			ret *= i;
	}
	return ret;
}
//------------------------------------------------------------------------------
// ��������� �������, ����������� ������� "� �� n �� k", ������ n!/[k!*(n-k)!]
template <class T>		// T - ��� ����� ����� �����.
T CfromNbyK(T n, T k)	// �� ����� ���� ������������
{
	T n1, n2, ret;
	if ( (k > 0) && (n > k) )
	{	// ������� �������� n!/n1!, ��� n1 = max(k, (n-k))
		n2 = n - k;
		if ( k > n2 )
			n1 = k;
		else
		{
			n1 = n2;
			n2 = k;
		}
		ret = ++n1;
		for(T i=++n1; i<=n; i++)
			ret *= i;
		// �������� ��������� �������� �� n2!, ��� n2 = min(k, (n-k))
		ret /= Factorial(n2);
	}
	else
		ret = 1;
	return ret;
}
//------------------------------------------------------------------------------
// ��������� �������, ����������� ������ - ����, ������������� �� ����������� ��
// ��� Y �� ������� �������. ���� �������������� � ��������, �� ������ ����
// � ��������� [0, 360)
template <class T>			// T - ��� float, double, long double
T Azimuth(					// ������ � ����.
	T x,					// ���������� X
	T y,					// ���������� Y
	bool bInvers = false)	// ��������� ��������������� �����������
{	// ������� ������ ������� (� ������ ����������) ���� � ����.
	T ret = atan2(y, x) * T(constD_180_pi);		// �������� (-180, +180]
	if (bInvers)
	{	// ��������������� �����������, �������� (-180, +180]
		if ( ret > T(0) )
			ret -= T(180);
		else
			ret += T(180);
	}
	// ������� ���� �� ��������� (-180, +180] � ������
	ret = T(90) - ret;
	// ���� ����, �� ����� � �������� [0, 360)
	if ( ret < T(0) )
		ret += T(360);
	return ret;
}
//------------------------------------------------------------------------------
// ��������� �������, ����������� ���� �����, �������������� � ��������
template <class T>			// T - ��� float, double, long double
T AngleOfElevation(T x, T y, T z)	// ���� ����� � ����.
{
	return atan( z / sqrt(x*x + y*y) ) * T(constD_180_pi);
}
//------------------------------------------------------------------------------
// ��������� �������, ����������� ����� �� ��������� ���-�� �������� ����
template <class T>				// T - ��� int, float, double
T Rounding(	const T val,		// ����������� �����
			const int num = 0)	// ���-�� ����������� �������� ����,
// ������������ ����� - ���������� �����
{// ���� num = 0, �� ��������� ����������� (���� ��� ��� �������� �����)
	int numDig;	// ���-�� �������� ���� � ����������
	T x, ret, dx;
	bool bMinus = val < T(0);
	if ( ( num >= 0 ) && ( val != T(0) ) )	// ������������� ���-�� ����
	{// �� ����� ����, ��� � ����������� ����� �� ����� ��������� 0
		if (bMinus)	// �������� �� ���������������
			x = -val;
		else
			x = val;
		// ��������� ������� ��������� �������� ����
		if (num != 0)
			numDig = num;	// ���-�� ������
		else
		{// ���������� �����������
			ret = Rounding(x, 2);
			dx = T(pow(10., int(floor(log10((double)ret))) - 1));
			if ( int(ret/dx) < 20 )
				numDig = 2;
			else
				numDig = 1;
		}
		// ���� ���������� ( floor(-2.8) = -3 �  floor(2.8) = 2 )
		dx = T(pow(10., int(floor(log10((double)x))) - numDig + 1));
		ret = T( int(x / dx + 0.5) ) * dx;
		// �������� �� ���������������
		if (bMinus)
			ret = -ret;
	}
	else
		ret = val;	// ��������� ������
	return ret;
}
//------------------------------------------------------------------------------
// ��������� �������, ����������� ������ ��������� ���������.
// �������� ���������� �� ����������� ��� �� ��������
template <class T>			// T - ��� ����� ��������, � ������� ����������
void SortYM(				//		��������� ">" � "="
	T *pT,					// ��������� �� ������
	int size,				// ������ �������
	bool flagUp = true)		// ���������� �� �����������
{
	T x0 = *pT, x1;
	bool flag;
	for(int i=0; i<size-1; i++)
	{
		x1 = pT[i+1];
		flag = x1 > x0;
		if (flagUp)
			flag = !flag;
		if (flag)
		{
			pT[i] = x1;
			pT[i+1] = x0;
		}
		else
			x0 = x1;
	}
	if (size > 2)
		SortYM(pT, size-1, flagUp);
}
//------------------------------------------------------------------------------
// ��������� �������, ������������ ���������� ����������� ���������
// �������� �������� �������. ������� �� ������ ������������.
template <class T>			// T - ��� ����� �����
int CounterExtremum(// �����. �������� - ���-�� �����������
	T *padd,	// �������� ���
	int size,	// ��� ������
	bool bMax,	// �����: 1 - ���������, 0 - ��������
	int zzX,	// ���-��� �� �
	T zzY,		// ���-��� �� Y
	PVOID pval,	// ������ �������� �����������, ���� pval != NULL
	std::vector<int> * pInd)// ������ �������� �����������, ��� NULL
{
	if (size < 3)
		return 0;
	int indRetStart, ret = 0, i = 2;
	bool flag,			// y2 > y1
		bGoUp,			// ����������� �������� �����
		bGoRet = false;	// �������� ��������
	T dy, valRetStart;
	std::vector<T> *pVal = (std::vector<T>*)pval;
	if (pVal)
		pVal->clear();
	if (pInd)
		pInd->clear();

	T y1 = padd[0];
	T y2 = padd[1];
	while ( y2 == y1 )
		y2 = padd[i++];
	flag = bGoUp = y2 > y1;

	for(; i<size; i++)
	{
		y1 = y2;
		y2 = padd[i];
		flag = y2 > y1;

		if ( ( ( flag && !bGoUp && !bGoRet) ||
			   (!flag &&  bGoUp && !bGoRet) ) && (y2 != y1) )
		{// ������ ��������� ����
			if ( (zzX == 0) && ( (y1 > y2) == bMax ) )
			{
				dy = y2 - y1;
				if ( dy < 0 )
					dy = -dy;
				if ( dy > zzY )
				{// ��� ���������, ������� �� ����
					if (pVal)
						pVal->push_back(y1);
					if (pInd)
						pInd->push_back(i-1);
					ret++;
					bGoRet = false;
					bGoUp = !bGoUp;
				}
			}
			else
			{
				indRetStart = i - 1;
				valRetStart = y1;
				bGoRet = true;
			}
		}
		else if ( ( ( flag &&  bGoUp && bGoRet) ||
				    (!flag && !bGoUp && bGoRet) ) && (y2 != y1) )
		{// ����� ��������� ����
			if ( zzX > 0 )
			{// ��� ���������, ������� �� ����
				if ( ( bGoUp == bMax ) && (i - indRetStart - 1) > zzX )
				{// � �������� �� �, ����� (i - indRetStart - 1), �.�.
					dy = y2 - valRetStart;	// ����� ��������� ����
					if ( dy < 0 )
						dy = -dy;
					if ( dy > zzY )
					{// � �������� �� Y
						if (pVal)
							pVal->push_back(valRetStart);
						if (pInd)
							pInd->push_back(indRetStart);
						ret++;	// ����������� ������� �������!!!
					}
				}
			}
			else
			{// ���� ��� �����
				if ( (y1 > y2) == bMax )
				{// ��� ���������, ������� �� ����
					dy = y2 - y1;
					if ( dy < 0 )
						dy = -dy;
					if ( dy > zzY )
					{// � ���� ��������� ��������
						if (pVal)
							pVal->push_back(y1);
						if (pInd)
							pInd->push_back(i-1);
						ret++;	// ����������� ������� �������!!!
					}
				}
			}
			bGoRet = false;
		}
		else if ( ( ( ( flag && !bGoUp && bGoRet) ||
				  (!flag &&  bGoUp && bGoRet) ) ) || ( (y2 == y1) && bGoRet ) )
		{// ���������� �������� ���
			if ( (i - indRetStart) > zzX )
			{// ����� �� ����� �� �
				dy = y2 - valRetStart;
				if ( dy < 0 )
					dy = -dy;
				if ( dy > zzY )
				{// ����� �� ����� �� Y - ���������
					if (bGoUp == bMax)
					{// ��� ���������
						if (pVal)
							pVal->push_back(valRetStart);
						if (pInd)
							pInd->push_back(indRetStart);
						ret++;
					}
					bGoUp = !bGoUp;
					bGoRet = false;
				}
			}
		//	else �� ����� �� ������� �� �, ������ �� �����
		}
	//	else ���������� ������ ���, ������ �� ������
	}//	for(; i<size; i++)
	return ret;
}
//------------------------------------------------------------------------------
// ��������� �������, �������������� ����������� �� ��������� ����.
// ��� sizeHistogram = 0 ���-�� ���������� ����������� ������������ ��
// ������� �������� ������
template <class T>			// T - ��� float ��� double, � ����� �� ����������� 
int* Histogram(
	T *padd,				// �������� ������
	int size,				// �� ������
	int *pValHistogram,		// ��. �� ������ ����������� ��� NULL
	int *pSizeHistogram,	// ���-�� ���������� �����������
	T *pValItem0Histogram,	// �������� �������� 0-��� ��������� �����������
	T *pWidthItemHistogram,	// ������ ���������� �����������
	int NumMaxHistogram = 1,// ���-�� ��������� ����������
	int sizeHistogram = 0)	// �������� ���-�� ���������� �����������
{
	int i, num;
	T a0, da, amin, amax;
	// ��������� ���-�� ���������� -----------------------------------
	if (sizeHistogram == 0)
	{
		num = int(pow((float)size, 0.4f));
		if (NumMaxHistogram < 2)
			num = int(1.25f * num);
		else
			num *= 2;
		num |= 1;
	}
	else
		num = sizeHistogram;
	if (num < 3)	// ������ ������, �.�. ����� � ���������
		num = 3;	// ����� num-2

	// ��������� ������ ���������� -----------------------------------
	amin = min(padd[0], padd[1]);
	amax = max(padd[0], padd[1]);
	for (i = 2; i < size; i++)
	{
		a0 = padd[i];
		if (amin > a0)
			amin = a0;
		if (amax < a0)
			amax = a0;
	}

	if (amax == amin)
	{// �������� ������ - ��� ���� � ���� �����
		*pSizeHistogram = 0;
		*pValItem0Histogram = amin;
		*pWidthItemHistogram = T(0);
		if (pValHistogram)
		{
			delete[] pValHistogram;
			pValHistogram = nullptr;
		}
		return NULL;
	}

	da = (amax - amin) / (num - 2);
	a0 = amin - T(0.5) * da;

	// ������ ������ ��� ����������� ------------------------------------
	if (pValHistogram)
	{
		delete[] pValHistogram;
		pValHistogram = nullptr;
	}

		pValHistogram = new int[num];
		ZeroMemory(pValHistogram, num * sizeof(int));	// ����������� !!!


	// ��������� � ������ ����������� ------------------------------------
	*pSizeHistogram = num;
	*pValItem0Histogram = amin;
	*pWidthItemHistogram = da;
	// �.�. � �� int � amax != amin, �� da != 0
	for(i=0; i<size; i++)
		pValHistogram[ int( (padd[i] - a0) / da ) ]++;	
	return pValHistogram;

}
//------------------------------------------------------------------------------