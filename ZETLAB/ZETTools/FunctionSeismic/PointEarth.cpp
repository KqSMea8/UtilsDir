//------------------------------------------------------------------------------
// ��� ������� � ��������� ������� ������������ �� ���� � 51794-2008 "����������
// ������������� ����������� �������. ������� ���������. ������ ��������������
// ��������� ������������ �����"
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <FunctionSeismic\PointEarth.h>
//------------------------------------------------------------------------------
void CPointEarth::Init()
{
	m_geodeticSystem = gsWGS84;
	m_CurGeodeticSystem = g_parWGS84;
	m_bPointEnable = false;
}
//------------------------------------------------------------------------------
CPointEarth::CPointEarth()
{
	Init();
}
//------------------------------------------------------------------------------
CPointEarth::CPointEarth(const CPointEarth& p)
{
	*this = p;
}
//------------------------------------------------------------------------------
CPointEarth::CPointEarth(double latDeg, double lonDeg, double alt)
{// ���� � ��������
	Init();
	Point3D<double> pd(latDeg, lonDeg, alt);
	SetGeodeticCoordinates(pd);
}
//------------------------------------------------------------------------------
void CPointEarth::operator = (const CPointEarth &p)
{
	m_bPointEnable = p.m_bPointEnable;
	m_geodeticSystem = p.m_geodeticSystem;
	m_CurGeodeticSystem = p.m_CurGeodeticSystem;
	m_cart = p.m_cart;
	m_geodDeg = p.m_geodDeg;
	m_geodRad = p.m_geodRad;
}
//------------------------------------------------------------------------------
bool CPointEarth::GetGeodeticCoordinates(
	Point3D<double> &geod,		// ������������� ����������
	bool degree)				// true - � ��������; false - � ��������
{// ��������� ������������� ��������� ����� ������
	bool ret = m_bPointEnable;
	if (ret)
	{
		if (degree)
			geod = m_geodDeg;	// ���� � ��������
		else
			geod = m_geodRad;	// ���� � ��������
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CPointEarth::SetGeodeticCoordinates(Point3D<double> &geodDeg)
{// ������� ����� ������ ����� ������������� ���������� � ��������
	bool ret = (-90. <= geodDeg.x) && (geodDeg.x <= 90.) &&		// ������
				(-180. <= geodDeg.y) && (geodDeg.y <= 180.) &&	// �������
				(-m_CurGeodeticSystem.maxAxis <= geodDeg.z);	// ������
	if (ret)
	{
		m_geodDeg = geodDeg;
		m_geodRad = Point3D<double> (
			GetRadian(m_geodDeg.x), GetRadian(m_geodDeg.y), m_geodDeg.z);
		ret = m_bPointEnable = ConvertGeodeticToCartesian(m_geodRad, m_cart);
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CPointEarth::GetCartesianCoordinates(
	Point3D<double> &cart)	// ��������� ����������
{// ��������� ���������� ��������� ����� ������
	bool ret = m_bPointEnable;
	if (ret)
		cart = m_cart;
	return ret;
}
//------------------------------------------------------------------------------
bool CPointEarth::SetCartesianCoordinates(Point3D<double> &cart)
{// ������� ����� ������ ����� ��������� ����������
	m_cart = cart;
	bool ret = ConvertCartesianToGeodetic(m_cart, m_geodRad);
	if (ret)
	{
		m_geodDeg = Point3D<double> (
			GetDegree(m_geodRad.x), GetDegree(m_geodRad.y), m_geodRad.z);
	}
	m_bPointEnable = ret;
	return ret;
}
//------------------------------------------------------------------------------
bool CPointEarth::Distance2ToPoint(
	CPointEarth &point,			// ����� �� ������� ��������� ���������
	double &norm)				// �������� �������� ���������
{// ��������� (� ������) �� ����� ������ ��
	//		�����, �������� �������������� ������������
	bool ret = m_bPointEnable && point.m_bPointEnable;
	if (ret)
	{
		Point3D<double> cart;
		ret = point.GetCartesianCoordinates(cart);
		if (ret)
		{
			double x = m_cart.x - cart.x;
			norm = x * x;
			x = m_cart.y - cart.y;
			norm += x * x;
			x = m_cart.z - cart.z;
			norm += x * x;
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CPointEarth::DistanceToPoint(
	CPointEarth &point,			// ����� �� ������� ��������� ���������
	double &distance)			// �������� ���������
{
	double norm;
	bool ret = Distance2ToPoint(point, norm);
	if (ret)
		distance = sqrt(norm);
	return ret;
}
//------------------------------------------------------------------------------
bool CPointEarth::ConvertGeodeticToCartesian(
	Point3D<double> &geodInpRad,
	Point3D<double> &cartOut)
{// �������������� ��������� ����� �� ������������� � ���������
	// B - ������������� ������ � ���.
	// L - ������������� ������� � ���.
	// N - ������ ��������
	bool ret;
	double B = geodInpRad.x;
	double sinB = sin(B);
	double N = 1. - m_CurGeodeticSystem.eccent2 * sinB * sinB;
	ret = N > 0.;
	if (ret)
	{
		//double cosB = cos(B);
		//cartOut.x = (N + geodInpRad.z) * cosB * cos(L);
		//cartOut.y = (N + geodInpRad.z) * cosB * sin(L);
		N = m_CurGeodeticSystem.maxAxis / sqrt(N);
		double L = geodInpRad.y;
		double cosB = cos(B) * (N + geodInpRad.z);
		cartOut.x = cosB * cos(L);
		cartOut.y = cosB * sin(L);
		cartOut.z = ( (1. - m_CurGeodeticSystem.eccent2) * N + geodInpRad.z)
			* sinB;
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CPointEarth::ConvertCartesianToGeodetic(
	Point3D<double> &cartInp,
	Point3D<double> &geodOutRad)
{// �������������� ��������� ����� �� ���������� � �������������
	bool ret;
	double B, L(0.), H;
	double sinB;
	double x, D = sqrt(cartInp.x * cartInp.x + cartInp.y * cartInp.y);
	if (D == 0.)
	{// ����� �� �����, ���������� ����� ������
		B = 0.5 * M_PI * abs(cartInp.z);
		sinB = sin(B);
		x = 1. - m_CurGeodeticSystem.eccent2 * sinB * sinB;
		ret = x >= 0.;
		if (ret)
		{
			L = 0.;
			H = 2 * sinB - m_CurGeodeticSystem.maxAxis * sqrt(x);
		}
	}
	else
	{// ����� �� �� ����� �������
		if (cartInp.z == 0.)
		{// ����� �� ��������� ��������
			B = 0.;
			H = D - m_CurGeodeticSystem.maxAxis;
			x = abs(asin(cartInp.y / D));
			if		( (cartInp.y < 0.) && (cartInp.x > 0.) )
				L = 2 * M_PI - x;
			else if ( (cartInp.y < 0.) && (cartInp.x < 0.) )
				L = M_PI + x;
			else if ( (cartInp.y > 0.) && (cartInp.x < 0.) )
				L = M_PI - x;
			else if ( (cartInp.y > 0.) && (cartInp.x > 0.) )
				L = x;
			else if ( (cartInp.y == 0.) && (cartInp.x > 0.) )
				L = 0.;
			else if ( (cartInp.y == 0.) && (cartInp.x < 0.) )
				L = M_PI;
			ret = true;
		} 
		else
		{// ����� ������
			double r = sqrt(cartInp.x * cartInp.x + cartInp.y * cartInp.y + cartInp.z * cartInp.z);
			double c = asin(cartInp.z / r);
			double p = 0.5 * m_CurGeodeticSystem.eccent2 * m_CurGeodeticSystem.maxAxis / r;
			double b, d, s1, s2 = 0.;
			do 
			{
				s1 = s2;
				b = c + s1;
				x = sin(b);
				x = 1. - m_CurGeodeticSystem.eccent2 * x * x;
				ret = x > 0.;
				if (ret)
				{
					s2 = asin( ( p * sin(2 * b) ) / sqrt(x) );
					d = abs(s2 - s1);
				}
				else
					break;
			} while (d > 0.0001);
			if (ret)
			{
				B = b;
				sinB = sin(B);
				x = 1. - m_CurGeodeticSystem.eccent2 * sinB * sinB;
				ret = x > 0.;
				if (ret)
				{
					H = D * cos(B) + cartInp.z * sinB -
								m_CurGeodeticSystem.maxAxis * sqrt(x);
					L = asin(cartInp.y / D);
				}
			}
		}
	}
	// ������ ����������
	if (ret)
	{
		geodOutRad.x = B;
		geodOutRad.y = L;
		geodOutRad.z = H;
	}
	return ret;
}
//------------------------------------------------------------------------------
// ����������� �� ����� ������ �� ������ ����� � �����������
// East, North, Up (ENU - ������, �����, �����)
bool CPointEarth::DirectionENU(
	CPointEarth &point,		// ����� � ������������ ENU = {0, 0, 0}
	Point3D<double> &enu)	// ������� ����������
{
	bool ret;
	Point3D<double> geod0;	// ������������� ���������� ����� 000
	Point3D<double> cart0;	// ��������� ���������� ����� 000
	ret = point.GetGeodeticCoordinates(geod0, false) &&
		  point.GetCartesianCoordinates(cart0);
	if (ret)
	{	// ������� ���������
		enu = m_cart - cart0;
		// ������� ���� �� �������
		enu.rotateXY(geod0.y);
		// ������� ���� �� ������
		enu.rotateZX(-geod0.x);
		// ������� ��� X �� ������
		enu.rotateXY(M_PI_2);
		// ������� ��� Y �� �����, ��� Z ����� �������� �����
		enu.rotateYZ(M_PI_2);
		// ��������� ������ ��������� �����������
		enu = -enu.one();
	}
	return ret;
}
//------------------------------------------------------------------------------
void CPointEarth::Clear()
{
	m_bPointEnable = false;
	m_cart = 0.;
	m_geodDeg = 0.;
	m_geodRad = 0.;
}
//------------------------------------------------------------------------------