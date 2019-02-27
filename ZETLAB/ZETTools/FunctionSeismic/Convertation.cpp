#include "Convertation.h"
#define _USE_MATH_DEFINES
#include <math.h>

// Параметры для перевода координат:
//		delta_X - линейный элемент трансформирования по оси X
//		delta_Y - линейный элемент трансформирования по оси Y
//		delta_Z - линейный элемент трансформирования по оси Z
//		omega_X - угловой элемент трансформирования по оси X
//		omega_Y - угловой элемент трансформирования по оси Y
//		omega_Z - угловой элемент трансформирования по оси Z
//		m		- масштабный элемент трансформирования по оси Z
//		Aa		- большая полуось эллипсоида системы координат, из которой осуществляется конвертация
//		Ab		- большая полуось эллипсоида системы координат, в которую осуществляется конвертация
//		alpha_a	- сжатие эллипсоида системы координат, из которой осуществляется конвертация
//		alpha_b	- сжатие эллипсоида системы координат, в которую осуществляется конвертация

//										delta_X		delta_Y		delta_Z	  omega_X	  omega_Y	omega_Z				m					   Aa		   Ab			alpha_a					alpha_b
const double gldParameters[7][11] = {{	-1.1,		-0.3,		-0.9,		0.,			0.,		-0.2,		-0.12 * pow(10., -6.),		6378136.,	6378137.,	1. / 298.25784,		1. / 298.257223563,	},
									 {	-0.36,		0.08,		0.18,		0.,			0.,		0.,			0.,							6378136.,	6378137.,	1. / 298.25784,		1. / 298.257223563,	},
									 {	1.07,		0.03,		-0.02,		0.,			0.,		0.13,		0.22 * pow(10., -6.),		6378136.,	6378136.,	1. / 298.25784,		1. / 298.25784,		},
									 {	23.93,		-141.03,	-79.98,		0.,			-0.35,	-0.79,		-0.22 * pow(10., -6.),		6378245.,	6378136.,	1. / 298.3,			1. / 298.25784,		},
									 {	24.83,		-130.97,	-81.74,		0.,			0.,		-0.13,		-0.22 * pow(10., -6.),		6378245.,	6378136.,	1. / 298.3,			1. / 298.25784,		},
									 {	25.,		-141.,		-80.,		0.,			-0.35,	-0.66,		0.,							6378245.,	6378136.,	1. / 298.3,			1. / 298.25784,		},
									 {	25.9,		-130.94,	-81.76,		0.,			0.,		0.,			0.,							6378245.,	6378136.,	1. / 298.3,			1. / 298.25784,		}};
//Число угловых секунд в радиане
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

	//средний квадрат эксцентриситета
	double dE2((2. * gldParameters[iIndex][10] - pow(gldParameters[iIndex][10], 2.) + 2. * gldParameters[iIndex][9] - pow(gldParameters[iIndex][9], 2.)) / 2.);
	//разность квадратов эксцентриситетов
	double dDeltaE2((2. * gldParameters[iIndex][10] - pow(gldParameters[iIndex][10], 2.)) - (2. * gldParameters[iIndex][9] - pow(gldParameters[iIndex][9], 2.)));
	//средняя большая полуось
	double dA((gldParameters[iIndex][8] + gldParameters[iIndex][7]) / 2.);
	//разность больших полуосей
	double dDeltaA(gldParameters[iIndex][8] - gldParameters[iIndex][7]);
	//радиус кривизны меридианного сечения
	double dM(dA * (1. - dE2) * pow((1. - dE2 * pow(sin(gpFrom.dLatitude), 2.)), 1.5));
	//радиус кривизны первого вертикала
	double dN(dA * pow((1 - dE2 * pow(sin(gpFrom.dLatitude), 2.)), 0.5));
	//синус широты
	double dSinLat(sin(ConvertDegreesToRadians(gpFrom.dLatitude)));
	//косинус широты
	double dCosLat(cos(ConvertDegreesToRadians(gpFrom.dLatitude)));
	//синус долготы
	double dSinLong(sin(ConvertDegreesToRadians(gpFrom.dLongitude)));
	//косинус долготы
	double dCosLong(cos(ConvertDegreesToRadians(gpFrom.dLongitude)));
	//координата X (прямоугольная система координат)
	double dX((dN + gpFrom.dAltitude) * dCosLat * dCosLong);
	//координата Y (прямоугольная система координат)
	double dY((dN + gpFrom.dAltitude) * dCosLat * dSinLong);
	//координата Z (прямоугольная система координат)
	double dZ(((1 - dE2) * dN + gpFrom.dAltitude) * dSinLat);
	//вспомогательный параметр
	double dC(ConvertRadiansToDegrees(asin(dZ / pow(pow(dX, 2.) + pow(dY, 2.) + pow(dZ, 2.) , 0.5))) * 3600.);

	//приращение широты
	double dDeltaLatitude	(
								ro / (dM + gpFrom.dAltitude) * 
								(dN / dA * dE2 * dSinLat * dCosLat * dDeltaA + 
								(pow(dN, 2.) / pow(dA, 2.) + 1.) * dN * dSinLat * dCosLat * dDeltaE2 / 2. -
								(gldParameters[iIndex][0] * dCosLong + gldParameters[iIndex][1] * dSinLong) * dSinLat + gldParameters[iIndex][2] * dCosLat) -
								gldParameters[iIndex][3] * dSinLong * (1. + dE2 * cos(2. * ConvertDegreesToRadians(gpFrom.dLatitude))) + 
								gldParameters[iIndex][4] * dCosLong * (1. + dE2 * cos(2. * ConvertDegreesToRadians(gpFrom.dLatitude))) - 
								ro * gldParameters[iIndex][6] * dE2 * dSinLat * dCosLat
							);

	//приращение долготы
	double dDeltaLongitude	(
								ro / ((dN + gpFrom.dAltitude) * dCosLat) * (- gldParameters[iIndex][0] * dSinLong + gldParameters[iIndex][1] * dCosLong) +
								tan(ConvertDegreesToRadians(gpFrom.dLatitude)) * (1. - dE2) * (gldParameters[iIndex][3] * dCosLong + gldParameters[iIndex][4] * dSinLong) - 
								gldParameters[iIndex][5]
							);

	//приращение высоты
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
	//синус широты
	double dSinLat1(sin(ConvertDegreesToRadians(gpPoint1.dLatitude)));
	//косинус широты
	double dCosLat1(cos(ConvertDegreesToRadians(gpPoint1.dLatitude)));
	//синус долготы
	double dSinLong1(sin(ConvertDegreesToRadians(gpPoint1.dLongitude)));
	//косинус долготы
	double dCosLong1(cos(ConvertDegreesToRadians(gpPoint1.dLongitude)));
	//радиус кривизны первого вертикала
	double dN1(dA * pow((1 - dE2 * pow(sin(gpPoint1.dLatitude), 2.)), 0.5));
	//координата X (прямоугольная система координат)
	double dX1((dN1 + gpPoint1.dAltitude) * dCosLat1 * dCosLong1);
	//координата Y (прямоугольная система координат)
	double dY1((dN1 + gpPoint1.dAltitude) * dCosLat1 * dSinLong1);
	//координата Z (прямоугольная система координат)
	double dZ1(((1 - dE2) * dN1 + gpPoint1.dAltitude) * dSinLat1);

	//синус широты
	double dSinLat2(sin(ConvertDegreesToRadians(gpPoint2.dLatitude)));
	//косинус широты
	double dCosLat2(cos(ConvertDegreesToRadians(gpPoint2.dLatitude)));
	//синус долготы
	double dSinLong2(sin(ConvertDegreesToRadians(gpPoint2.dLongitude)));
	//косинус долготы
	double dCosLong2(cos(ConvertDegreesToRadians(gpPoint2.dLongitude)));
	//радиус кривизны первого вертикала
	double dN2(dA * pow((1 - dE2 * pow(sin(gpPoint2.dLatitude), 2.)), 0.5));
	//координата X (прямоугольная система координат)
	double dX2((dN2 + gpPoint2.dAltitude) * dCosLat2 * dCosLong2);
	//координата Y (прямоугольная система координат)
	double dY2((dN2 + gpPoint2.dAltitude) * dCosLat2 * dSinLong2);
	//координата Z (прямоугольная система координат)
	double dZ2(((1 - dE2) * dN2 + gpPoint2.dAltitude) * dSinLat2);

	*dDeltaX = dX1 - dX2;
	*dDeltaY = dY1 - dY2;
	*dDeltaZ = dZ1 - dZ2;

	return true;
}


