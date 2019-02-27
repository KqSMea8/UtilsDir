// Класс трёхмерного тэтарэдра для решения задачи нахождения гиппоцентра землетрясения
#pragma once

#include <FunctionSeismic\SimplexPoint.h>
#include <Seismo\StationSSEC\Struct_EventStation\Ssm_str_EventStation.zschema1.h>
#include <Metrology\Rectangle.h>
#include <Metrology\Space.h>
#include <vector>
#include <list>

using namespace std;

class CSimplex
{
private:
	list<Ssm_str_EventStation> m_resStationList;

	DATE m_Time0;				//	точка отсчёта относительного момента

	bool m_bDebug;
	bool m_bError;

	double m_alpha = 1.0;		//	коэффициент отражения
	double m_betta = 0.5;		//	коэффициент сжатия
	double m_gamma = 2.0;		//	коэффициент растяжения

	CSimplexPoint m_center;		//	центр масс
	CSimplexPoint m_reflect;	//	отражённая точка

	CRectangle3D<double> m_rect;			//	область поиска гиппоцентра

	vector<CSimplexPoint> m_simplex;		//	вектор врешин симплекса

	double CalcResidual(CPointEarth &point, DATE time);		//	вычисляет невязку точки

	void GetRectangle3DOneStation(Ssm_str_EventStation &resStation, CRectangle3D<double> &rect);
	void SetInitialPoint(Point4D<double> point);

	void CalcParameters(CSimplexPoint &point);		//	считает время и навезку для заданной точки

	DATE FindOptimalTime(CPointEarth &point);		//	ищет по формуле время начала события, обеспечивающее минимальную невязку

	bool RGreater(const CSimplexPoint &point1, const CSimplexPoint &point2);
	bool TestConvergence();	//	проверка на сходимость

	void SortByResidual();	//	сортирует точки по их отклонению
	void CenterOfMass();	//	определяет центр масс симплекса
	void Reflection();		//	отражает самую "тяжёлую" точку относительно центра масс
	void ReflectionGamma();	//	дополнительно растягивает успешное отражение
	void Pressing();		//	сжимает неудачно выбранное направление движения
	void Dilatation();		//	гомотетия сжатия к точке с наимельншим "весом"

	void PointInRect(CSimplexPoint &point);			//	проверка точки на нахождение в области поиска и её ограничение

	inline bool operator()(const CSimplexPoint &p1, const CSimplexPoint &p2) const { return p1.r > p2.r; };

	void DebugOutput(CString str);
	void DebugOutput(CString str, CSimplexPoint point);

public:

	CSimplex();
	~CSimplex();

	bool SimplexMethod(_Out_ CSimplexPoint &point);

	void SetStationList(_In_ list<Ssm_str_EventStation> &resStationList);
	void SetZeroTime(_In_ DATE &time);
	void SetDebugOutput(_In_ bool flag);
};

