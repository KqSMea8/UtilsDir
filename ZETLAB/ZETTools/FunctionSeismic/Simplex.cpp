
#ifndef _WIN32_WINNT
#ifndef _WIN32_WINNT_MAXVER
#include <winsdkver.h>
#endif
#define _WIN32_WINNT _WIN32_WINNT_MAXVER
#endif
//------------------------------------------------------------------------------

#include "Simplex.h"
#include <ZetTools\include\ZetTools.h>
#include <functional>


CSimplex::CSimplex()
{
}

CSimplex::~CSimplex()
{
}

void CSimplex::SetStationList(_In_ list<Ssm_str_EventStation> &resStationList)
{
	m_resStationList.assign(resStationList.begin(), resStationList.end());
}

void CSimplex::SetZeroTime(_In_ DATE &time)
{
	m_Time0 = time;
}

void CSimplex::SetDebugOutput(_In_ bool flag)
{
	m_bDebug = flag;
}

bool CSimplex::RGreater(const CSimplexPoint &point1, const CSimplexPoint &point2)
{
	return point1.r > point2.r;
}

bool CSimplex::TestConvergence()
{
	double tmin, tmax, xmax, xmin, ymax, ymin, zmax, zmin;
	tmin = tmax = m_simplex[0].p.t;
	xmax = xmin = m_simplex[0].p.x;
	ymax = ymin = m_simplex[0].p.y;
	zmax = zmin = m_simplex[0].p.z;
	for (int i = 1, size = m_simplex.size(); i < size; i++)
	{
		if (tmin > m_simplex[i].p.t)
			tmin = m_simplex[i].p.t;
		if (tmax < m_simplex[i].p.t)
			tmax = m_simplex[i].p.t;
		if (xmin > m_simplex[i].p.x)
			xmin = m_simplex[i].p.x;
		if (xmax < m_simplex[i].p.x)
			xmax = m_simplex[i].p.x;
		if (ymin > m_simplex[i].p.y)
			ymin = m_simplex[i].p.y;
		if (ymax < m_simplex[i].p.y)
			ymax = m_simplex[i].p.y;
		if (zmin > m_simplex[i].p.z)
			zmin = m_simplex[i].p.z;
		if (zmax < m_simplex[i].p.z)
			zmax = m_simplex[i].p.z;
	}
	double deltat = (tmax - tmin) * M_NumberSecInDay;
	double deltax = (xmax - xmin);
	double deltay = (ymax - ymin);
	double deltaz = (zmax - zmin);
	if (deltax < 0.01 && deltay < 0.01 && deltaz < 1000.)
		return false;
	else
		return true;
}

void CSimplex::SortByResidual()
{
	bool test(true);
	while (test)
	{
		test = false;
		for (int i = 0, size = m_simplex.size(); i < size - 1; i++)
		{
			if (m_simplex[i].r < m_simplex[i + 1].r)
			{
				CSimplexPoint temp = m_simplex[i];
				m_simplex[i] = m_simplex[i + 1];
				m_simplex[i + 1] = temp;
				test = true;
			}
		}
	}
}

void CSimplex::GetRectangle3DOneStation(Ssm_str_EventStation &resStation, CRectangle3D<double> &rect)
{
	bool bDist = resStation.distance > 0.;
	double half = bDist ? resStation.distance : 50.;
	half /= M_OneDegreeOfMeridianInkm;	// переход из км в градусы
	half *= 1.5;		// + 10% на погрешности Vp
	rect.m_x.SetSpaceFromHalf(resStation.latitude, half);

	half /= cosd(resStation.latitude);	// учЄт длины широты
	rect.m_y.SetSpaceFromHalf(resStation.longitude, half);

	half = (bDist ? resStation.distance : 50.) * 1000. / 2.;
	rect.m_z.SetSpaceFromHalf(resStation.altitude, half);
}

DATE CSimplex::FindOptimalTime(CPointEarth &point)
{
	// определение времени начала землетр€сени€, которое имеет минимальную нев€зку
	// в результате "долгих" алгебраических манипул€ций была выведена формула
	double distance(0.);
	double time(0.);
	long count(0);
	for (auto iter = m_resStationList.begin(); iter != m_resStationList.end(); ++iter)
	{
		CPointEarth pointStation(iter->latitude, iter->longitude, iter->altitude);
		double distance_xy(0.);
		//double timep = DATE(zet::tools::time::TimeStamp(iter->zet_header.seconds,
		//	iter->zet_header.nanoseconds, iter->zet_header.ticks));
		double timep(0.);
		ztGetStructureTimeUTC(&iter->zet_header, &timep);

		//double tmptime = timep * M_NumberSecInDay;
		if (point.DistanceToPoint(pointStation, distance_xy) && timep > 0)
		{
			distance += distance_xy / (iter->velocityP * 1000.);
			time += timep;
			count++;
		}
	}
	double ret = (time - distance * M_SecAsPartOfDay) / count;
	return DATE(ret);
}

double CSimplex::CalcResidual(CPointEarth &point, DATE time)
{
	double ret(0.);
	for (auto iter = m_resStationList.begin(); iter != m_resStationList.end(); ++iter)
	{
		CPointEarth pointStation(iter->latitude, iter->longitude, iter->altitude);
		double distance_xy(0.);
		if (point.DistanceToPoint(pointStation, distance_xy))
		{
			distance_xy *= 0.001;							// км
			//DATE Tp = DATE(zet::tools::time::TimeStamp(iter->zet_header.seconds,
			//	iter->zet_header.nanoseconds, iter->zet_header.ticks));
			DATE Tp(0.);
			ztGetStructureTimeUTC(&iter->zet_header, &Tp);

			//double dt = (Tp - m_Time0) * M_NumberSecInDay - time;
			double dt = (Tp - time) * M_NumberSecInDay;
			double distance_vt = iter->velocityP * dt;		//	км
			double residual = distance_xy - distance_vt;
			ret += residual * residual;						//	км^2
		}
	}
	ret = sqrt(ret / m_resStationList.size());
	return ret;
}

void CSimplex::CalcParameters(CSimplexPoint &point)
{
	point.r = CalcResidual(point.GetPointEarth(), point.p.t);
}

void CSimplex::CenterOfMass()
{
	CSimplexPoint com;
	double mass = 0;
	for (auto iter = m_simplex.begin() + 1; iter != m_simplex.end(); iter++)
	{
		com.p += iter->p * iter->r;
		mass += iter->r;
	}
	if (mass == 0)
		m_bError = true;
	com.p /= mass;
	CalcParameters(com);
	m_center = com;

	//if (m_bDebug)
	//	DebugOutput(L"SIMPLEX CENTERofMASS", com);
}

void CSimplex::Reflection()
{
	CSimplexPoint ref;
	ref.p = (1 + m_alpha) * m_center.p - m_alpha * m_simplex[0].p;
	//PointInRect(ref);
	CalcParameters(ref);
	m_reflect = ref;

	//if (m_bDebug)
	//	DebugOutput(L"SIMPLEX REFLECTION", ref);
}

void CSimplex::ReflectionGamma()
{
	CSimplexPoint reg;
	reg.p = (1 - m_gamma) * m_center.p + m_gamma * m_reflect.p;
	//PointInRect(reg);
	CalcParameters(reg);
	// если раст€жение прошло "удачно", то выбираем "раст€нутую" точку и заносим еЄ в вектор вместо ервой
	if (reg.r < m_reflect.r)
	{
		m_simplex[0] = reg;
	}
	else
	{
		m_simplex[0] = m_reflect;
	}

	//if (m_bDebug)
	//	DebugOutput(L"SIMPLEX REFLECTIONtoGAMMA", reg);
}

void CSimplex::Pressing()
{
	CSimplexPoint prs;
	if (!m_simplex.empty())
	{
		prs.p = m_betta * m_simplex[0].p + (1 - m_betta) * m_reflect.p;
		CalcParameters(prs);
	}

	if (prs.r < m_simplex[0].r)
	{
		m_simplex[0] = prs;

		//if (m_bDebug)
		//	DebugOutput(L"SIMPLEX PRESSING", prs);
	}
	else
	{
		Dilatation();
	}
}

void CSimplex::Dilatation()
{
	for (long i = 0, size = m_simplex.size(); i < size - 1; i++)
	{
		m_simplex[i].p = m_simplex[size - 1].p + (m_simplex[i].p - m_simplex[size - 1].p) / 2;
		CalcParameters(m_simplex[i]);
	}

	//if (m_bDebug)
	//	DebugOutput(L"SIMPLEX DELATETION");
}

void CSimplex::PointInRect(CSimplexPoint &point)
{
	//CSimplexPoint tmp = point;
	//if (!m_rect.IsPointInside(tmp.p.x, tmp.p.y, tmp.p.z))
	{
		if (point.p.x < m_rect.m_x.m_left)
			point.p.x = m_rect.m_x.m_left;
		if (point.p.x > m_rect.m_x.m_right)
			point.p.x = m_rect.m_x.m_right;
		if (point.p.y < m_rect.m_y.m_left)
			point.p.y = m_rect.m_y.m_left;
		if (point.p.y > m_rect.m_y.m_right)
			point.p.y = m_rect.m_y.m_right;
		if (point.p.z < m_rect.m_z.m_left)
			point.p.z = m_rect.m_z.m_left;
		if (point.p.z > m_rect.m_z.m_right)
			point.p.z = m_rect.m_z.m_right;
		//CalcParameters(point);
	}
}

void CSimplex::SetInitialPoint(Point4D<double> point)
{
	CSimplexPoint point1;
	point1.p = point;
	CalcParameters(point1);
	m_simplex.push_back(point1);

	CSimplexPoint point2 = point1;
	point2.p.x += 0.1;
	CalcParameters(point2);
	m_simplex.push_back(point2);

	point2 = point1;
	point2.p.y += 0.1;
	CalcParameters(point2);
	m_simplex.push_back(point2);

	point2 = point1;
	point2.p.z += 1000;
	CalcParameters(point2);
	m_simplex.push_back(point2);

	point2 = point1;
	point2.p.t += M_SecAsPartOfDay;
	CalcParameters(point2);
	m_simplex.push_back(point2);
}

bool CSimplex::SimplexMethod(_Out_ CSimplexPoint &point)
{
	DWORD dt1 = GetTickCount();
	vector<CSimplexPoint> results;
	vector<Point4D<double>> starts;
	Point4D<double> pmax, pmin;
	for (auto iter = m_resStationList.begin(); iter != m_resStationList.end(); ++iter)
	{
		//double timep = DATE(zet::tools::time::TimeStamp(iter->zet_header.seconds,
		//	iter->zet_header.nanoseconds, iter->zet_header.ticks));
		double timep(0.);
		ztGetStructureTimeUTC(&iter->zet_header, &timep);
		starts.push_back(Point4D<double>(iter->latitude, iter->longitude, iter->altitude, timep));
		if (starts.size() == 1)
		{
			pmax = starts.front();
			pmin = starts.front();
		}
		else
		{
			if (pmax.x < starts.back().x)
				pmax.x = starts.back().x;
			if (pmax.y < starts.back().y)
				pmax.y = starts.back().y;
			if (pmax.z < starts.back().z)
				pmax.z = starts.back().z;
			if (pmax.t < starts.back().t)
				pmax.t = starts.back().t;
			if (pmin.x > starts.back().x)
				pmin.x = starts.back().x;
			if (pmin.y > starts.back().y)
				pmin.y = starts.back().y;
			if (pmin.z > starts.back().z)
				pmin.z = starts.back().z;
			if (pmin.t > starts.back().t)
				pmin.t = starts.back().t;
		}
	}

	starts.push_back(Point4D<double>(pmax.x + 5.0, pmax.y + 5.0, pmax.z, pmin.t));
	starts.push_back(Point4D<double>(pmin.x - 5.0, pmax.y + 5.0, pmax.z, pmin.t));
	starts.push_back(Point4D<double>(pmin.x - 5.0, pmin.y - 5.0, pmax.z, pmin.t));
	starts.push_back(Point4D<double>(pmax.x + 5.0, pmin.y - 5.0, pmax.z, pmin.t));
	starts.push_back((pmax + pmin)/2);

	for (auto iter = starts.begin(); iter != starts.end(); ++iter)
	{
		if (!m_simplex.empty())
			m_simplex.clear();

		SetInitialPoint(*iter);

		long size = m_simplex.size();

		//if (m_bDebug)
		//	DebugOutput(L"SIMPLEX START");

		// цикл поиска оптимального значени€ координаты гиппоцентра
		m_bError = false;
		long count = 0;
		long maxCount = 120;
		while (TestConvergence() && count < maxCount)
		{
			count++;

			if (m_bError)
				break;

			SortByResidual();
			CenterOfMass();
			Reflection();
			// проверка оптимальности выбранного шага
			if (m_reflect.r < m_simplex[0].r)
			{
				ReflectionGamma();
				continue;
			}

			if (m_reflect.r >= m_simplex[size - 1].r && m_reflect.r < m_simplex[1].r)
			{
				m_simplex[0] = m_reflect;
				continue;
			}

			if (m_reflect.r >= m_simplex[1].r && m_reflect.r < m_simplex[0].r)
			{
				CSimplexPoint temp = m_reflect;
				m_simplex[0] = m_reflect;
				m_reflect = temp;
			}
			// на этом шаге очевидно, что m_reflect.r >= m_simplex[0].r
			// производим "сжатие" и переходим к следующему циклу оптимизации
			Dilatation();
		}

		if (!m_bError)
		{
			CSimplexPoint res;
			for (auto iter = m_simplex.begin(); iter != m_simplex.end(); ++iter)
				res.p += iter->p;
			res.p /= (int)m_simplex.size();
			CalcParameters(res);
			point = res;

			results.push_back(res);

			/*if (m_bDebug)
			{
				if (count == maxCount)
				{
					DebugOutput(L"SIMPLEX END by COUNT", res);
				}
				else
				{
					CString str;
					str.Format(L"SIMPLEX END by CONVERGENCE    %d", count);
					DebugOutput(str, res);
				}
			}*/
		}
		else
		{
			//CString str;
			//str.Format(L"SIMPLEX END by ERROR    %d", count);
			//DebugOutput(str);
		}
	}
	
	point.r = 10e+9;
	for (int i = 0, size = results.size(); i < size; i++)
	{
		if (point.r > results[i].r)
			point = results[i];
	}

	DWORD dt2 = GetTickCount();

	if (m_bDebug)
	{
		//OutputDebugStringW(L"\n\n");
		DebugOutput(L"SIMPLEX OPTIMAL POINT", point);
		//CString outstr;
		//outstr.Format(L"work time %d ms\n", dt2 - dt1);
		//OutputDebugStringW(outstr);
	}
	
	return true;
}

void CSimplex::DebugOutput(CString str)
{
	CString tmp;
	OutputDebugStringW(str + L"\n");
	for (int i = 0, size = m_simplex.size(); i < size; i++)
	{
		wchar_t buf[200] = { 0 };
		CZetTime zt(m_simplex[i].p.t);
		zt.ConvertToString(buf, 200, 0.001);
	//	ztDATEtoString(m_simplex[i].p.t, buf, 200, 0.001);
		tmp.Format(L"%6.2f N    %6.2f E    %6.0f A    %s T    %6.3f R\n", 
			m_simplex[i].p.x, m_simplex[i].p.y, m_simplex[i].p.z, buf, m_simplex[i].r);
		OutputDebugStringW(tmp);
	}
}

void CSimplex::DebugOutput(CString str, CSimplexPoint point)
{
	CString tmp;
	OutputDebugStringW(str + L"\n");
	wchar_t buf[200] = { 0 };
	CZetTime zt(point.p.t);
	zt.ConvertToString(buf, 200, 0.001);
//	ztDATEtoString(point.p.t, buf, 200, 0.001);
	tmp.Format(L"%6.2f N    %6.2f E    %6.0f A    %s T    %6.3f R\n", 
		point.p.x, point.p.y, point.p.z, buf, point.r);
	OutputDebugStringW(tmp);
}