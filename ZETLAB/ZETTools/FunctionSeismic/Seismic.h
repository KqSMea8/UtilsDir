//------------------------------------------------------------------------------
//	������ �� 10.07.2012
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
#include "afx.h"
#include <Constants.h>
//------------------------------------------------------------------------------
struct Point2D
{// ������-������ (�����) �� ���������� ���������, ���������� �����������
// ����� � ����� (0, 0)
public:
	float x,	// ������������� ����������� - �� ������ �� ������
		  y;	// ������������� ����������� - �� ����� �� ���������
	Point2D() { x = 0.f; y = 0.f; }
	Point2D(float a, float b) { x = a; y = b; }
	float norm() { return x*x + y*y; }			// ����� �������
	float abs() { return sqrt((norm())); }		// ����� �������	
	Point2D one() { float a = abs(); return Point2D(x/a, y/a); }//��������� ������
	Point2D conj() { return Point2D(x, -y); }
	Point2D reverse() { return Point2D(y, x); }

//	bool	operator == (const Point2D &p)	{ return (x == p.x) && (y == p.y); } ���������� ��-�� ����������� ������ ��������� 2014.06.02 pvs studio 
	bool	operator == (const Point2D &p)	{ return (IsEqual(x, p.x, 0.0001f)) && (IsEqual(y, p.y, 0.0001f)); }
//	bool	operator != (const Point2D &p)	{ return (x != p.x) || (y != p.y); }
	bool	operator != (const Point2D &p)	{ return (IsNotEqual(x, p.x, 0.0001f)) || (IsNotEqual(y, p.y, 0.0001f)); }

	void    operator =  (const int &a)		{ x = (float)a; y = 0.f;}
	void    operator =  (const float &a)	{ x = a; y = 0.f; }
	Point2D operator -  ()					{ return Point2D(-x, -y); }
	Point2D operator +  (const Point2D &p)	{ return Point2D(x + p.x, y + p.y); }
	void    operator += (const Point2D &p)	{ x += p.x;	y += p.y; }
	Point2D operator -  (const Point2D &p)	{ return Point2D(x - p.x, y - p.y); }
	void    operator -= (const Point2D &p)	{ x -= p.x;	y -= p.y; }
	Point2D operator *  (const int &a)		{ return Point2D(a * x, a * y); }
	Point2D operator *  (const float &a)	{ return Point2D(a * x, a * y); }
	void    operator *= (const int &a)		{ x *= (float)a; y *= (float)a; }
	void    operator *= (const float &a)	{ x *= a; y *= a; }
	Point2D operator /  (const int &a)		{ return Point2D(x/(float)a, y/(float)a); }
	Point2D operator /  (const float &a)	{ return Point2D(x/a, y/a); }
	void    operator /= (const int &a)		{ x /= (float)a; y /= (float)a; }
	void    operator /= (const float &a)	{ x /= a; y /= a; }
	// ��������� ������������
	float	operator *  (const Point2D &p)	{ return x * p.x + y * p.y; }
};
inline Point2D operator * ( const int &a,   Point2D &p )	{ return p * a; }
inline Point2D operator * ( const float &a, Point2D &p )	{ return p * a; }
//------------------------------------------------------------------------------
struct Point3D
{// ������-������ (�����) � ���������� 3-� ������ ������������
public:
	float x,	// �� ������ ����� ����� � ������� = 0 � �������� = 0
		  y,	// �� ������ ����� ����� � ������� = 0 � �������� = 90
		  z;	// �� ������ ����� �������� �����
	Point3D() { x = 0.f; y = 0.f; z = 0.f; }	// ����� �����
	Point3D(float a, float b, float c) { x = a; y = b; z = c; }
	float norm() { return x*x + y*y + z*z; }	// ����� �������
	float abs() { return sqrt((norm())); }		// ����� �������	
	Point3D one() { float a = abs(); return Point3D(x/a, y/a, z/a); }//��������� ������

//	bool	operator == (const Point3D &p)	{ return (x == p.x) && (y == p.y) && (z == p.z); } ���������� ��-�� ����������� ������ ��������� 2014.06.02 pvs studio 
	bool	operator == (const Point3D &p)	{ return (IsEqual(x, p.x, 0.0001f)) && (IsEqual(y, p.y, 0.0001f)) && (IsEqual(z, p.z, 0.0001f)); }
//	bool	operator != (const Point3D &p)	{ return (x != p.x) || (y != p.y) || (z != p.z); } ���������� ��-�� ����������� ������ ��������� 2014.06.02 pvs studio 
	bool	operator != (const Point3D &p)	{ return (IsNotEqual(x, p.x, 0.0001f)) || (IsNotEqual(y, p.y, 0.0001f)) || (IsNotEqual(z, p.z, 0.0001f)); }
	void    operator =  (const int &a)		{ x = (float)a; y = 0.f; z = 0.f;}
	void    operator =  (const float &a)	{ x = a; y = 0.f; z = 0.f; }
	Point3D operator -  ()					{ return Point3D(-x, -y, -z); }
	Point3D operator +  (const Point3D &p)	{ return Point3D(x + p.x, y + p.y, z + p.z); }
	void    operator += (const Point3D &p)	{ x += p.x;	y += p.y; z +=p.z; }
	Point3D operator -  (const Point3D &p)	{ return Point3D(x - p.x, y - p.y, z - p.z); }
	void    operator -= (const Point3D &p)	{ x -= p.x;	y -= p.y; z -= p.z; }
	Point3D operator *  (const int &a)		{ return Point3D(a * x, a * y, a * z); }
	Point3D operator *  (const float &a)	{ return Point3D(a * x, a * y, a * z); }
	void    operator *= (const int &a)		{ x *= (float)a; y *= (float)a; z *= (float)a; }
	void    operator *= (const float &a)	{ x *= a; y *= a; z *= a; }
	Point3D operator /  (const int &a)		{ return Point3D(x/(float)a, y/(float)a, z/(float)a); }
	Point3D operator /  (const float &a)	{ return Point3D(x/a, y/a, z/a); }
	void    operator /= (const int &a)		{ x /= (float)a; y /= (float)a; z /= (float)a; }
	void    operator /= (const float &a)	{ x /= a; y /= a; z /= a; }
	// ��������� ������������
	float operator *  (const Point3D &p)	{ return x * p.x + y * p.y + z * p.z; }
	// ��������� ������������
	Point3D operator || (const Point3D &p)	{ return Point3D(y*p.z-z*p.y, z*p.x-x*p.z, x*p.y-y*p.x); }
};
inline Point3D operator * ( const int &a,   Point3D &p )	{ return p * a; }
inline Point3D operator * ( const float &a, Point3D &p )	{ return p * a; }
//------------------------------------------------------------------------------
struct Point3Dd		// Point3Ddouble
{// ������-������ (�����) � ���������� 3-� ������ ������������
public:
	double x,	// �� ������ ����� ����� � ������� = 0 � �������� = 0
		  y,	// �� ������ ����� ����� � ������� = 0 � �������� = 90
		  z;	// �� ������ ����� �������� �����
	Point3Dd() { x = 0.f; y = 0.f; z = 0.f; }	// ����� �����
	Point3Dd(double a, double b, double c) { x = a; y = b; z = c; }
	double norm() { return x*x + y*y + z*z; }	// ����� �������
	double abs() { return sqrt((norm())); }		// ����� �������	
	Point3Dd one() { double a = abs(); return Point3Dd(x/a, y/a, z/a); }//��������� ������

//	bool	 operator == (const Point3Dd &p){ return (x == p.x) && (y == p.y) && (z == p.z); } ���������� ��-�� ����������� ������ ��������� 2014.06.02 pvs studio 
	bool	 operator == (const Point3Dd &p){ return (IsEqual(x, p.x, 0.0001)) && (IsEqual(y, p.y, 0.0001)) && (IsEqual(z, p.z, 0.0001)); }
//	bool	 operator != (const Point3Dd &p){ return (x != p.x) || (y != p.y) || (z != p.z); } ���������� ��-�� ����������� ������ ��������� 2014.06.02 pvs studio 
	bool	 operator != (const Point3Dd &p){ return (IsNotEqual(x, p.x, 0.0001)) || (IsNotEqual(y, p.y, 0.0001)) || (IsNotEqual(z, p.z, 0.0001)); }
	void     operator =  (const int &a)		{ x = (float)a; y = 0.f; z = 0.f;}
	void     operator =  (const float &a)	{ x = a; y = 0.f; z = 0.f; }
	void     operator =  (const double &a)	{ x = a; y = 0.f; z = 0.f; }
	Point3Dd operator -  ()					{ return Point3Dd(-x, -y, -z); }
	Point3Dd operator +  (const Point3Dd &p){ return Point3Dd(x + p.x, y + p.y, z + p.z); }
	void     operator += (const Point3Dd &p){ x += p.x;	y += p.y; z +=p.z; }
	Point3Dd operator -  (const Point3Dd &p){ return Point3Dd(x - p.x, y - p.y, z - p.z); }
	void     operator -= (const Point3Dd &p){ x -= p.x;	y -= p.y; z -= p.z; }
	Point3Dd operator *  (const int &a)		{ return Point3Dd(a * x, a * y, a * z); }
	Point3Dd operator *  (const float &a)	{ return Point3Dd(a * x, a * y, a * z); }
	Point3Dd operator *  (const double &a)	{ return Point3Dd(a * x, a * y, a * z); }
	void     operator *= (const int &a)		{ x *= (double)a; y *= (double)a; z *= (double)a; }
	void     operator *= (const float &a)	{ x *= a; y *= a; z *= a; }
	void     operator *= (const double &a)	{ x *= a; y *= a; z *= a; }
	Point3Dd operator /  (const int &a)		{ return Point3Dd(x/(double)a, y/(double)a, z/(double)a); }
	Point3Dd operator /  (const float &a)	{ return Point3Dd(x/a, y/a, z/a); }
	Point3Dd operator /  (const double &a)	{ return Point3Dd(x/a, y/a, z/a); }
	void     operator /= (const int &a)		{ x /= (double)a; y /= (double)a; z /= (double)a; }
	void     operator /= (const float &a)	{ x /= a; y /= a; z /= a; }
	void     operator /= (const double &a)	{ x /= a; y /= a; z /= a; }
	// ��������� ������������
	double operator *  (const Point3Dd &p)	{ return x * p.x + y * p.y + z * p.z; }
	// ��������� ������������
	Point3Dd operator || (const Point3Dd &p){ return Point3Dd(y*p.z-z*p.y, z*p.x-x*p.z, x*p.y-y*p.x); }
};
inline Point3Dd operator * ( const int &a,    Point3Dd &p )	{ return p * a; }
inline Point3Dd operator * ( const float &a,  Point3Dd &p )	{ return p * a; }
inline Point3Dd operator * ( const double &a, Point3Dd &p )	{ return p * a; }
//------------------------------------------------------------------------------