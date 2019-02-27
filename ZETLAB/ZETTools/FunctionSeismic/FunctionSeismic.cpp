//------------------------------------------------------------------------------
#include "StdAfx.h"
#include <Intel\ipp\ipps.h>
#include <FunctionSeismic\FunctionSeismic.h>
#include <Buffer\SmartArray.h>
//------------------------------------------------------------------------------
// Функция расчёта дистанции и пеленга (и их погрешностей) по известным
// координатам сейсмоприёмника (первая точка) и гипоцентра (вторая точка).
// Считаю, что погрешностями координат сейсмоприёмника можно пренебречь.
//------------------------------------------------------------------------------
void DistanceAndBearing(
	// входные данные
	float lat1,			// широта первой точки, град
	float lat1_er,		// погрешность широты первой точки, град
	float long1,		// долгота первой точки, град
	float long1_er,		// погрешность долготы первой точки, град
	float depth1,		// глубина первой точки, км
	float depth1_er,	// погрешность глубины первой точки, км
	float lat2,			// широта второй точки, град
	float long2,		// долгота второй точки, град
	float heigth2,		// высота второй точки, м
	// выходные данные
	float *distance,	// дистанция, км
	float *distance_er,	// погрешность дистанции, км
	float *bearing,		// пеленг из первой точки во вторую
	float *bearing_er)	// погрешность пеленга, град.
{
	float a, b, c;
	// переменные ----------------------------------------
	float f = lat1 * const_pi_180;	// широта в радианах
	float l = long1 * const_pi_180;// долгота в радианах
	float sinf1 = sin(f);
	float cosf1 = cos(f);
	float sinl1 = sin(l);
	float cosl1 = cos(l);

	float f2 = lat2 * const_pi_180;	// широта в радианах
	float l2 = long2 * const_pi_180;// долгота в радианах
	float sinf2 = sin(f2);
	float cosf2 = cos(f2);
	float sinl2 = sin(l2);
	float cosl2 = cos(l2);

	// расчет дистанции ---------------------------------
	float h1 = REarthInkm - depth1;	// REarthInkm - средний радиус Земли в км
	float x = h1 * cosf1 * cosl1;
	float y = h1 * cosf1 * sinl1;
	float z = h1 * sinf1;
	float df = lat1_er * const_pi_180;
	float dl = long1_er * const_pi_180;
	float dz = depth1_er;

	float h2 = REarthInkm;
//	if ( heigth2 != -9999 ) исправлена из-за исправления ошибки найденной 2014.06.02 pvs studio 
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

	// расчет пеленга -----------------------------------
	c = f - f2;	// разность широт
	f +=f2;
	f *= 0.5f;	// средняя широта
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
// Конвертация географических координат в декартовые
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
// Конвертация декартовых координат в географические
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
// Расчёт величины функции невязки.
// Выражение функции см. в теории.
float CalcFunction(
	std::vector <PointAll> *pPoints,	// точки сейсмоприёмников
	PointXYZ *pP)						// точка аргумента
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
// Pасчёт градиента функции невязки.
// Выражения для производных см. в теории.
void CalcGrad(
	std::vector <PointAll> *pPoints,	// точки сейсмоприёмников
	PointXYZ *pP,						// точка аргумента
	PointXYZ *pGrad)					// указатель на градиент
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
// Поиск минимума на заданном направлении.
// Вoзвращаемое значение - новая точка отлична от исходной
bool FindMin(
	// входные параметры
	std::vector <PointAll> *pPoints,	// точки сейсмоприёмников
	PointAll *pEpicentr,	// точка эпицентра (точность определения координат)
	PointXYZ *pP0,			// исходная точка
	float *pValP0,			// значение функции невязки в исходной точке
	PointXYZ *pS,			// направление спуска
	// выходные параметры
	PointXYZ *pP1,			// новая точка
	float *pValP1)			// значение функции невязки в новой точке
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
// Реализация метода сопряжённых градиентов в соответствии с алгоритмом
// Флетчера-Ривса. Возвращаемое значение - кол-во выполненных итераций.
// Расчетные формулы см. в теории.
long ConjGradFR(
	std::vector <PointAll> *pPoints,	// точки сейсмоприёмников
	PointAll *pEpicentr)	// точка эпицентра (точность определения координат)
{
	long iter = 0;
	PointXYZ S0, S1, P0, P1, grad0, grad1;
	float val0, val1, acc2 = pEpicentr->m_dist * pEpicentr->m_dist;
	float dx = pEpicentr->m_dist;

	// Начальная точка - это средняя точка
	for(auto it = pPoints->begin(); it != pPoints->end(); ++it)
		P0 += it->m_cart;
	P0 = P0 / (float)pPoints->size();

	// 0-ая итерация
	val0 = CalcFunction(pPoints, &P0);
	CalcGrad(pPoints, &P0, &grad1);
	S1 = -grad1;
	// на 0-ой итерации можно искать точку грубо, это сократит вычисления
	pEpicentr->m_dist = 10 * dx;
	if ( FindMin(pPoints, pEpicentr, &P0, &val0, &S1, &P1, &val1) )
	{// если новая точка найдена запускаю итерационный процесс
		pEpicentr->m_dist = dx;	// возвращаю точность
		do
		{// k-ая итерация
			P0 = P1;	// ppoint[num].m_cart = P1; в FindMin		
			val0 = val1;
			grad0 = grad1;
			S0 = S1;
			CalcGrad(pPoints, &P0, &grad1);
			S1 = S0 * (grad1.norm() / grad0.norm()) - grad1;
			if ( !FindMin(pPoints, pEpicentr, &P0, &val0, &S1, &P1, &val1) )
			{
				S1 = -grad1;
				FindMin(pPoints, pEpicentr, &P0, &val0, &S1, &P1, &val1);
				// либо найдена новая точка и проверяю условие выхода из do{},
				// либо найден минимум в Р1 = Р0
			}
			iter++;
		}
		while ( (P1 - P0).norm() > acc2 );
		pEpicentr->m_cart = P1;
	}
	//else выбранная начальная точка попала в минимум
	return iter;
}
//------------------------------------------------------------------------------
// Определение точности оценок координат эпицентра.
// Погрешность - это радиус сферы неопределённости (pEpicentr->m_impr).
// Расчётные формулы см. в теории.
void CalcImPrecition(
	std::vector <PointAll> *pPoints,	// точки сейсмоприёмников
	PointAll *pEpicentr)	// точка эпицентра (радиус сферы)
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
// Расчёт оценок координат эпицентра и радиус сферы неопределённости.
// pEpic->m_impr - радиус сферы неопределённости
long FindEpicenter(
	std::vector <PointAll> *pPoints,		// точки сейсмоприёмников
	PointAll *pEpicentr,					// точка эпицентра
	float accuracy)	// точность определения декартовых координат эпицентра, км
{
	pEpicentr->m_dist = accuracy;

	// расчёт декартовых координат
	for(auto it=pPoints->begin(); it!=pPoints->end(); ++it)
		ConvertGeographyInCartesian(&(*it));
	
	// определение оценок координат эпицентра
	long ret = ConjGradFR(pPoints, pEpicentr);
	ConvertCartesianInGeography(pEpicentr);

	// определение радиус неопределённости, км
	CalcImPrecition(pPoints, pEpicentr);
	
	return ret;
}
//------------------------------------------------------------------------------
// Расчет кол-ва округляемых разрядов для значений времени
void Calc_RoundNumberForTime(
	const float freqADC,// частота дискретизации
	int & numTime)		// кол-во отображаемых цифр после запятой в значениях времени
{
	numTime = 1 - CalcOrderOfValue(1.f / freqADC);
}
//------------------------------------------------------------------------------
// Расчет кол-ва округляемых разрядов для значений ускорения, скорости и перемещения
// minAccel - минимальное значение ускорения
// freqMax - максимальная частота
// numA - кол-во отображаемых цифр после запятой в значениях ускорения
// numV - кол-во отображаемых цифр после запятой в значениях скорости
// numX - кол-во отображаемых цифр после запятой в значениях перемещения
void Calc_RoundNumberForAVX(const float minAccel, const float freqMax,
	int & numA, int & numV, int & numX)
{
	numA = 1 - CalcOrderOfValue(minAccel);

	float w = (float)(IPP_2PI * freqMax);
	float val = minAccel / w;
	numV = 1 - CalcOrderOfValue(val);

	val /= w;
	numX = 1 - CalcOrderOfValue(val * 1000);		// перемещение в мм
}
//------------------------------------------------------------------------------
// Округление значений в структуре PeakAccel
// sPeakAccel - структура, в которой требуется округлить значения
// numA - кол-во отображаемых цифр после запятой в значениях ускорения
// numV - кол-во отображаемых цифр после запятой в значениях скорости
// numX - кол-во отображаемых цифр после запятой в значениях перемещения
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
// Округление значений в структуре sEventParam
// sEventParam - структура, в которой требуется округлить значения
// numA - кол-во отображаемых цифр после запятой в значениях ускорения
// numV - кол-во отображаемых цифр после запятой в значениях скорости
// numX - кол-во отображаемых цифр после запятой в значениях перемещения
// numT - кол-во отображаемых цифр после запятой в значениях времени
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
// Округление значений в структуре sEventStation
// sEventStation - структура, в которой требуется округлить значения
// numA - кол-во отображаемых цифр после запятой в значениях ускорения
// numV - кол-во отображаемых цифр после запятой в значениях скорости
// numX - кол-во отображаемых цифр после запятой в значениях перемещения
// numT - кол-во отображаемых цифр после запятой в значениях времени
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