// ����� ���������� ��������� ��� ������� ������ ���������� ����������� �������������
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

	DATE m_Time0;				//	����� ������� �������������� �������

	bool m_bDebug;
	bool m_bError;

	double m_alpha = 1.0;		//	����������� ���������
	double m_betta = 0.5;		//	����������� ������
	double m_gamma = 2.0;		//	����������� ����������

	CSimplexPoint m_center;		//	����� ����
	CSimplexPoint m_reflect;	//	��������� �����

	CRectangle3D<double> m_rect;			//	������� ������ �����������

	vector<CSimplexPoint> m_simplex;		//	������ ������ ���������

	double CalcResidual(CPointEarth &point, DATE time);		//	��������� ������� �����

	void GetRectangle3DOneStation(Ssm_str_EventStation &resStation, CRectangle3D<double> &rect);
	void SetInitialPoint(Point4D<double> point);

	void CalcParameters(CSimplexPoint &point);		//	������� ����� � ������� ��� �������� �����

	DATE FindOptimalTime(CPointEarth &point);		//	���� �� ������� ����� ������ �������, �������������� ����������� �������

	bool RGreater(const CSimplexPoint &point1, const CSimplexPoint &point2);
	bool TestConvergence();	//	�������� �� ����������

	void SortByResidual();	//	��������� ����� �� �� ����������
	void CenterOfMass();	//	���������� ����� ���� ���������
	void Reflection();		//	�������� ����� "������" ����� ������������ ������ ����
	void ReflectionGamma();	//	������������� ����������� �������� ���������
	void Pressing();		//	������� �������� ��������� ����������� ��������
	void Dilatation();		//	��������� ������ � ����� � ����������� "�����"

	void PointInRect(CSimplexPoint &point);			//	�������� ����� �� ���������� � ������� ������ � � �����������

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

