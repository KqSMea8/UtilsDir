//------------------------------------------------------------------------------
#pragma once
#include <Metrology\Space.h>
//------------------------------------------------------------------------------
template <class T>
class CRectangle
{// двумерный прямоугольник
public:
	CSpace<T> m_x;	// интервал по X
	CSpace<T> m_y;	// интервал по Y

	CRectangle()
		: m_x()
		, m_y()
	{}
	CRectangle(const CRectangle<T> &rec)
	{
		m_x = rec.m_x;
		m_y = rec.m_y;
	}
	virtual ~CRectangle() {}

	void Clear()
	{
		m_x.Clear();
		m_y.Clear();
	}

	bool IsOk() {
		return m_x.IsSpaceOk() && m_y.IsSpaceOk(); }

	bool IsNotEmpty() {
		return m_x.IsSpaceNotEmpty() && m_y.IsSpaceNotEmpty(); }

	bool IsCross(CRectangle &rec) {
		return m_x.IsSpaceCross(rec.m_x) && m_y.IsSpaceCross(rec.m_y); }

	bool IsPointInside(const T valX, const T valY) {
		return m_x.IsSpaceCross(valX) && m_y.IsSpaceCross(valY); } 

	bool SetOnlyCross(CRectangle &rec){
		return m_x.SetSpaceOnlyCross(rec.m_x) && m_y.SetSpaceOnlyCross(rec.m_y); }
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
template <class T>
class CRectangle3D
{// двумерный прямоугольник
public:
	CSpace<T> m_x;	// интервал по X
	CSpace<T> m_y;	// интервал по Y
	CSpace<T> m_z;	// интервал по Z

	CRectangle3D()
		: m_x()
		, m_y()
		, m_z()
	{}
	CRectangle3D(const CRectangle3D<T> &rec)
	{
		m_x = rec.m_x;
		m_y = rec.m_y;
		m_z = rec.m_z;
	}
	virtual ~CRectangle3D() {}

	void Clear()
	{
		m_x.Clear();
		m_y.Clear();
		m_z.Clear();
	}

	bool IsOk() {
		return m_x.IsSpaceOk() && m_y.IsSpaceOk() && m_z.IsSpaceOk();
	}

	bool IsNotEmpty() {
		return m_x.IsSpaceNotEmpty() && m_y.IsSpaceNotEmpty() && m_z.IsSpaceNotEmpty();
	}

	bool IsCross(CRectangle3D &rec) {
		return m_x.IsSpaceCross(rec.m_x) && m_y.IsSpaceCross(rec.m_y) && m_z.IsSpaceCross(rec.m_z);
	}

	bool IsPointInside(const T valX, const T valY, const T valZ) {
		return m_x.IsSpaceCross(valX) && m_y.IsSpaceCross(valY) && m_z.IsSpaceCross(valZ);
	}

	bool SetOnlyCross(CRectangle3D &rec){
		return m_x.SetSpaceOnlyCross(rec.m_x) && m_y.SetSpaceOnlyCross(rec.m_y) && m_z.SetSpaceOnlyCross(rec.m_z);
	}
};