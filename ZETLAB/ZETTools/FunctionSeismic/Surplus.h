// ����� ���������� ��������
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

	DATE m_Time0;							//	����� ������� �������������� �������

	bool m_bDebug;

	CRectangle3D<double> m_rect;			//	������� ������ �����������

	CSpace<DATE> m_time;

	void GetRectangle3DOneStation(Ssm_str_EventStation &resStation, CRectangle3D<double> &rect);

	void CalcParameters(CSimplexPoint &point);				//	������� ����� � ������� ��� �������� �����
	double CalcResidual(CPointEarth &point, DATE time);		//	��������� ������� �����
	DATE FindOptimalTime(CPointEarth &point);				//	���� �� ������� ����� ������ �������, �������������� ����������� �������
	void TimeInterval();									//	������� ���������� ��������� ������ ������� ������ �������

	void DebugOutput(CString str, CSimplexPoint point);

public:
	CSurplus();
	~CSurplus();

	bool SurplusMethod(_Out_ CSimplexPoint &point);

	void SetStationList(_In_ list<Ssm_str_EventStation> &resStationList);
	void SetZeroTime(_In_ DATE &time);
	void SetDebugOutput(_In_ bool flag);
};

