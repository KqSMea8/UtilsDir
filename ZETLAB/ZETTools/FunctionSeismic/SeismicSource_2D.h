//------------------------------------------------------------------------------
//	Файл CSeismicSource_2D.h
//	Версия от 12.07.2012
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
// Класс предназначен для определения источника сейсмического воздействия по
// результатам измерений группой сейсмоприёмников. Строится функция максимального
// правдоподобия (ФМП). Методом сопряженных градиентов в соответствии с
// алгоритмом Флетчера-Ривса находится минимальное значение этой функции. Это и
// есть оценка точки источника.
// Под источником понимается эпицентр, который определяется по пересечению
// гипербол. Гиперболы рассчитываются по разнице времён вступления события.
//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
#include "afx.h"
#include <FunctionSeismic\Seismic.h>
//------------------------------------------------------------------------------
class CSeismicSource_2D
{
private:
	// исходные данные ----------------------------------------
	int Number;			// кол-во сейсмоприёмников в группе
	float Vp;			// скорость распространения P-фазы, км/сек
	float dVp;			// погрешность скорости P-фазы, км/сек
	float dT;			// погрешность времён вступлений (одна для всех), сек
	// массивы данных сейсмоприёмников
	Point2D *pGeog;		// географические координаты: x - latitude, y - longitude
	Point2D *pCart;		// декартовы координаты
	DATE *pTp;			// времена вступлений P-фазы 
	float *pDist2;		// рассчитанные разности дистанций до эпицентра
	float *pUncertDist;	// их погрешности
	// выходные данные ---------------------------------------
	Point2D SourceGeog;	// географические координаты источника
	Point2D SourceCart;	// декартовы координаты источника
	DATE SourceTime;	// время события
	float UncertR;		// радиус неопределённости источника, км
	float UncertT;		// погрешность времени события, сек

	// вспомогательные переменные ----------------------------
	int NumberHyperbole;// количествр гипербол
	float AccCoord;		// точность определения координат источника, км
	// расчёт функции и градиента на все случаи жизни
	float _FunctionAndGrad(Point2D *pP, Point2D *pGrad, bool needFunction);
	Point2D GeogCenter2D;	// географические координаты центральной точки 2D,
	// т.е. точки касания декартовой плоскости поверхности Земли

	// приватные функции класса ------------------------------
	// функции конвертации координат (географических и декартовых)
//	void ConvertGeographyInCartesian();
//	void ConvertCartesianInGeography();

	// функции расчёта значений и градиента функции максимального правдоподобия
	float CalcFunction(Point2D *pP);
	void  CalcGrad(Point2D *pP, Point2D *pGrad);
	float CalcFunctionAndGrad(Point2D *pP, Point2D *pGrad);

	void  CalcSourceTime();

	// расчёт радиуса неопределённости
	void CalcUncertaintyRadius();

	// вспомогательные функции
	Point2D SetFirstPoint();
	bool CalcDistance();
//	bool SetCenterFor2D();
	long ConjGrad_FletcherReeves();
	bool FindMin(Point2D *pP0, float *pValP0, Point2D *pS,
				 Point2D *pP1, float *pValP1, float accuracy);
protected:
public:
	CSeismicSource_2D();
	virtual ~CSeismicSource_2D();
	bool Create(int number, float dt, float vp, float dvp);

	// функции конвертации координат (географических и декартовых)
	void ConvertGeographyInCartesian();
	void ConvertCartesianInGeography();
	bool SetCenterFor2D();

	// функции свойств (географические координаты, времена вступлений)
	long GetNumber() { return Number; }
	bool SetNumber(int num);
	float GetVp() { return Vp; }
	bool SetVp(float val);
	float GetUncertaintyVp() { return dVp; }
	bool SetUncertaintyVp(float val);
	float GetLatitude(int num);
	float GetLongitude(int num);
	bool SetLatitude(int num, float val);
	bool SetLongitude(int num, float val);
	DATE GetTp(int num);
	bool SetTp(int num, DATE val);
	Point2D* GetpCartPoint() { return pCart; }		// декартовы координаты

	// функции класса (расчёты и получение результатов)
	long  FindSource();
	float GetSourceLatitude() { return SourceGeog.y; }
	float GetSourceLongitude() { return SourceGeog.x; }
	DATE  GetSourceTime() { return SourceTime; }
	float GetUncertaintyRadius() { return UncertR; }
	float GetUncertaintyTime() { return UncertT; }
};
//------------------------------------------------------------------------------