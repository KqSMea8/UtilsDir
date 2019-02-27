//------------------------------------------------------------------------------
//	���� CSeismicSource_2D.cpp
//	������ �� 12.07.2012
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
#include "afx.h"
#include "StdAfx.h"
#include <FunctionSeismic\SeismicSource_2D.h>
//------------------------------------------------------------------------------
CSeismicSource_2D::CSeismicSource_2D()
{
	pGeog = NULL;
	pCart = NULL;
	pTp = NULL;
	pDist2 = NULL;
	pUncertDist = NULL;
}
//------------------------------------------------------------------------------
CSeismicSource_2D::~CSeismicSource_2D()
{
	if (pGeog)
	{
		delete [] pGeog;
		pGeog = nullptr;
	}
	if (pCart)
	{
		delete [] pCart;
		pCart = nullptr;
	}
	if (pTp)
	{
		delete [] pTp;
		pTp = nullptr;
	}
	if (pDist2)
	{
		delete [] pDist2;
		pDist2 = nullptr;
	}
	if (pUncertDist)
	{
		delete [] pUncertDist;
		pUncertDist = nullptr;
	}
}
//------------------------------------------------------------------------------
bool CSeismicSource_2D::Create(
	int number,		// ���-�� ��������������� � ������
	float dt,		// ����������� ����������� ���� �����, ���
	float vp,		// �������� ��������������� P-����, ��/���
	float dvp)		// ����������� ���� �������� ���������������, ��/���
{
	bool ret = (number > 2) && (dt > 0.f) && (vp > 0.f) && (dvp > 0.f);
	if (ret)
	{
		Number = number;
		NumberHyperbole = CfromNbyK(Number, 2);
		Vp = vp;
		dVp = dvp;
		dT = dt;
		AccCoord = dT * Vp;
		pGeog = new Point2D [Number];
		pCart = new Point2D [Number];
		pTp = new DATE [Number];
		pDist2 = new float [NumberHyperbole];
		pUncertDist = new float [NumberHyperbole];
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CSeismicSource_2D::SetNumber(int num)
{
	bool ret;
	if ( (Number != num) && (num > 2) )
	{
		CSeismicSource_2D::~CSeismicSource_2D();
		ret = Create(num, dT, Vp, dVp);
	}
	else if (Number == num)
		ret = true;
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
bool CSeismicSource_2D::SetVp(float val)
{
	bool ret;
	if (val > 0.f)
	{
		ret = true;
		Vp = val;
	}
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
bool CSeismicSource_2D::SetUncertaintyVp(float val)
{
	bool ret;
	if (val > 0.f)
	{
		ret = true;
		dVp = val;
	}
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
void CSeismicSource_2D::ConvertGeographyInCartesian()
{// ������ ���������� ��������� ����������������
	float d_lati, d_long, cosCenter = cos(GeogCenter2D.y * const_pi_180);
	for(int i=0; i<Number; i++)
	{
		d_lati = (pGeog[i].y - GeogCenter2D.y) * const_pi_180;	// ������
		d_long = (pGeog[i].x - GeogCenter2D.x) * const_pi_180;	// �������
		pCart[i].y = REarthInkm * d_lati * cosCenter;
		pCart[i].x = REarthInkm * d_long;
	}
}
//------------------------------------------------------------------------------
void CSeismicSource_2D::ConvertCartesianInGeography()
{// ������ �������������� ��������� ���������
	float d_lati = SourceCart.y / (REarthInkm * cos(GeogCenter2D.y * const_pi_180));
	float d_long = SourceCart.x / REarthInkm;
	SourceGeog.y = GeogCenter2D.y + d_lati * const_180_pi;
	SourceGeog.x = GeogCenter2D.x + d_long * const_180_pi;
}
//------------------------------------------------------------------------------
bool CSeismicSource_2D::CalcDistance()
{// ������ ��������� ��� 2D
	int i, n, m;
	float a, b, d;
	bool ret = true;
	if (ret)
		for(i=0; i<Number; i++)
			ret = ret && (pTp[i] > 0.);
	if (ret)
	{
		m = 0;
		for(i=0; i<Number-1; i++)
		for(n=i+1; n<Number; n++)
		{
			a = float( fabs( double(pTp[i] - pTp[n]) ) / SecAsPartOfDay );
			d = a * Vp;
			pDist2[m] = d * d;
			b = a * dVp / Vp / Vp;
			a = dT / Vp;
			pUncertDist[m++] = sqrt( 2 * a * a + b * b );
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CSeismicSource_2D::SetCenterFor2D()
{
	bool ret;
	GeogCenter2D = Point2D();	// ���� ����� �����������
	for(int i=0; i<Number; i++)
	{
		ret = (pGeog[i].x != 0.f) && (pGeog[i].y != 0.f);
		if (ret)
			GeogCenter2D += pGeog[i];
		else
			break;
	}
	if (ret)
		GeogCenter2D /= Number;	
	return ret;
}
//------------------------------------------------------------------------------
Point2D CSeismicSource_2D::SetFirstPoint()
{
	int n = 0;
	double _min = pTp[0];
	for(int i=1; i<Number; i++)
	{
		if ( _min > pTp[i] )
		{
			_min = pTp[i];
			n = i;
		}
	}
	return pCart[n]*0.5f;
}
//------------------------------------------------------------------------------
float CSeismicSource_2D::GetLatitude(	// ������ � ����.
	const int num)	// ����� ��������������, ���� �� 1
{
	float ret;
	if ( (num > 0) && (num <= Number) )
		ret = pGeog[num - 1].y;
	else
		ret = 0.f;
	return ret;
}
//------------------------------------------------------------------------------
float CSeismicSource_2D::GetLongitude(	// ������� � ����.
	const int num)	// ����� ��������������, ���� �� 1
{
	float ret;
	if ( (num > 0) && (num <= Number) )
		ret = pGeog[num - 1].x;
	else
		ret = 0.f;
	return ret;
}
//------------------------------------------------------------------------------
bool CSeismicSource_2D::SetLatitude(
	const int num,	// ����� ��������������, ���� �� 1
	float val)		// ������ � ����.
{
	bool ret = (num > 0) && (num <= Number) &&
		(val > -90.f) && (val < 90.f);
	if (ret)
		pGeog[num - 1].y = val;
	return ret;
}
//------------------------------------------------------------------------------
bool CSeismicSource_2D::SetLongitude(
	const int num,	// ����� ��������������, ���� �� 1
	float val)		// ������� � ����.
{
	bool ret = (num > 0) && (num <= Number) &&
		(val > -180.f) && (val <= 180.f);
	if (ret)
		pGeog[num - 1].x = val;
	return ret;
}
//------------------------------------------------------------------------------
DATE CSeismicSource_2D::GetTp(
	int num)	// ����� ��������������, ���� �� 1
{
	DATE ret;
	if ( (num > 0) && (num <= Number) )
		ret = pTp[num - 1];
	else
		ret = 0.;
	return ret;
}
//------------------------------------------------------------------------------
bool CSeismicSource_2D::SetTp(
	int num,	// ����� ��������������, ���� �� 1
	DATE val)
{
	bool ret = (val > 0.) && (num > 0) && (num <= Number);
	if (ret)
		pTp[num - 1] = val;
	return ret;
}
//------------------------------------------------------------------------------
// ���������� ������ ���������� ��������� ��������� ���� ������ ��������
// ������� ������� ������� ����������� ���������� � �������� ���� �������� �
// ������������ � ���������� ��������-�����
//------------------------------------------------------------------------------
// �������� �������
long CSeismicSource_2D::FindSource()
{
	// ����������� ������� �����
	SetCenterFor2D();
	// ������ ���������� ��������� ���� ��������������� (������� SetCenterFor2D)
	ConvertGeographyInCartesian();
	
	// �������� ��� ------------------------------------------
/*	Point2D Ps = Point2D(1.f, 0.f);		// ���� ������ ���������� ������� ���������
	DATE T0 = pTp[0] - ( Ps - pCart[0] ).abs() / Vp * SecAsPartOfDay;
	for(int i=1; i<Number; i++)
		pTp[i] = T0 + ( Ps - pCart[i] ).abs() / Vp * SecAsPartOfDay;
*/	// ����� ��������� ���� ----------------------------------

	// ����������� ������� ���������
	CalcDistance();
	// ����� ���������
	long ret = ConjGrad_FletcherReeves();
	// ������ �������������� ��������� ���������
	ConvertCartesianInGeography();
	// ������ ������� ���������������
	CalcUncertaintyRadius();
	// ������ ������� ������� � � ���������������
	CalcSourceTime();
	return ret;	// ��������� ���-�� ��������
}
//------------------------------------------------------------------------------
// ���������� ������ ����������� ����������  � ������������ � ����������
// ��������-�����.
long CSeismicSource_2D::ConjGrad_FletcherReeves()
{
	long iter = 0;
	Point2D
		P0,		// �������� ����� �� �����������
		S0,		// ���������� �����������
		S1,		// ������� �����������
		grad0,	// ���������� ��������
		grad1;	// ������� ��������
	float val0, val1, acc2 = AccCoord * AccCoord;

	// ��������� �����
	P0 = SetFirstPoint();

	// 0-�� ��������
	val0 = CalcFunctionAndGrad(&P0, &grad1);
	S1 = -grad1;
	// �� 0-�� �������� ����� ������ ����� �����, ��� �������� ����������
	FindMin(&P0, &val0, &S1, &SourceCart, &val1, 10*AccCoord);

	do
	{// k-�� ��������
		P0 = SourceCart;
		val0 = val1;
		grad0 = grad1;
		S0 = S1;
		CalcGrad(&P0, &grad1);
		S1 = S0 * (grad1.norm() / grad0.norm()) - grad1;
		if ( !FindMin(&P0, &val0, &S1, &SourceCart, &val1, AccCoord) )
		{
			S1 = -grad1;
			FindMin(&P0, &val0, &S1, &SourceCart, &val1, AccCoord);
			// ���� ������� ����� ����� � �������� ������� ������ �� do{},
			// ���� ������ ������� � SourceCart = �0
		}
		iter++;
	}
	while ( (SourceCart - P0).norm() > acc2 );
	return iter;
}
//------------------------------------------------------------------------------
// ����� �������� ������� �� �������� �����������
bool CSeismicSource_2D::FindMin(
	Point2D *pP0,	// �������� �����
	float *pValP0,	// �������� ������� � �������� �����
	Point2D *pS,	// ����������� ������
	Point2D *pP1,	// ��������� �����
	float *pValP1,	// �������� ������� � ��������� �����
	float accuracy)	// �������� ����������� ��������
{
	float val0, val1 = *pValP0;
	Point2D Pcur, P1 = *pP0, dP = 0.5f * accuracy * pS->one();
	Point2D dir = pS->one();
	do
	{
		Pcur = P1;
		val0 = val1;
		P1 = Pcur + dP;
		val1 = CalcFunction(&P1);
	}
	while ( val1 < val0 );
	*pValP1 = val0;
	*pP1 = Pcur;
	return *pP0 != Pcur;
}
//------------------------------------------------------------------------------
// ������ �������� � ��������� ������� ������������� �������������
float CSeismicSource_2D::_FunctionAndGrad(
	Point2D *pP,		// ��. �� �����, �������� �������
	Point2D *pGrad,		// ��. �� ������������ �������� ��� NULL
	bool needFunction)	// ����, ��� ��������� �������� �������
{
	Point2D grad, Pn, Pm;	// ����������� �� ���������, � ����������
	float a, b, ret = 0.f;
	int i = 0;
	for(int n=0; n<Number-1; n++)
	for(int m=n+1; m<Number; m++)
	{
		Pn = *pP - pCart[n];
		Pm = *pP - pCart[m];
		a = Pn.abs() - Pm.abs();
		b = a * a - pDist2[i++];
		if (needFunction)
			ret += b * b;
		if (pGrad)/*
		{
			Point2D gg = ( a * b ) * ( Pn.one() - Pm.one() );
			grad += gg;
		}
			//*/grad += ( a * b ) * ( Pn.one() - Pm.one() );
	}
	if (pGrad)
		*pGrad = 4 * grad;
	return ret;
}
//------------------------------------------------------------------------------
// ������ �������� ������� ������������� ������������� � ����� pP
float CSeismicSource_2D::CalcFunction(
	Point2D *pP)		// �������� �������
{
	return _FunctionAndGrad(pP, NULL, true);
}
//------------------------------------------------------------------------------
// ������ �������� ��������� ������� ������������� ������������� � ����� pP
void CSeismicSource_2D::CalcGrad(
	Point2D *pP,	// �������� �������
	Point2D *pGrad)	// ������������ ��������
{
	_FunctionAndGrad(pP, pGrad, false);
}
//------------------------------------------------------------------------------
// ������ �������� � ��������� ������� ������������� ������������� � ����� pP
float CSeismicSource_2D::CalcFunctionAndGrad(
	Point2D *pP,	// �������� �������
	Point2D *pGrad)	// ������������ ��������
{
	return _FunctionAndGrad(pP, pGrad, true);
}
//------------------------------------------------------------------------------
void CSeismicSource_2D::CalcUncertaintyRadius()
{
	float a, nsp=0.f, sko=0.f;
	int i, n, m = 0;
	for(i=0; i<Number-1; i++)
	for(n=i+1; n<Number; n++)
	{
		a = fabs( ( SourceCart - pCart[i] ).abs() - 
				  ( SourceCart - pCart[n] ).abs() ) - sqrt(pDist2[m]);
		sko += a * a;
		a = pUncertDist[m++];
		nsp += a * a;
	}
	UncertR = 2.f * sqrt( sko / (NumberHyperbole-1) + nsp / 3);
}
//------------------------------------------------------------------------------
void CSeismicSource_2D::CalcSourceTime()
{
	int i;
	SourceTime = 0.;
	DATE t, *pT0 = new DATE [Number];
	for(i=0; i<Number; i++)
	{
		t = pTp[i] - ( SourceCart - pCart[i] ).abs() / Vp *SecAsPartOfDay;
		SourceTime += t;
		pT0[i] = t;
	}
	SourceTime /= Number;

	double sko = 0.;
	for(i=0; i<Number; i++)
	{
		t = ( pT0[i] - SourceTime ) / SecAsPartOfDay;
		sko += t * t;
	}
	UncertT = 2.f * sqrt( (float)sko / (Number-1) + dT * dT / 3 );
	delete [] pT0;
	pT0 = nullptr;
}
//------------------------------------------------------------------------------