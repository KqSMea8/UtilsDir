//------------------------------------------------------------------------------
#include "StdAfx.h"
#include <Intel\ipp\ipps.h>
#include <FunctionSeismic\FunctionSeismic.h>
#include <Buffer\SmartArray.h>
//------------------------------------------------------------------------------
// ������� ������� ��������� � ������� (� �� ������������) �� ���������
// ����������� �������������� (������ �����) � ���������� (������ �����).
// ������, ��� ������������� ��������� �������������� ����� ����������.
//------------------------------------------------------------------------------
void DistanceAndBearing(
	// ������� ������
	float lat1,			// ������ ������ �����, ����
	float lat1_er,		// ����������� ������ ������ �����, ����
	float long1,		// ������� ������ �����, ����
	float long1_er,		// ����������� ������� ������ �����, ����
	float depth1,		// ������� ������ �����, ��
	float depth1_er,	// ����������� ������� ������ �����, ��
	float lat2,			// ������ ������ �����, ����
	float long2,		// ������� ������ �����, ����
	float heigth2,		// ������ ������ �����, �
	// �������� ������
	float *distance,	// ���������, ��
	float *distance_er,	// ����������� ���������, ��
	float *bearing,		// ������ �� ������ ����� �� ������
	float *bearing_er)	// ����������� �������, ����.
{
	float a, b, c;
	// ���������� ----------------------------------------
	float f = lat1 * const_pi_180;	// ������ � ��������
	float l = long1 * const_pi_180;// ������� � ��������
	float sinf1 = sin(f);
	float cosf1 = cos(f);
	float sinl1 = sin(l);
	float cosl1 = cos(l);

	float f2 = lat2 * const_pi_180;	// ������ � ��������
	float l2 = long2 * const_pi_180;// ������� � ��������
	float sinf2 = sin(f2);
	float cosf2 = cos(f2);
	float sinl2 = sin(l2);
	float cosl2 = cos(l2);

	// ������ ��������� ---------------------------------
	float h1 = REarthInkm - depth1;	// REarthInkm - ������� ������ ����� � ��
	float x = h1 * cosf1 * cosl1;
	float y = h1 * cosf1 * sinl1;
	float z = h1 * sinf1;
	float df = lat1_er * const_pi_180;
	float dl = long1_er * const_pi_180;
	float dz = depth1_er;

	float h2 = REarthInkm;
//	if ( heigth2 != -9999 ) ���������� ��-�� ����������� ������ ��������� 2014.06.02 pvs studio 
	if (IsNotEqual(heigth2, -9999.0f, 0.0001f))
		h2 += heigth2 * 0.001f;
	float x2 = h2 * cosf2 * cosl2;
	float y2 = h2 * cosf2 * sinl2;
	float z2 = h2 * sinf2;

	x -= x2;
	y -= y2;
	z -= z2;

	a = cosf1 * cosl1 * dz;
	b = sinf1 * cosl1 * df;
	c = cosf1 * sinl1 * dl;
	float dx = sqrt( a*a + h1*h1 * (b*b + c*c) );

	a = cosf1 * sinf1 * dz;
	b = sinf1 * sinl1 * df;
	c = cosf1 * cosl1 * dl;
	float dy = sqrt( a*a + h1*h1 * (b*b + c*c) );

	a = sinf1 * dz;
	b = h1 * cosf1 * df;
	dz = sqrt( a*a + b*b );

	*distance = sqrt( x*x + y*y + z*z );
	a = x * dx;
	b = y * dy;
	c = z + dz;
	*distance_er = sqrt( a*a + b*b + c*c ) / *distance;

	// ������ ������� -----------------------------------
	c = f - f2;	// �������� �����
	f +=f2;
	f *= 0.5f;	// ������� ������
	l -= l2;
	x = cos(f) * sin(l);
	y = sin(c);

	a = sin(f) * sin(l) * df;
	b = cos(f) * cos(l) * dl;
	dx = sqrt(a*a + b*b);
	dy = abs(cos(c)) * df;

	a = const_180_pi * atan2(x, y);
	if ( a < 0.f )
		a += 360.f;
	*bearing = a;
	a = x * x;
	b = y * y;
	*bearing_er = sqrt( b*dx*dx + a*dy*dy ) / (a + b) * const_180_pi;
}
//------------------------------------------------------------------------------
// ����������� �������������� ��������� � ����������
void ConvertGeographyInCartesian(PointAll *pP)
{
/*	pP->m_cart.x = pP->m_geog.m_long;
	pP->m_cart.y = pP->m_geog.m_lati;
	pP->m_cart.z = pP->m_geog.m_heig;*/

	float lat = pP->m_geog.m_lati * const_pi_180;
	float lon = pP->m_geog.m_long * const_pi_180;
	float r = REarthInkm + pP->m_geog.m_heig;
	float cos_lat = cos(lat);
	pP->m_cart.x = r * cos_lat * cos(lon);
	pP->m_cart.y = r * cos_lat * sin(lon);
	pP->m_cart.z = r * sin(lat);
}
//------------------------------------------------------------------------------
// ����������� ���������� ��������� � ��������������
void ConvertCartesianInGeography(PointAll *pP)
{
/*	pP->m_geog.m_long = pP->m_cart.x;
	pP->m_geog.m_lati = pP->m_cart.y;
	pP->m_geog.m_heig = pP->m_cart.z;*/

	pP->m_geog.m_long = const_180_pi * atan2( pP->m_cart.y, pP->m_cart.x );
	pP->m_geog.m_lati = const_180_pi *
		asin(pP->m_cart.z / sqrt( pP->m_cart.x * pP->m_cart.x +
								  pP->m_cart.y * pP->m_cart.y +
								  pP->m_cart.z * pP->m_cart.z ) );
	pP->m_geog.m_heig = pP->m_cart.abs() - REarthInkm;
}
//------------------------------------------------------------------------------
// ������ �������� ������� �������.
// ��������� ������� ��. � ������.
float CalcFunction(
	std::vector <PointAll> *pPoints,	// ����� ���������������
	PointXYZ *pP)						// ����� ���������
{
	float a, ret = 0.f;
	for(auto it=pPoints->begin(); it!=pPoints->end(); ++it)
	{
		a = ( *pP - it->m_cart ).norm() - it->m_dist * it->m_dist;
		ret += a * a;
	}
	return ret;
}
//------------------------------------------------------------------------------
// P����� ��������� ������� �������.
// ��������� ��� ����������� ��. � ������.
void CalcGrad(
	std::vector <PointAll> *pPoints,	// ����� ���������������
	PointXYZ *pP,						// ����� ���������
	PointXYZ *pGrad)					// ��������� �� ��������
{
	float c;
	PointXYZ Pi, dxyz;
	for(auto it=pPoints->begin(); it!=pPoints->end(); ++it)
	{
		Pi = *pP - it->m_cart;
		c = Pi.norm() - it->m_dist * it->m_dist;
		dxyz += c * Pi;
	}
	*pGrad = 4 * dxyz;
}
//------------------------------------------------------------------------------
// ����� �������� �� �������� �����������.
// �o���������� �������� - ����� ����� ������� �� ��������
bool FindMin(
	// ������� ���������
	std::vector <PointAll> *pPoints,	// ����� ���������������
	PointAll *pEpicentr,	// ����� ��������� (�������� ����������� ���������)
	PointXYZ *pP0,			// �������� �����
	float *pValP0,			// �������� ������� ������� � �������� �����
	PointXYZ *pS,			// ����������� ������
	// �������� ���������
	PointXYZ *pP1,			// ����� �����
	float *pValP1)			// �������� ������� ������� � ����� �����
{
	float val0, val1 = *pValP0;
	PointXYZ Pcur, P1 = *pP0, dP = *pS * (0.5f * pEpicentr->m_dist / pS->abs());
	do
	{
		Pcur = P1;
		val0 = val1;
		P1 = Pcur + dP;
		val1 = CalcFunction(pPoints, &P1);
	}
	while ( val1 < val0 );
	*pValP1 = val0;
	*pP1 = Pcur;
	return *pP0 != Pcur;
}
//------------------------------------------------------------------------------
// ���������� ������ ���������� ���������� � ������������ � ����������
// ��������-�����. ������������ �������� - ���-�� ����������� ��������.
// ��������� ������� ��. � ������.
long ConjGradFR(
	std::vector <PointAll> *pPoints,	// ����� ���������������
	PointAll *pEpicentr)	// ����� ��������� (�������� ����������� ���������)
{
	long iter = 0;
	PointXYZ S0, S1, P0, P1, grad0, grad1;
	float val0, val1, acc2 = pEpicentr->m_dist * pEpicentr->m_dist;
	float dx = pEpicentr->m_dist;

	// ��������� ����� - ��� ������� �����
	for(auto it = pPoints->begin(); it != pPoints->end(); ++it)
		P0 += it->m_cart;
	P0 = P0 / (float)pPoints->size();

	// 0-�� ��������
	val0 = CalcFunction(pPoints, &P0);
	CalcGrad(pPoints, &P0, &grad1);
	S1 = -grad1;
	// �� 0-�� �������� ����� ������ ����� �����, ��� �������� ����������
	pEpicentr->m_dist = 10 * dx;
	if ( FindMin(pPoints, pEpicentr, &P0, &val0, &S1, &P1, &val1) )
	{// ���� ����� ����� ������� �������� ������������ �������
		pEpicentr->m_dist = dx;	// ��������� ��������
		do
		{// k-�� ��������
			P0 = P1;	// ppoint[num].m_cart = P1; � FindMin		
			val0 = val1;
			grad0 = grad1;
			S0 = S1;
			CalcGrad(pPoints, &P0, &grad1);
			S1 = S0 * (grad1.norm() / grad0.norm()) - grad1;
			if ( !FindMin(pPoints, pEpicentr, &P0, &val0, &S1, &P1, &val1) )
			{
				S1 = -grad1;
				FindMin(pPoints, pEpicentr, &P0, &val0, &S1, &P1, &val1);
				// ���� ������� ����� ����� � �������� ������� ������ �� do{},
				// ���� ������ ������� � �1 = �0
			}
			iter++;
		}
		while ( (P1 - P0).norm() > acc2 );
		pEpicentr->m_cart = P1;
	}
	//else ��������� ��������� ����� ������ � �������
	return iter;
}
//------------------------------------------------------------------------------
// ����������� �������� ������ ��������� ���������.
// ����������� - ��� ������ ����� ��������������� (pEpicentr->m_impr).
// ��������� ������� ��. � ������.
void CalcImPrecition(
	std::vector <PointAll> *pPoints,	// ����� ���������������
	PointAll *pEpicentr)	// ����� ��������� (������ �����)
{
	float a, b, x = 0.f;
	for(auto it=pPoints->begin(); it!=pPoints->end(); ++it)
	{
		a = abs( (pEpicentr->m_cart - it->m_cart).abs()
			     - it->m_dist );
		b = it->m_impr;
		x +=  a * a + b * b / 3.f;
	}
	pEpicentr->m_impr = Rounding( sqrt( x / (float)(pPoints->size()-1) ) );
}
//------------------------------------------------------------------------------
// ������ ������ ��������� ��������� � ������ ����� ���������������.
// pEpic->m_impr - ������ ����� ���������������
long FindEpicenter(
	std::vector <PointAll> *pPoints,		// ����� ���������������
	PointAll *pEpicentr,					// ����� ���������
	float accuracy)	// �������� ����������� ���������� ��������� ���������, ��
{
	pEpicentr->m_dist = accuracy;

	// ������ ���������� ���������
	for(auto it=pPoints->begin(); it!=pPoints->end(); ++it)
		ConvertGeographyInCartesian(&(*it));
	
	// ����������� ������ ��������� ���������
	long ret = ConjGradFR(pPoints, pEpicentr);
	ConvertCartesianInGeography(pEpicentr);

	// ����������� ������ ���������������, ��
	CalcImPrecition(pPoints, pEpicentr);
	
	return ret;
}
//------------------------------------------------------------------------------
// ������ ���-�� ����������� �������� ��� �������� �������
void Calc_RoundNumberForTime(
	const float freqADC,// ������� �������������
	int & numTime)		// ���-�� ������������ ���� ����� ������� � ��������� �������
{
	numTime = 1 - CalcOrderOfValue(1.f / freqADC);
}
//------------------------------------------------------------------------------
// ������ ���-�� ����������� �������� ��� �������� ���������, �������� � �����������
// minAccel - ����������� �������� ���������
// freqMax - ������������ �������
// numA - ���-�� ������������ ���� ����� ������� � ��������� ���������
// numV - ���-�� ������������ ���� ����� ������� � ��������� ��������
// numX - ���-�� ������������ ���� ����� ������� � ��������� �����������
void Calc_RoundNumberForAVX(const float minAccel, const float freqMax,
	int & numA, int & numV, int & numX)
{
	numA = 1 - CalcOrderOfValue(minAccel);

	float w = (float)(IPP_2PI * freqMax);
	float val = minAccel / w;
	numV = 1 - CalcOrderOfValue(val);

	val /= w;
	numX = 1 - CalcOrderOfValue(val * 1000);		// ����������� � ��
}
//------------------------------------------------------------------------------
// ���������� �������� � ��������� PeakAccel
// sPeakAccel - ���������, � ������� ��������� ��������� ��������
// numA - ���-�� ������������ ���� ����� ������� � ��������� ���������
// numV - ���-�� ������������ ���� ����� ������� � ��������� ��������
// numX - ���-�� ������������ ���� ����� ������� � ��������� �����������
void Round_PeakAccel(Ssm_str_PeakAccel & sPeakAccel, const int numA, const int numV, const int numX)
{
	RoundToN_DigitsAfterPoint(sPeakAccel.peakAccelX, sPeakAccel.peakAccelX, numA);
	RoundToN_DigitsAfterPoint(sPeakAccel.peakAccelY, sPeakAccel.peakAccelY, numA);
	RoundToN_DigitsAfterPoint(sPeakAccel.peakAccelZ, sPeakAccel.peakAccelZ, numA);
	RoundToN_DigitsAfterPoint(sPeakAccel.peakAccelA, sPeakAccel.peakAccelA, numA);

	RoundToN_DigitsAfterPoint(sPeakAccel.peakVelocityX, sPeakAccel.peakVelocityX, numV);
	RoundToN_DigitsAfterPoint(sPeakAccel.peakVelocityY, sPeakAccel.peakVelocityY, numV);
	RoundToN_DigitsAfterPoint(sPeakAccel.peakVelocityZ, sPeakAccel.peakVelocityZ, numV);
	RoundToN_DigitsAfterPoint(sPeakAccel.peakVelocityA, sPeakAccel.peakVelocityA, numV);

	RoundToN_DigitsAfterPoint(sPeakAccel.peakMotionX, sPeakAccel.peakMotionX, numX);
	RoundToN_DigitsAfterPoint(sPeakAccel.peakMotionY, sPeakAccel.peakMotionY, numX);
	RoundToN_DigitsAfterPoint(sPeakAccel.peakMotionZ, sPeakAccel.peakMotionZ, numX);
	RoundToN_DigitsAfterPoint(sPeakAccel.peakMotionA, sPeakAccel.peakMotionA, numX);
}
//------------------------------------------------------------------------------
// ���������� �������� � ��������� sEventParam
// sEventParam - ���������, � ������� ��������� ��������� ��������
// numA - ���-�� ������������ ���� ����� ������� � ��������� ���������
// numV - ���-�� ������������ ���� ����� ������� � ��������� ��������
// numX - ���-�� ������������ ���� ����� ������� � ��������� �����������
// numT - ���-�� ������������ ���� ����� ������� � ��������� �������
void Round_EventParam(Ssm_str_EventParam & sEventParam, const int numA, const int numV, const int numX, const int numT)
{
	RoundToN_DigitsAfterPoint(sEventParam.peakAccelX, sEventParam.peakAccelX, numA);
	RoundToN_DigitsAfterPoint(sEventParam.peakAccelY, sEventParam.peakAccelY, numA);
	RoundToN_DigitsAfterPoint(sEventParam.peakAccelZ, sEventParam.peakAccelZ, numA);
	RoundToN_DigitsAfterPoint(sEventParam.peakAccelA, sEventParam.peakAccelA, numA);

	RoundToN_DigitsAfterPoint(sEventParam.peakVelocityX, sEventParam.peakVelocityX, numV);
	RoundToN_DigitsAfterPoint(sEventParam.peakVelocityY, sEventParam.peakVelocityY, numV);
	RoundToN_DigitsAfterPoint(sEventParam.peakVelocityZ, sEventParam.peakVelocityZ, numV);
	RoundToN_DigitsAfterPoint(sEventParam.peakVelocityA, sEventParam.peakVelocityA, numV);

	RoundToN_DigitsAfterPoint(sEventParam.peakMotionX, sEventParam.peakMotionX, numX);
	RoundToN_DigitsAfterPoint(sEventParam.peakMotionY, sEventParam.peakMotionY, numX);
	RoundToN_DigitsAfterPoint(sEventParam.peakMotionZ, sEventParam.peakMotionZ, numX);
	RoundToN_DigitsAfterPoint(sEventParam.peakMotionA, sEventParam.peakMotionA, numX);

	RoundToN_DigitsAfterPoint(sEventParam.mainPeriodX, sEventParam.mainPeriodX, numT);
	RoundToN_DigitsAfterPoint(sEventParam.mainPeriodY, sEventParam.mainPeriodY, numT);
	RoundToN_DigitsAfterPoint(sEventParam.mainPeriodZ, sEventParam.mainPeriodZ, numT);

	RoundToN_DigitsAfterPoint(sEventParam.signalSrcSNR, sEventParam.signalSrcSNR, 3);

	RoundToN_DigitsAfterPoint(sEventParam.dTps, sEventParam.dTps, numT);
	RoundToN_DigitsAfterPoint(sEventParam.durationEvent, sEventParam.durationEvent, numT);
	RoundToN_DigitsAfterPoint(sEventParam.endEventTimeSRV, sEventParam.endEventTimeSRV, numT);
}
//------------------------------------------------------------------------------
// ���������� �������� � ��������� sEventStation
// sEventStation - ���������, � ������� ��������� ��������� ��������
// numA - ���-�� ������������ ���� ����� ������� � ��������� ���������
// numV - ���-�� ������������ ���� ����� ������� � ��������� ��������
// numX - ���-�� ������������ ���� ����� ������� � ��������� �����������
// numT - ���-�� ������������ ���� ����� ������� � ��������� �������
void Round_EventStation(Ssm_str_EventStation & sEventStation, const int numA, const int numV, const int numX, const int numT)
{
	RoundToN_DigitsAfterPoint(sEventStation.peakAccelX, sEventStation.peakAccelX, numA);
	RoundToN_DigitsAfterPoint(sEventStation.peakAccelY, sEventStation.peakAccelY, numA);
	RoundToN_DigitsAfterPoint(sEventStation.peakAccelZ, sEventStation.peakAccelZ, numA);
	RoundToN_DigitsAfterPoint(sEventStation.peakAccelA, sEventStation.peakAccelA, numA);

	RoundToN_DigitsAfterPoint(sEventStation.peakVelocityX, sEventStation.peakVelocityX, numV);
	RoundToN_DigitsAfterPoint(sEventStation.peakVelocityY, sEventStation.peakVelocityY, numV);
	RoundToN_DigitsAfterPoint(sEventStation.peakVelocityZ, sEventStation.peakVelocityZ, numV);
	RoundToN_DigitsAfterPoint(sEventStation.peakVelocityA, sEventStation.peakVelocityA, numV);

	RoundToN_DigitsAfterPoint(sEventStation.peakMotionX, sEventStation.peakMotionX, numX);
	RoundToN_DigitsAfterPoint(sEventStation.peakMotionY, sEventStation.peakMotionY, numX);
	RoundToN_DigitsAfterPoint(sEventStation.peakMotionZ, sEventStation.peakMotionZ, numX);
	RoundToN_DigitsAfterPoint(sEventStation.peakMotionA, sEventStation.peakMotionA, numX);

	RoundToN_DigitsAfterPoint(sEventStation.mainPeriodX, sEventStation.mainPeriodX, numT);
	RoundToN_DigitsAfterPoint(sEventStation.mainPeriodY, sEventStation.mainPeriodY, numT);
	RoundToN_DigitsAfterPoint(sEventStation.mainPeriodZ, sEventStation.mainPeriodZ, numT);

	RoundToN_DigitsAfterPoint(sEventStation.magnitude, sEventStation.magnitude, 1);
	RoundToN_DigitsAfterPoint(sEventStation.distance, sEventStation.distance, 1);
	RoundToN_DigitsAfterPoint(sEventStation.signalSrcSNR, sEventStation.signalSrcSNR, 3);
	RoundToN_DigitsAfterPoint(sEventStation.intensity, sEventStation.intensity, 1);
}
//------------------------------------------------------------------------------