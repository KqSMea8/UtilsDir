//------------------------------------------------------------------------------
//	¬ерси€ от 10.07.2012
//	«еленоград, «јќ "Ё“ћ—", —авостин ё.ћ.
//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
#include "afx.h"
#include <Constants.h>
//------------------------------------------------------------------------------
struct Point2D
{// радиус-вектор (точка) на декартовой плоскости, касающейс€ поверхности
// «емли в точке (0, 0)
public:
	float x,	// положительное направление - на восток по широте
		  y;	// положительное направление - на север по меридиану
	Point2D() { x = 0.f; y = 0.f; }
	Point2D(float a, float b) { x = a; y = b; }
	float norm() { return x*x + y*y; }			// норма вектора
	float abs() { return sqrt((norm())); }		// длина вектора	
	Point2D one() { float a = abs(); return Point2D(x/a, y/a); }//единичный вектор
	Point2D conj() { return Point2D(x, -y); }
	Point2D reverse() { return Point2D(y, x); }

//	bool	operator == (const Point2D &p)	{ return (x == p.x) && (y == p.y); } исправлена из-за исправлени€ ошибки найденной 2014.06.02 pvs studio 
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
	// скал€рное произведение
	float	operator *  (const Point2D &p)	{ return x * p.x + y * p.y; }
};
inline Point2D operator * ( const int &a,   Point2D &p )	{ return p * a; }
inline Point2D operator * ( const float &a, Point2D &p )	{ return p * a; }
//------------------------------------------------------------------------------
struct Point3D
{// радиус-вектор (точка) в декартовом 3-х мерном пространстве
public:
	float x,	// от центра через точку с широтой = 0 и долготой = 0
		  y,	// от центра через точку с широтой = 0 и долготой = 90
		  z;	// от центра через —еверный полюс
	Point3D() { x = 0.f; y = 0.f; z = 0.f; }	// центр «емли
	Point3D(float a, float b, float c) { x = a; y = b; z = c; }
	float norm() { return x*x + y*y + z*z; }	// норма вектора
	float abs() { return sqrt((norm())); }		// длина вектора	
	Point3D one() { float a = abs(); return Point3D(x/a, y/a, z/a); }//единичный вектор

//	bool	operator == (const Point3D &p)	{ return (x == p.x) && (y == p.y) && (z == p.z); } исправлена из-за исправлени€ ошибки найденной 2014.06.02 pvs studio 
	bool	operator == (const Point3D &p)	{ return (IsEqual(x, p.x, 0.0001f)) && (IsEqual(y, p.y, 0.0001f)) && (IsEqual(z, p.z, 0.0001f)); }
//	bool	operator != (const Point3D &p)	{ return (x != p.x) || (y != p.y) || (z != p.z); } исправлена из-за исправлени€ ошибки найденной 2014.06.02 pvs studio 
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
	// скал€рное произведение
	float operator *  (const Point3D &p)	{ return x * p.x + y * p.y + z * p.z; }
	// векторное произведение
	Point3D operator || (const Point3D &p)	{ return Point3D(y*p.z-z*p.y, z*p.x-x*p.z, x*p.y-y*p.x); }
};
inline Point3D operator * ( const int &a,   Point3D &p )	{ return p * a; }
inline Point3D operator * ( const float &a, Point3D &p )	{ return p * a; }
//------------------------------------------------------------------------------
struct Point3Dd		// Point3Ddouble
{// радиус-вектор (точка) в декартовом 3-х мерном пространстве
public:
	double x,	// от центра через точку с широтой = 0 и долготой = 0
		  y,	// от центра через точку с широтой = 0 и долготой = 90
		  z;	// от центра через —еверный полюс
	Point3Dd() { x = 0.f; y = 0.f; z = 0.f; }	// центр «емли
	Point3Dd(double a, double b, double c) { x = a; y = b; z = c; }
	double norm() { return x*x + y*y + z*z; }	// норма вектора
	double abs() { return sqrt((norm())); }		// длина вектора	
	Point3Dd one() { double a = abs(); return Point3Dd(x/a, y/a, z/a); }//единичный вектор

//	bool	 operator == (const Point3Dd &p){ return (x == p.x) && (y == p.y) && (z == p.z); } исправлена из-за исправлени€ ошибки найденной 2014.06.02 pvs studio 
	bool	 operator == (const Point3Dd &p){ return (IsEqual(x, p.x, 0.0001)) && (IsEqual(y, p.y, 0.0001)) && (IsEqual(z, p.z, 0.0001)); }
//	bool	 operator != (const Point3Dd &p){ return (x != p.x) || (y != p.y) || (z != p.z); } исправлена из-за исправлени€ ошибки найденной 2014.06.02 pvs studio 
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
	// скал€рное произведение
	double operator *  (const Point3Dd &p)	{ return x * p.x + y * p.y + z * p.z; }
	// векторное произведение
	Point3Dd operator || (const Point3Dd &p){ return Point3Dd(y*p.z-z*p.y, z*p.x-x*p.z, x*p.y-y*p.x); }
};
inline Point3Dd operator * ( const int &a,    Point3Dd &p )	{ return p * a; }
inline Point3Dd operator * ( const float &a,  Point3Dd &p )	{ return p * a; }
inline Point3Dd operator * ( const double &a, Point3Dd &p )	{ return p * a; }
//------------------------------------------------------------------------------