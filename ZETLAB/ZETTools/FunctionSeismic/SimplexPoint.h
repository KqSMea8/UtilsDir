#pragma once

#include <FunctionSeismic\PointEarth.h>

#define MAX_RESIDUAL	5.

class CSimplexPoint
{
public:

	Point4D<double> p;				//	координаты события: x,y,z - пространство и t - время
	double r;						//	невязка расчёта

	CSimplexPoint() { r = 0; };
	~CSimplexPoint() {};

	CPointEarth GetPointEarth() { return CPointEarth(p.x, p.y, p.z); };

	void operator =(const CSimplexPoint &sp) { p = sp.p; r = sp.r; };
};

