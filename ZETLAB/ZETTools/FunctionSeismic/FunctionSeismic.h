//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
#include "afx.h"
#include <vector>
#include <Constants.h>
#include <mathZet.h>
#include <Seismo\DetectorSSEC\Struct_PeakAccel\Ssm_str_PeakAccel.zschema1.h>
#include <Seismo\DetectorSSEC\Struct_EventParam\Ssm_str_EventParam.zschema1.h>
#include <Seismo\StationSSEC\Struct_EventStation\Ssm_str_EventStation.zschema1.h>
//------------------------------------------------------------------------------
struct PointXYZ
{// радиус-вектор (точка) в декартовом 3-х мерном пространстве
public:
	float x,	// от центра через точку с широтой = 0 и долготой = 0
		  y,	// от центра через точку с широтой = 0 и долготой = 90
		  z;	// от центра через Северный полюс
	PointXYZ() { x = 0.f; y = 0.f; z = 0.f; }	// центр Земли
	PointXYZ(float a, float b, float c) { x = a; y = b; z = c; }
	float norm() { return x*x + y*y + z*z; }	// норма вектора
	float abs() { return sqrt((norm())); }		// длина вектора	
	PointXYZ one() { float a = abs(); return PointXYZ(x/a, y/a, z/a); }//единичный вектор

//	bool	 operator == (const PointXYZ &p){ return (x == p.x) && (y == p.y) && (z == p.z); } исправлена из-за исправления ошибки найденной 2014.06.02 pvs studio 
	bool	 operator == (const PointXYZ &p){ return (IsEqual(x,p.x, 0.0001f)) && (IsEqual(y, p.y, 0.0001f)) && (IsEqual(z, p.z, 0.0001f)); } 
//	bool	 operator != (const PointXYZ &p){ return (x != p.x) || (y != p.y) || (z != p.z); } исправлена из-за исправления ошибки найденной 2014.06.02 pvs studio 
	bool	 operator != (const PointXYZ &p){ return (IsNotEqual(x, p.x, 0.0001f)) || (IsNotEqual(y, p.y, 0.0001f)) || (IsNotEqual(z, p.z, 0.0001f)); }
	void     operator =  (const int &a)		{ x = (float)a; y = 0.f; z = 0.f;}
	void     operator =  (const float &a)	{ x = a; y = 0.f; z = 0.f; }
	PointXYZ operator -  ()					{ return PointXYZ(-x, -y, -z); }
	PointXYZ operator +  (const PointXYZ &p){ return PointXYZ(x + p.x, y + p.y, z + p.z); }
	void     operator += (const PointXYZ &p){ x += p.x;	y += p.y; z +=p.z; }
	PointXYZ operator -  (const PointXYZ &p){ return PointXYZ(x - p.x, y - p.y, z - p.z); }
	void     operator -= (const PointXYZ &p){ x -= p.x;	y -= p.y; z -= p.z; }
	PointXYZ operator *  (const int &a)		{ return PointXYZ(a * x, a * y, a * z); }
	PointXYZ operator *  (const float &a)	{ return PointXYZ(a * x, a * y, a* z); }
	void     operator *= (const int &a)		{ x *= (float)a; y *= (float)a; z *= (float)a; }
	void     operator *= (const float &a)	{ x *= a; y *= a; z *= a; }
	PointXYZ operator /  (const int &a)		{ return PointXYZ(x/(float)a, y/(float)a, z/(float)a); }
	PointXYZ operator /  (const float &a)	{ return PointXYZ(x/a, y/a, z/a); }
	void     operator /= (const int &a)		{ x /= (float)a; y /= (float)a; z /= (float)a; }
	void     operator /= (const float &a)	{ x /= a; y /= a; z /= a; }
	// скалярное произведение
	float operator *  (const PointXYZ &p)	{ return x * p.x + y * p.y + z * p.z; }
	// векторное произведение
	PointXYZ operator || (const PointXYZ &p){ return PointXYZ(y*p.z-z*p.y, z*p.x-x*p.z, x*p.y-y*p.x); }
};
inline PointXYZ operator * ( const int &a,   PointXYZ &p )	{ return p * a; }
inline PointXYZ operator * ( const float &a, PointXYZ &p )	{ return p * a; }
//------------------------------------------------------------------------------
struct PointLLH
{// географические координаты точки над/под поверхностью Земли
	float m_lati,		// широта в град., (северное полушарие > 0)
		  m_long,		// долгота в град., (восточная долгота > 0)
		  m_heig;		// вертикаль,  (выше уровня моря > 0)
	PointLLH() { m_lati = 0.f; m_long = 0.f; m_heig = 0.f; }
	PointLLH(float a, float b, float c) { m_lati = a; m_long = b; m_heig = c; }
};
//------------------------------------------------------------------------------
struct PointAll
{// Координаты точки (сейсмоприёмника)
	PointLLH m_geog;	// географические координаты точки
	PointXYZ m_cart;	// декартовы координаты
	float m_dist;		// измеренная дистанция до эпицентра
	float m_impr;		// погрешность дистанции до эпицентра
};
//------------------------------------------------------------------------------
// Расчет дистанции и пеленга по географическим координатам акселерометра
// (первоя точка) и гипоцентра (вторая точка)
void DistanceAndBearing(float lat1, float lat1_er, float long1, float long1_er,
	float depth1, float depth1_er, float lat2, float long2, float heigth2,
	float *distance, float *distance_er, float *bearing, float *bearing_er);
//------------------------------------------------------------------------------
// Функции конвертации координат из географических в декартовы и наоборот
void ConvertGeographyInCartesian(PointAll *pP);
void ConvertCartesianInGeography(PointAll *pP);
//------------------------------------------------------------------------------
// Функции расчёта значений функции невязки и её градиента
float CalcFunction(std::vector <PointAll> *pPoints, PointXYZ *pP);
void  CalcGrad(std::vector <PointAll> *pPoints, PointXYZ *pP, PointXYZ *pGrad);
//------------------------------------------------------------------------------
// Реализация поиска декартовых координат эпицентра путём поиска минимума
// функции невязки методом сопряженных градиентов с заданием шага итерации в
// соответствии с алгоритмом Флетчера-Ривса
//------------------------------------------------------------------------------
// Основная функция
long FindEpicenter(std::vector <PointAll> *pPoints,
	PointAll *pEpicentr, float accuracy);
// Реализация метода сопряжённых градиентов
// в соответствии с алгоритмом Флетчера-Ривса
long ConjGradFR_3D(std::vector <PointAll> *pPoints, PointAll *pEpicentr);
// Поиск минимума на заданном направлении
bool FindMin_3D(std::vector <PointAll> *pPoints, PointAll *pEpicentr,
	PointXYZ *pP0, float *pValP0, PointXYZ *pS, PointXYZ *pP1, float *pValP1);
// Определение точности оценок координат эпицентра
void CalcImPrecition(std::vector <PointAll> *pPoints, PointAll *pEpicentr);
//------------------------------------------------------------------------------
/* Расчет баллов по шкале MSK64 в соответствии с
"Методика измерений уровня сейсмического воздействия. ЗАО “ЭТМС”, 2013 г.
Свидетельство об аттестации № 002-01,00260-2011-2013 от 20,12,2013 г.*/
template <class T>
void GetPointMSK64(float accel, T &point)
{
	if (accel < 0.01)
		point = 0;
	else if (accel <= 0.02f)
		point = 1;
	else if (accel <= 0.04f)
		point = 2;
	else if (accel <= 0.075f)
		point = 3;
	else if (accel <= 0.15f)
		point = 4;
	else if (accel <= 0.3f)
		point = 5;
	// ГОСТ ---------------------
	else if (accel <= 0.6f)
		point = 6;
	else if (accel <= 1.2f)
		point = 7;
	else if (accel <= 2.4f)
		point = 8;
	// ГОСТ ---------------------
	else if (accel <= 4.8f)
		point = 9;
	else if (accel <= 9.6f)
		point = 10;
	else if (accel <= 19.2f)
		point = 11;
	else// if (accel <= 38.4f)
		point = 12;
}
//------------------------------------------------------------------------------
// Расчет кол-ва округляемых разрядов для значений времени
// freqADC - частота дискретизации;
// numTime - кол-во отображаемых цифр после запятой в значениях времени
void Calc_RoundNumberForTime(const float freqADC, int & numTime);
//------------------------------------------------------------------------------
// Расчет кол-ва округляемых разрядов для значений ускорения, скорости и перемещения
// minAccel - минимальное значение ускорения
// freqMax - максимальная частота
// numA - кол-во отображаемых цифр после запятой в значениях ускорения
// numV - кол-во отображаемых цифр после запятой в значениях скорости
// numX - кол-во отображаемых цифр после запятой в значениях перемещения
void Calc_RoundNumberForAVX(const float minAccel, const float freqMax,
	int & numA, int & numV, int & numX);
//------------------------------------------------------------------------------
// Округление значений в структуре PeakAccel
// sPeakAccel - структура, в которой требуется округлить значения
// numA - кол-во отображаемых цифр после запятой в значениях ускорения
// numV - кол-во отображаемых цифр после запятой в значениях скорости
// numX - кол-во отображаемых цифр после запятой в значениях перемещения
void Round_PeakAccel(Ssm_str_PeakAccel & sPeakAccel, const int numA, const int numV, const int numX);
//------------------------------------------------------------------------------
// Округление значений в структуре sEventParam
// sEventParam - структура, в которой требуется округлить значения
// numA - кол-во отображаемых цифр после запятой в значениях ускорения
// numV - кол-во отображаемых цифр после запятой в значениях скорости
// numX - кол-во отображаемых цифр после запятой в значениях перемещения
// numT - кол-во отображаемых цифр после запятой в значениях времени
void Round_EventParam(Ssm_str_EventParam & sEventParam, const int numA, const int numV, const int numX, const int numT);
//------------------------------------------------------------------------------
// Округление значений в структуре sEventStation
// sEventStation - структура, в которой требуется округлить значения
// numA - кол-во отображаемых цифр после запятой в значениях ускорения
// numV - кол-во отображаемых цифр после запятой в значениях скорости
// numX - кол-во отображаемых цифр после запятой в значениях перемещения
// numT - кол-во отображаемых цифр после запятой в значениях времени
void Round_EventStation(Ssm_str_EventStation & sEventStation, const int numA, const int numV, const int numX, const int numT);
//------------------------------------------------------------------------------