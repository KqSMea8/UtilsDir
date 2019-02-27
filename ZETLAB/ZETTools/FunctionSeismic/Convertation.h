#pragma once

struct gPoint
{
	double dLatitude;
	double dLongitude;
	double dAltitude;

	gPoint::gPoint()
	{
		dLatitude = 0.;
		dLongitude = 0.;
		dAltitude = 0.;
	}
	gPoint::gPoint(double _dLatitude, double _dLongitude, double _dAltitude)
	{
		dLatitude = _dLatitude;
		dLongitude = _dLongitude;
		dAltitude = _dAltitude;
	}
};

enum GeodeticDatum : int
{
	gdPZ90_WGS84 = 0,
	gdWGS84_PZ90,
	gdPZ90_02_WGS84,
	gdWGS84_PZ90_02,
	gdPZ90_02_PZ90,
	gdPZ90_PZ90_02,
	gdSK42_PZ90_02,
	gdPZ90_02_SK42,
	gdSK95_PZ90_02,
	gdPZ90_02_SK95,
	gdSK42_PZ90,
	gdPZ90_SK42,
	gdSK95_PZ90,
	gdPZ90_SK95,
	gdWGS84_SK42,
	gdWGS84_SK95,
};

enum GeodeticSystem : int
{
	gsWGS84 = 0,
	gsSK42,
	gsSK95,
	gsPZ90,
	gsPZ90_02,
};

bool ConvertGeodeticCoordinates(GeodeticDatum gdFromTo, gPoint gpFrom, gPoint* gpTo);
bool GetDeltaBetweenTwoPoints(GeodeticSystem gsSystem, gPoint gpPoint1, gPoint gpPoint2, double* dDeltaX, double* dDeltaY, double* dDeltaZ);
