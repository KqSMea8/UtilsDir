//------------------------------------------------------------------------------
#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <FunctionSeismic\Point_2D_3D.h>
//------------------------------------------------------------------------------
enum GeodeticSystem
{// Тип системы геодезических координат 
	gsWGS84,		// World Geodetic System 1984 (WGS 84)
	gsSK42,			// система координат 1942 года (СК-42)
	gsSK95,			// система координат 1995 года (СК-95)
	gsPZ90,			// Параметры Земли 1990 года (ПЗ-90)
	gsPZ90_02,		// уточнённая версия ПЗ-90 2002 года (ПЗ-90.02)
	gsPZ90_11,		// уточнённая версия ПЗ-90 2011 года (ПЗ-90.11)
	gsGSK2011		// геодезическая системв координат 2011 года (ГСК-2011)
};
//------------------------------------------------------------------------------
struct ParamGeodeticSystem
{
	double maxAxis;	// большая полуось эллипсоида, м
	double eccent2;	// квадрат эксцентриситета эллипсоида
	ParamGeodeticSystem()
	{
		maxAxis = eccent2 = 0.;
	}
	ParamGeodeticSystem(
		double _maxAxis,	// большая полуось эллипсоида
		double compres)		// сжатие эллипсоида
	{
		maxAxis = _maxAxis;
		eccent2 = compres * (2. - compres);
	}
};
//------------------------------------------------------------------------------
const ParamGeodeticSystem g_parWGS84   = ParamGeodeticSystem(6378137.,  1./298.257223563);
const ParamGeodeticSystem g_parSK42    = ParamGeodeticSystem(6378245.,  1./298.3);
const ParamGeodeticSystem g_parSK95    = ParamGeodeticSystem(6378137.,  1./298.25784);
const ParamGeodeticSystem g_parPZ90    = ParamGeodeticSystem(6378136.,  1./298.25784);
const ParamGeodeticSystem g_parPZ90_02 = ParamGeodeticSystem(6378136.,  1./298.25784);
const ParamGeodeticSystem g_parPZ90_11 = ParamGeodeticSystem(6378136.,  1./298.25784);
const ParamGeodeticSystem g_parGSK2011 = ParamGeodeticSystem(6378136.5, 1./298.2564151);
//------------------------------------------------------------------------------
#define M_OneDegreeOfMeridianInkm 111.135
//------------------------------------------------------------------------------
/* Класс "Точка Земли". Имеент три набора координат.
1. Декартовые координаты m_cart (см. ГОСТ Р 51794-2008):
	- начало системы координгат в центре масс Земли;
	- ось Z направлена в Международное условное начало (среднее положение полюса),
		т.е. это ось вращенеия Земли;
	- ось X лежит в плоскости начального меридиана;
	- ось Y дополняет систему координат до правой.
	Таким образом:
	- ось Z направлена на северный полюс;
	- ось X направлена в точку на экваторе с долготой 0 град.;
	- ось Y направлена в точку на экваторе с долготой 180 град.;
2 и 3. Геодезические координаты m_geodDeg и m_geodRad в градусах и радианах:
	- геодезическая широта - угол между нормалью к поверхности эллипсоида,
		проходящий через заданную точку, и плоскостью экватора эллипсоида;
	- геодезическая долгота - двухгранный угол между плоскостями геодезического
		меридиана данной точки и начального меридиана.
	- геодезическая высота - высота точки над поверхностью эллипсоида.
	 Геодезические координаты в Point3D:
		x - широта;
		y - долгота;
		z - высота над уровнем моря
*/
class CPointEarth
{
private:
	bool m_bPointEnable;		// точка класса задана
	Point3D<double> m_cart;		// Мировые декартовы координаты точки класса
	Point3D<double> m_geodDeg;	// геодезические координаты точки класса (градусы)
	Point3D<double> m_geodRad;	// геодезические координаты точки класса (радианы)
	GeodeticSystem m_geodeticSystem;// используемая система геодезических координат
	ParamGeodeticSystem m_CurGeodeticSystem;
	void Init();

	// Преобразование координат
	bool ConvertGeodeticToCartesian(Point3D<double> &geodInpRad,
									Point3D<double> &cartOut);
	bool ConvertCartesianToGeodetic(Point3D<double> &cartInp,
									Point3D<double> &geodOutRad);
protected:

public:
	CPointEarth();
	CPointEarth(const CPointEarth &);
	CPointEarth(double latDeg, double lonDeg, double alt);	// углы в градусах

	void operator = (const CPointEarth &p);
//	virtual ~CPointEarth();

	// функции свойств --------------------------------------------------------
	bool GetPointEnable() { return m_bPointEnable; }

	GeodeticSystem GetGeodeticSystem() { return m_geodeticSystem; }
	bool SetGeodeticSystem(GeodeticSystem system) { return system == gsWGS84; }
	
	// Декартовы координаты точки класса
	bool GetCartesianCoordinates(Point3D<double> &cart);
	bool GetCartesianCoordinates(Point3D<double> &cart) const;
	bool SetCartesianCoordinates(Point3D<double> &cart);
	
	// Геодезические координаты точки класса (углы в градусах или радианах)
	double GetLatitudeInDegree() { return m_geodDeg.x; }
	double GetLongitudeInGegree() { return m_geodDeg.y; }
	double GetAltitudeInMeters() { return m_geodDeg.z; }

	bool GetGeodeticCoordinates(Point3D<double> &geod, bool degree = true);
	bool SetGeodeticCoordinates(Point3D<double> &geodDeg);	// только в градусах

	// функции класса ---------------------------------------------------------
	// Дистанция (в метрах) от точки класса до другой точки
	bool Distance2ToPoint(CPointEarth &point, double &norm);
	bool DistanceToPoint(CPointEarth &point, double &distance);

	// Направление из точки класса на другую точку в координатах
	// ENU (восток, север, вверх)
	bool DirectionENU(CPointEarth &point, Point3D<double> &enu);

	void Clear();
};
//------------------------------------------------------------------------------