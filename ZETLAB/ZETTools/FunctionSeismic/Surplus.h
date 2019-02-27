// Класс выполнения перебора
#pragma once

#include <FunctionSeismic\SimplexPoint.h>
//#include <Seismo\Station\Struct_Res\Ssm_res_Station.zschema1.h>
#include <Seismo\StationSSEC\Struct_EventStation\Ssm_str_EventStation.zschema1.h>
#include <Metrology\Rectangle.h>
#include <Metrology\Space.h>
#include <ZetTools\include\ZetTools.h>
#include <vector>
#include <list>

using namespace std;

class CSurplus
{
	list<Ssm_str_EventStation> m_resStationList;

	DATE m_Time0;							//	точка отсчёта относительного момента

	bool m_bDebug;

	CRectangle3D<double> m_rect;			//	область поиска гиппоцентра

	CSpace<DATE> m_time;

	void GetRectangle3DOneStation(Ssm_str_EventStation &resStation, CRectangle3D<double> &rect);

	void CalcParameters(CSimplexPoint &point);				//	считает время и навезку для заданной точки
	double CalcResidual(CPointEarth &point, DATE time);		//	вычисляет невязку точки
	DATE FindOptimalTime(CPointEarth &point);				//	ищет по формуле время начала события, обеспечивающее минимальную невязку
	void TimeInterval();									//	считает допустимый интвервал поиска времени начала события

	void DebugOutput(CString str, CSimplexPoint point);

public:
	CSurplus();
	~CSurplus();

	bool SurplusMethod(_Out_ CSimplexPoint &point);

	void SetStationList(_In_ list<Ssm_str_EventStation> &resStationList);
	void SetZeroTime(_In_ DATE &time);
	void SetDebugOutput(_In_ bool flag);
};

