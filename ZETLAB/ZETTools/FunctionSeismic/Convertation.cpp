#include "Convertation.h"
#define _USE_MATH_DEFINES
#include <math.h>

// ��������� ��� �������� ���������:
//		delta_X - �������� ������� ����������������� �� ��� X
//		delta_Y - �������� ������� ����������������� �� ��� Y
//		delta_Z - �������� ������� ����������������� �� ��� Z
//		omega_X - ������� ������� ����������������� �� ��� X
//		omega_Y - ������� ������� ����������������� �� ��� Y
//		omega_Z - ������� ������� ����������������� �� ��� Z
//		m		- ���������� ������� ����������������� �� ��� Z
//		Aa		- ������� ������� ���������� ������� ���������, �� ������� �������������� �����������
//		Ab		- ������� ������� ���������� ������� ���������, � ������� �������������� �����������
//		alpha_a	- ������ ���������� ������� ���������, �� ������� �������������� �����������
//		alpha_b	- ������ ���������� ������� ���������, � ������� �������������� �����������

//										delta_X		delta_Y		delta_Z	  omega_X	  omega_Y	omega_Z				m					   Aa		   Ab			alpha_a					alpha_b
const double gldParameters[7][11] = {{	-1.1,		-0.3,		-0.9,		0.,			0.,		-0.2,		-0.12 * pow(10., -6.),		6378136.,	6378137.,	1. / 298.25784,		1. / 298.257223563,	},
									 {	-0.36,		0.08,		0.18,		0.,			0.,		0.,			0.,							6378136.,	6378137.,	1. / 298.25784,		1. / 298.257223563,	},
									 {	1.07,		0.03,		-0.02,		0.,			0.,		0.13,		0.22 * pow(10., -6.),		6378136.,	6378136.,	1. / 298.25784,		1. / 298.25784,		},
									 {	23.93,		-141.03,	-79.98,		0.,			-0.35,	-0.79,		-0.22 * pow(10., -6.),		6378245.,	6378136.,	1. / 298.3,			1. / 298.25784,		},
									 {	24.83,		-130.97,	-81.74,		0.,			0.,		-0.13,		-0.22 * pow(10., -6.),		6378245.,	6378136.,	1. / 298.3,			1. / 298.25784,		},
									 {	25.,		-141.,		-80.,		0.,			-0.35,	-0.66,		0.,							6378245.,	6378136.,	1. / 298.3,			1. / 298.25784,		},
									 {	25.9,		-130.94,	-81.76,		0.,			0.,		0.,			0.,							6378245.,	6378136.,	1. / 298.3,			1. / 298.25784,		}};
//����� ������� ������ � �������
const double ro = 206264.806;				

double ConvertDegreesToRadians(double dDegrees)
{
	return M_PI * dDegrees / 180.;
}

double ConvertRadiansToDegrees(double dRadians)
{
	return 180. * dRadians / M_PI;
}

bool ConvertGeodeticCoordinates(GeodeticDatum gdFromTo, gPoint gpFrom, gPoint* gpTo)
{
	if (gdFromTo > gdPZ90_SK95)
	{
		switch(gdFromTo)
		{
			case gdWGS84_SK42:
				ConvertGeodeticCoordinates(gdWGS84_PZ90, gpFrom, gpTo);
				gpFrom = *gpTo;
				ConvertGeodeticCoordinates(gdPZ90_SK42, gpFrom, gpTo);
			break;
			case gdWGS84_SK95:
				ConvertGeodeticCoordinates(gdWGS84_PZ90, gpFrom, gpTo);
				gpFrom = *gpTo;
				ConvertGeodeticCoordinates(gdPZ90_SK95, gpFrom, gpTo);
			break;
			default:
			break;
		}
		return true;
	}

	int iDirection(gdFromTo % 2);
	int iIndex(gdFromTo / 2);
	*gpTo = gpFrom;

	//������� ������� ���������������
	double dE2((2. * gldParameters[iIndex][10] - pow(gldParameters[iIndex][10], 2.) + 2. * gldParameters[iIndex][9] - pow(gldParameters[iIndex][9], 2.)) / 2.);
	//�������� ��������� ����������������
	double dDeltaE2((2. * gldParameters[iIndex][10] - pow(gldParameters[iIndex][10], 2.)) - (2. * gldParameters[iIndex][9] - pow(gldParameters[iIndex][9], 2.)));
	//������� ������� �������
	double dA((gldParameters[iIndex][8] + gldParameters[iIndex][7]) / 2.);
	//�������� ������� ��������
	double dDeltaA(gldParameters[iIndex][8] - gldParameters[iIndex][7]);
	//������ �������� ������������ �������
	double dM(dA * (1. - dE2) * pow((1. - dE2 * pow(sin(gpFrom.dLatitude), 2.)), 1.5));
	//������ �������� ������� ���������
	double dN(dA * pow((1 - dE2 * pow(sin(gpFrom.dLatitude), 2.)), 0.5));
	//����� ������
	double dSinLat(sin(ConvertDegreesToRadians(gpFrom.dLatitude)));
	//������� ������
	double dCosLat(cos(ConvertDegreesToRadians(gpFrom.dLatitude)));
	//����� �������
	double dSinLong(sin(ConvertDegreesToRadians(gpFrom.dLongitude)));
	//������� �������
	double dCosLong(cos(ConvertDegreesToRadians(gpFrom.dLongitude)));
	//���������� X (������������� ������� ���������)
	double dX((dN + gpFrom.dAltitude) * dCosLat * dCosLong);
	//���������� Y (������������� ������� ���������)
	double dY((dN + gpFrom.dAltitude) * dCosLat * dSinLong);
	//���������� Z (������������� ������� ���������)
	double dZ(((1 - dE2) * dN + gpFrom.dAltitude) * dSinLat);
	//��������������� ��������
	double dC(ConvertRadiansToDegrees(asin(dZ / pow(pow(dX, 2.) + pow(dY, 2.) + pow(dZ, 2.) , 0.5))) * 3600.);

	//���������� ������
	double dDeltaLatitude	(
								ro / (dM + gpFrom.dAltitude) * 
								(dN / dA * dE2 * dSinLat * dCosLat * dDeltaA + 
								(pow(dN, 2.) / pow(dA, 2.) + 1.) * dN * dSinLat * dCosLat * dDeltaE2 / 2. -
								(gldParameters[iIndex][0] * dCosLong + gldParameters[iIndex][1] * dSinLong) * dSinLat + gldParameters[iIndex][2] * dCosLat) -
								gldParameters[iIndex][3] * dSinLong * (1. + dE2 * cos(2. * ConvertDegreesToRadians(gpFrom.dLatitude))) + 
								gldParameters[iIndex][4] * dCosLong * (1. + dE2 * cos(2. * ConvertDegreesToRadians(gpFrom.dLatitude))) - 
								ro * gldParameters[iIndex][6] * dE2 * dSinLat * dCosLat
							);

	//���������� �������
	double dDeltaLongitude	(
								ro / ((dN + gpFrom.dAltitude) * dCosLat) * (- gldParameters[iIndex][0] * dSinLong + gldParameters[iIndex][1] * dCosLong) +
								tan(ConvertDegreesToRadians(gpFrom.dLatitude)) * (1. - dE2) * (gldParameters[iIndex][3] * dCosLong + gldParameters[iIndex][4] * dSinLong) - 
								gldParameters[iIndex][5]
							);

	//���������� ������
	double dDeltaAltitude	(
								- dA / dN * dDeltaA + dN * pow(dSinLat, 2.) * dDeltaE2 / 2. + (gldParameters[iIndex][0] * dCosLong + gldParameters[iIndex][1] * dSinLong) * dCosLat + 
								gldParameters[iIndex][2] * dSinLat - dN * dE2 * dSinLat * dCosLat * (gldParameters[iIndex][3] / dC * dSinLong - gldParameters[iIndex][4] / dC * dCosLong) + 
								(pow(dA, 2.) / dN + gpFrom.dAltitude) * gldParameters[iIndex][6]
							);

	if (iDirection != 0)
	{
		dDeltaLatitude *= -1.;
		dDeltaLongitude *= -1.;
		dDeltaAltitude *= -1.;
	}
	
	(*gpTo).dLatitude += dDeltaLatitude / 3600.;
	(*gpTo).dLongitude += dDeltaLongitude / 3600.;
	(*gpTo).dAltitude += dDeltaAltitude;
	
	return true;
}

bool GetDeltaBetweenTwoPoints(GeodeticSystem gsSystem, gPoint gpPoint1, gPoint gpPoint2, double* dDeltaX, double* dDeltaY, double* dDeltaZ)
{
	double dA(0.), dE2(0.);
	switch(gsSystem)
	{
		case gsWGS84:
			dA = 6378137.;
			dE2 = 2. * 1. / 298.257223563 - pow(1. / 298.257223563, 2.);
			break;
		case gsSK42:
		case gsSK95:
			dA = 6378245.;
			dE2 = 2. * 1. / 298.3 - pow(1. / 298.3, 2.);
			break;
		case gsPZ90:
		case gsPZ90_02:
			dA = 6378136.;
			dE2 = 2. * 1. / 298.25784 - pow(1. / 298.25784, 2.);
			break;
		default:
			break;
	}
	//����� ������
	double dSinLat1(sin(ConvertDegreesToRadians(gpPoint1.dLatitude)));
	//������� ������
	double dCosLat1(cos(ConvertDegreesToRadians(gpPoint1.dLatitude)));
	//����� �������
	double dSinLong1(sin(ConvertDegreesToRadians(gpPoint1.dLongitude)));
	//������� �������
	double dCosLong1(cos(ConvertDegreesToRadians(gpPoint1.dLongitude)));
	//������ �������� ������� ���������
	double dN1(dA * pow((1 - dE2 * pow(sin(gpPoint1.dLatitude), 2.)), 0.5));
	//���������� X (������������� ������� ���������)
	double dX1((dN1 + gpPoint1.dAltitude) * dCosLat1 * dCosLong1);
	//���������� Y (������������� ������� ���������)
	double dY1((dN1 + gpPoint1.dAltitude) * dCosLat1 * dSinLong1);
	//���������� Z (������������� ������� ���������)
	double dZ1(((1 - dE2) * dN1 + gpPoint1.dAltitude) * dSinLat1);

	//����� ������
	double dSinLat2(sin(ConvertDegreesToRadians(gpPoint2.dLatitude)));
	//������� ������
	double dCosLat2(cos(ConvertDegreesToRadians(gpPoint2.dLatitude)));
	//����� �������
	double dSinLong2(sin(ConvertDegreesToRadians(gpPoint2.dLongitude)));
	//������� �������
	double dCosLong2(cos(ConvertDegreesToRadians(gpPoint2.dLongitude)));
	//������ �������� ������� ���������
	double dN2(dA * pow((1 - dE2 * pow(sin(gpPoint2.dLatitude), 2.)), 0.5));
	//���������� X (������������� ������� ���������)
	double dX2((dN2 + gpPoint2.dAltitude) * dCosLat2 * dCosLong2);
	//���������� Y (������������� ������� ���������)
	double dY2((dN2 + gpPoint2.dAltitude) * dCosLat2 * dSinLong2);
	//���������� Z (������������� ������� ���������)
	double dZ2(((1 - dE2) * dN2 + gpPoint2.dAltitude) * dSinLat2);

	*dDeltaX = dX1 - dX2;
	*dDeltaY = dY1 - dY2;
	*dDeltaZ = dZ1 - dZ2;

	return true;
}


