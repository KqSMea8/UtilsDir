//------------------------------------------------------------------------------
#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <FunctionSeismic\Point_2D_3D.h>
//------------------------------------------------------------------------------
enum GeodeticSystem
{// ��� ������� ������������� ��������� 
	gsWGS84,		// World Geodetic System 1984 (WGS 84)
	gsSK42,			// ������� ��������� 1942 ���� (��-42)
	gsSK95,			// ������� ��������� 1995 ���� (��-95)
	gsPZ90,			// ��������� ����� 1990 ���� (��-90)
	gsPZ90_02,		// ��������� ������ ��-90 2002 ���� (��-90.02)
	gsPZ90_11,		// ��������� ������ ��-90 2011 ���� (��-90.11)
	gsGSK2011		// ������������� ������� ��������� 2011 ���� (���-2011)
};
//------------------------------------------------------------------------------
struct ParamGeodeticSystem
{
	double maxAxis;	// ������� ������� ����������, �
	double eccent2;	// ������� ��������������� ����������
	ParamGeodeticSystem()
	{
		maxAxis = eccent2 = 0.;
	}
	ParamGeodeticSystem(
		double _maxAxis,	// ������� ������� ����������
		double compres)		// ������ ����������
	{
		maxAxis = _maxAxis;
		eccent2 = compres * (2. - compres);
	}
};
//------------------------------------------------------------------------------
const ParamGeodeticSystem g_parWGS84   = ParamGeodeticSystem(6378137.,  1./298.257223563);
const ParamGeodeticSystem g_parSK42    = ParamGeodeticSystem(6378245.,  1./298.3);
const ParamGeodeticSystem g_parSK95    = ParamGeodeticSystem(6378137.,  1./298.25784);
const ParamGeodeticSystem g_parPZ90    = ParamGeodeticSystem(6378136.,  1./298.25784);
const ParamGeodeticSystem g_parPZ90_02 = ParamGeodeticSystem(6378136.,  1./298.25784);
const ParamGeodeticSystem g_parPZ90_11 = ParamGeodeticSystem(6378136.,  1./298.25784);
const ParamGeodeticSystem g_parGSK2011 = ParamGeodeticSystem(6378136.5, 1./298.2564151);
//------------------------------------------------------------------------------
#define M_OneDegreeOfMeridianInkm 111.135
//------------------------------------------------------------------------------
/* ����� "����� �����". ������ ��� ������ ���������.
1. ���������� ���������� m_cart (��. ���� � 51794-2008):
	- ������ ������� ���������� � ������ ���� �����;
	- ��� Z ���������� � ������������� �������� ������ (������� ��������� ������),
		�.�. ��� ��� ��������� �����;
	- ��� X ����� � ��������� ���������� ���������;
	- ��� Y ��������� ������� ��������� �� ������.
	����� �������:
	- ��� Z ���������� �� �������� �����;
	- ��� X ���������� � ����� �� �������� � �������� 0 ����.;
	- ��� Y ���������� � ����� �� �������� � �������� 180 ����.;
2 � 3. ������������� ���������� m_geodDeg � m_geodRad � �������� � ��������:
	- ������������� ������ - ���� ����� �������� � ����������� ����������,
		���������� ����� �������� �����, � ���������� �������� ����������;
	- ������������� ������� - ����������� ���� ����� ����������� ��������������
		��������� ������ ����� � ���������� ���������.
	- ������������� ������ - ������ ����� ��� ������������ ����������.
	 ������������� ���������� � Point3D:
		x - ������;
		y - �������;
		z - ������ ��� ������� ����
*/
class CPointEarth
{
private:
	bool m_bPointEnable;		// ����� ������ ������
	Point3D<double> m_cart;		// ������� ��������� ���������� ����� ������
	Point3D<double> m_geodDeg;	// ������������� ���������� ����� ������ (�������)
	Point3D<double> m_geodRad;	// ������������� ���������� ����� ������ (�������)
	GeodeticSystem m_geodeticSystem;// ������������ ������� ������������� ���������
	ParamGeodeticSystem m_CurGeodeticSystem;
	void Init();

	// �������������� ���������
	bool ConvertGeodeticToCartesian(Point3D<double> &geodInpRad,
									Point3D<double> &cartOut);
	bool ConvertCartesianToGeodetic(Point3D<double> &cartInp,
									Point3D<double> &geodOutRad);
protected:

public:
	CPointEarth();
	CPointEarth(const CPointEarth &);
	CPointEarth(double latDeg, double lonDeg, double alt);	// ���� � ��������

	void operator = (const CPointEarth &p);
//	virtual ~CPointEarth();

	// ������� ������� --------------------------------------------------------
	bool GetPointEnable() { return m_bPointEnable; }

	GeodeticSystem GetGeodeticSystem() { return m_geodeticSystem; }
	bool SetGeodeticSystem(GeodeticSystem system) { return system == gsWGS84; }
	
	// ��������� ���������� ����� ������
	bool GetCartesianCoordinates(Point3D<double> &cart);
	bool GetCartesianCoordinates(Point3D<double> &cart) const;
	bool SetCartesianCoordinates(Point3D<double> &cart);
	
	// ������������� ���������� ����� ������ (���� � �������� ��� ��������)
	double GetLatitudeInDegree() { return m_geodDeg.x; }
	double GetLongitudeInGegree() { return m_geodDeg.y; }
	double GetAltitudeInMeters() { return m_geodDeg.z; }

	bool GetGeodeticCoordinates(Point3D<double> &geod, bool degree = true);
	bool SetGeodeticCoordinates(Point3D<double> &geodDeg);	// ������ � ��������

	// ������� ������ ---------------------------------------------------------
	// ��������� (� ������) �� ����� ������ �� ������ �����
	bool Distance2ToPoint(CPointEarth &point, double &norm);
	bool DistanceToPoint(CPointEarth &point, double &distance);

	// ����������� �� ����� ������ �� ������ ����� � �����������
	// ENU (������, �����, �����)
	bool DirectionENU(CPointEarth &point, Point3D<double> &enu);

	void Clear();
};
//------------------------------------------------------------------------------