#pragma once

#include <FunctionSeismic\PointEarth.h>

#define MAX_RESIDUAL	5.

class CSimplexPoint
{
public:

	Point4D<double> p;				//	���������� �������: x,y,z - ������������ � t - �����
	double r;						//	������� �������

	CSimplexPoint() { r = 0; };
	~CSimplexPoint() {};

	CPointEarth GetPointEarth() { return CPointEarth(p.x, p.y, p.z); };

	void operator =(const CSimplexPoint &sp) { p = sp.p; r = sp.r; };
};

