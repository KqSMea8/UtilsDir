//------------------------------------------------------------------------------
//	������ �� 05.12.2013
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
#include "afx.h"
#include <mathZet.h>
//------------------------------------------------------------------------------
template <class T>
struct Point2D		// T - ��� float ��� double
{// ������-������ (�����) �� ���������� ���������
	// � ������ ������� ���������
public:
	T x,
	  y;
	Point2D()			{ x = 0; y = 0; }
	Point2D(T a, T b)	{ x = a; y = b; }
	void Set(T a, T b)
	{
		x = a; y = b;
	}
	float	norm()		{ return x*x + y*y; }			// ����� �������
	float	abs()		{ return sqrt((norm())); }		// ����� �������	
	Point2D one()		{ T a = abs(); return Point2D(x/a, y/a); }//��������� ������
	Point2D conj()		{ return Point2D(x, -y); }		// ����������
	Point2D reverse()	{ return Point2D(y, x); }		// ������������
	void rotate(T angle)	// ������� �� ���� � ��������
	{// ���� ������������� �� ��� � ������ ������� �������
		T _x = x;
		T _y = y;
		T m_cos = cos(angle);
		T m_sin = sin(angle);
		x = _x * m_cos + _y * m_sin;
		y =-_x * m_sin + _y * m_cos;
	}
	bool	operator == (const Point2D &p)	{ return (x == p.x) && (y == p.y); }
	bool	operator != (const Point2D &p)	{ return (x != p.x) || (y != p.y); }
	void    operator =  (const int &a)		{ x = (T)a; y = 0; }
	void    operator =  (const float &a)	{ x = (T)a; y = 0; }
	void    operator =  (const double &a)	{ x = (T)a; y = 0; }
	Point2D operator -  ()					{ return Point2D(-x, -y); }
	Point2D operator +  (const Point2D &p)	{ return Point2D(x + p.x, y + p.y); }
	void    operator += (const Point2D &p)	{ x += p.x;	y += p.y; }
	Point2D operator -  (const Point2D &p)	{ return Point2D(x - p.x, y - p.y); }
	void    operator -= (const Point2D &p)	{ x -= p.x;	y -= p.y; }
	Point2D operator *  (const int &a)		{ return Point2D((T)a * x, (T)a * y); }
	Point2D operator *  (const float &a)	{ return Point2D((T)a * x, (T)a * y); }
	Point2D operator *  (const double &a)	{ return Point2D((T)a * x, (T)a * y); }
	void    operator *= (const int &a)		{ x *= (T)a; y *= (T)a; }
	void    operator *= (const float &a)	{ x *= (T)a; y *= (T)a; }
	void    operator *= (const double &a)	{ x *= (T)a; y *= (T)a; }
	Point2D operator /  (const int &a)		{ return Point2D(x/(T)a, y/(T)a); }
	Point2D operator /  (const float &a)	{ return Point2D(x/(T)a, y/(T)a); }
	Point2D operator /  (const double &a)	{ return Point2D(x/(T)a, y/(T)a); }
	void    operator /= (const int &a)		{ x /= (T)a; y /= (T)a; }
	void    operator /= (const float &a)	{ x /= (T)a; y /= (T)a; }
	void    operator /= (const double &a)	{ x /= (T)a; y /= (T)a; }
	// ��������� ������������
	T		operator *  (const Point2D &p)	{ return x * p.x + y * p.y; }
	// ��������� ������������
	T		operator || (const Point2D &p)	{ return x * p.y - y * p.x; }
};
template <class T>
Point2D<T> operator * ( const int &a,   Point2D<T> &p )	{ return p * (T)a; }
template <class T>
Point2D<T> operator * ( const float &a, Point2D<T> &p )	{ return p * (T)a; }
template <class T>
Point2D<T> operator * ( const double &a, Point2D<T> &p )	{ return p * (T)a; }
//------------------------------------------------------------------------------
template <class T>
struct Point3D		// T - ��� float ��� double
{// ������-������ (�����) � ���������� 3-� ������ ������������
// � ������ ������� ���������
public:
	T x,	// 
	  y,	// 
	  z;	// 
	Point3D()			{ x = 0; y = 0; z = 0; }
	Point3D(T a, T b, T c)	{ x = a; y = b; z = c; }
	T		norm()		{ return x*x + y*y + z*z; }	// ����� �������
	T		abs()		{ return sqrt((norm())); }		// ����� �������	
	Point3D	one()		{ T a = abs(); return Point3D(x/a, y/a, z/a); }//��������� ������
	void Set(T a, T b, T c)
	{
		x = a; y = b; z = c;
	}
	void rotateXY(T angle)	// ������� ���� ��������� � ��������� X0Y �� ����
	{// ���� � �������� ������������� �� ��� � ������ ������� �������
		Point2D<T> p2(x, y);
		p2.rotate(angle);
		x = p2.x;	y = p2.y;
	}
	void rotateYZ(T angle)	// ������� ���� ��������� � ��������� Y0Z �� ����
	{// ���� � �������� ������������� �� ��� Y ������ ������� �������
		Point2D<T> p2(y, z);
		p2.rotate(angle);
		y = p2.x;	z = p2.y;
	}
	void rotateZX(T angle)	// ������� ���� ��������� � ��������� Z0X �� ����
	{// ���� � �������� ������������� �� ��� Z ������ ������� �������
		Point2D<T> p2(z, x);
		p2.rotate(angle);
		z = p2.x;	x = p2.y;
	}
	bool	operator == (const Point3D &p)	{ return (x == p.x) && (y == p.y) && (z == p.z); }
	bool	operator != (const Point3D &p)	{ return (x != p.x) || (y != p.y) || (z != p.z); }
	void    operator =  (const int &a)		{ x = (T)a; y = 0; z = 0;}
	void    operator =  (const float &a)	{ x = (T)a; y = 0; z = 0; }
	void	operator =  (const double &a)	{ x = (T)a; y = 0; z = 0; }
	void	operator =	(const Point2D<T> &a)	{ x = a.x; y = a.y; z = 0; }
	Point3D operator -  ()					{ return Point3D(-x, -y, -z); }
	Point3D operator +  (const Point3D &p)	{ return Point3D(x + p.x, y + p.y, z + p.z); }
	void    operator += (const Point3D &p)	{ x += p.x;	y += p.y; z +=p.z; }
	Point3D operator -  (const Point3D &p)	{ return Point3D(x - p.x, y - p.y, z - p.z); }
	void    operator -= (const Point3D &p)	{ x -= p.x;	y -= p.y; z -= p.z; }
	Point3D operator *	(const int &a)		{ return Point3D((T)a * x,(T) a * y, (T)a * z); }
	Point3D operator *  (const float &a)	{ return Point3D((T)a * x, (T)a * y, (T)a * z); }
	Point3D operator *  (const double &a)	{ return Point3D((T)a * x, (T)a * y, (T)a * z); }
	void    operator *= (const int &a)		{ x *= (T)a; y *= (T)a; z *= (T)a; }
	void    operator *= (const float &a)	{ x *= (T)a; y *= (T)a; z *= (T)a; }
	void    operator *= (const double &a)	{ x *= (T)a; y *= (T)a; z *= (T)a; }
	Point3D operator /  (const int &a)		{ return Point3D(x/(T)a, y/(T)a, z/(T)a); }
	Point3D operator /  (const float &a)	{ return Point3D(x/(T)a, y/(T)a, z/(T)a); }
	Point3D operator /  (const double &a)	{ return Point3D(x/(T)a, y/(T)a, z/(T)a); }
	void    operator /= (const int &a)		{ x /= (T)a; y /= (T)a; z /= (T)a; }
	void    operator /= (const float &a)	{ x /= (T)a; y /= (T)a; z /= (T)a; }
	void    operator /= (const double &a)	{ x /= (T)a; y /= (T)a; z /= (T)a; }
	// ��������� ������������
	T		operator *  (const Point3D &p)	{ return x * p.x + y * p.y + z * p.z; }
	// ��������� ������������
	Point3D operator || (const Point3D &p)	{ return Point3D(y*p.z-z*p.y, z*p.x-x*p.z, x*p.y-y*p.x); }
};
template <class T>
Point3D<T> operator * ( const int &a,    Point3D<T> &p )	{ return p * (T)a; }
template <class T>
Point3D<T> operator * ( const float &a,  Point3D<T> &p )	{ return p * (T)a; }
template <class T>
Point3D<T> operator * ( const double &a, Point3D<T> &p )	{ return p * (T)a; }

//------------------------------------------------------------------------------
template <class T>
struct Point4D		// T - ��� float ��� double
{// ������-������ (�����) � ���������� 4-� ������ ������������
	// � ������ ������� ���������
public:
	T x,	// 
	  y,	// 
	  z,	// 
	  t;	//
	Point4D()			{ x = 0; y = 0; z = 0; t = 0; }
	Point4D(T a, T b, T c, T d)	{ x = a; y = b; z = c; t = d; }
	T		norm()		{ return x*x + y*y + z*z + t*t; }	// ����� �������
	T		abs()		{ return sqrt((norm())); }		// ����� �������	
	Point4D	one()		{ T a = abs(); return Point3D(x / a, y / a, z / a, t / a); }//��������� ������
	void Set(T a, T b, T c, T d)
	{
		x = a; y = b; z = c; t = d;
	}

	bool	operator == (const Point4D &p)	{ return (x == p.x) && (y == p.y) && (z == p.z) && (t == p.t); }
	bool	operator != (const Point4D &p)	{ return (x != p.x) || (y != p.y) || (z != p.z) || (t != p.t); }
	void    operator =  (const int &a)		{ x = (T)a; y = 0; z = 0; t = 0; }
	void    operator =  (const float &a)	{ x = (T)a; y = 0; z = 0; t = 0; }
	void	operator =  (const double &a)	{ x = (T)a; y = 0; z = 0; t = 0; }
	void	operator =	(const Point3D<T> &a)	{ x = a.x; y = a.y; z = a.z; t = 0; }
	Point4D operator -  ()					{ return Point4D(-x, -y, -z, -t); }
	Point4D operator +  (const Point4D &p)	{ return Point4D(x + p.x, y + p.y, z + p.z, t + p.t); }
	void    operator += (const Point4D &p)	{ x += p.x;	y += p.y; z += p.z; t += p.t; }
	Point4D operator -  (const Point4D &p)	{ return Point4D(x - p.x, y - p.y, z - p.z, t - p.t); }
	void    operator -= (const Point4D &p)	{ x -= p.x;	y -= p.y; z -= p.z; t -= p.t; }
	Point4D operator *	(const int &a)		{ return Point4D((T)a * x, (T)a * y, (T)a * z, (T)a * t); }
	Point4D operator *  (const float &a)	{ return Point4D((T)a * x, (T)a * y, (T)a * z, (T)a * t); }
	Point4D operator *  (const double &a)	{ return Point4D((T)a * x, (T)a * y, (T)a * z, (T)a * t); }
	void    operator *= (const int &a)		{ x *= (T)a; y *= (T)a; z *= (T)a; t *= (T)a; }
	void    operator *= (const float &a)	{ x *= (T)a; y *= (T)a; z *= (T)a; t *= (T)a; }
	void    operator *= (const double &a)	{ x *= (T)a; y *= (T)a; z *= (T)a; t *= (T)a; }
	Point4D operator /  (const int &a)		{ return Point4D(x / (T)a, y / (T)a, z / (T)a, t / (T)a); }
	Point4D operator /  (const float &a)	{ return Point4D(x / (T)a, y / (T)a, z / (T)a, t / (T)a); }
	Point4D operator /  (const double &a)	{ return Point4D(x / (T)a, y / (T)a, z / (T)a, t / (T)a); }
	void    operator /= (const int &a)		{ x /= (T)a; y /= (T)a; z /= (T)a; t /= (T)a; }
	void    operator /= (const float &a)	{ x /= (T)a; y /= (T)a; z /= (T)a; t /= (T)a; }
	void    operator /= (const double &a)	{ x /= (T)a; y /= (T)a; z /= (T)a; t /= (T)a; }
	// ��������� ������������
	T		operator *  (const Point4D &p)	{ return x * p.x + y * p.y + z * p.z + t * p.t; }
};
template <class T>
Point4D<T> operator * (const int &a, Point4D<T> &p)	{ return p * (T)a; }
template <class T>
Point4D<T> operator * (const float &a, Point4D<T> &p)	{ return p * (T)a; }
template <class T>
Point4D<T> operator * (const double &a, Point4D<T> &p)	{ return p * (T)a; }
