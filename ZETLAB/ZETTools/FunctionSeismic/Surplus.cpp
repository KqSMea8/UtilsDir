
#ifndef _WIN32_WINNT
#ifndef _WIN32_WINNT_MAXVER
#include <winsdkver.h>
#endif
#define _WIN32_WINNT _WIN32_WINNT_MAXVER
#endif
//------------------------------------------------------------------------------
#include "Surplus.h"

CSurplus::CSurplus()
{
}

CSurplus::~CSurplus()
{
}

void CSurplus::SetStationList(_In_ list<Ssm_str_EventStation> &resStationList)
{
	m_resStationList.assign(resStationList.begin(), resStationList.end());
}

void CSurplus::SetZeroTime(_In_ DATE &time)
{
	m_Time0 = time;
}

void CSurplus::SetDebugOutput(_In_ bool flag)
{
	m_bDebug = flag;
}

void CSurplus::GetRectangle3DOneStation(Ssm_str_EventStation &resStation, CRectangle3D<double> &rect)
{
	bool bDist = resStation.distance > 0.;
	double half = bDist ? resStation.distance : 50.;
	half /= M_OneDegreeOfMeridianInkm;	// переход из км в градусы
	half *= 1.5;		// + 10% на погрешности Vp
	rect.m_x.SetSpaceFromHalf(resStation.latitude, half);

	half /= cosd(resStation.latitude);	// учЄт длины широты
	rect.m_y.SetSpaceFromHalf(resStation.longitude, half);

	//half = (bDist ? resStation.distance : 50.) * 1000. / 2.;
	//rect.m_z.SetSpaceFromHalf(resStation.altitude, half);
	rect.m_z.SetSpaceLeft(-50000.);
	rect.m_z.SetSpaceRight(-1000.);
}

void CSurplus::CalcParameters(CSimplexPoint &point)
{
	point.p.t = FindOptimalTime(point.GetPointEarth());
	point.r = CalcResidual(point.GetPointEarth(), point.p.t);
}

double CSurplus::CalcResidual(CPointEarth &point, DATE time)
{
	double ret(0.);
	for (auto iter = m_resStationList.begin(); iter != m_resStationList.end(); ++iter)
	{
		CPointEarth pointStation(iter->latitude, iter->longitude, iter->altitude);
		double distance_xy(0.);
		if (point.DistanceToPoint(pointStation, distance_xy))
		{
			distance_xy *= 0.001;							// км
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

DATE CSurplus::FindOptimalTime(CPointEarth &point)
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

void CSurplus::TimeInterval()
{
	DATE tmin = 10e+9;
	DATE tmax = 0;
	for (auto iter = m_resStationList.begin(); iter != m_resStationList.end(); ++iter)
	{
		//DATE timep = DATE(zet::tools::time::TimeStamp(iter->zet_header.seconds,
		//	iter->zet_header.nanoseconds, iter->zet_header.ticks));
		DATE timep(0.);
		ztGetStructureTimeUTC(&iter->zet_header, &timep);
		DATE time0 = timep - iter->distance / iter->velocityP * M_SecAsPartOfDay;
		if (tmin > time0 - 0.14 * M_SecAsPartOfDay)
			tmin = time0 - 0.14 * M_SecAsPartOfDay;
		if (tmax < time0 + 0.14 * M_SecAsPartOfDay)
			tmax = time0 + 0.14 * M_SecAsPartOfDay;
	}
	m_time.SetSpaceLeft(tmin);
	m_time.SetSpaceRight(tmax);

	if (m_bDebug)
	{
		CString str;
		str.Format(L"SURPLUS: time %f   %f\n", tmin, tmax);
		OutputDebugStringW(str);
	}
}

void CSurplus::DebugOutput(CString str, CSimplexPoint point)
{
	CString tmp;
	OutputDebugStringW(str + L"\n");
	wchar_t buf[200] = { 0 };
	CZetTime zt(point.p.t);
	zt.ConvertToString(buf, 200, 0.001);
//	ztDATEtoString(point.p.t, buf, 200, 0.001);
	tmp.Format(L"%6.2f N    %6.2f E    %6.0f A    %s T    %.3f R\n",
		point.p.x, point.p.y, point.p.z, buf, point.r);
	OutputDebugStringW(tmp);
}

bool CSurplus::SurplusMethod(_Out_ CSimplexPoint &point)
{
	DWORD dt1 = GetTickCount();

	CRectangle3D<double> curRect;
	DATE TimeT0 = 10e+9;
	auto itf = m_resStationList.begin();
	m_rect.Clear();
	for (auto it = m_resStationList.begin(), ite = m_resStationList.end(); it != ite; ++it)
	{
		GetRectangle3DOneStation(*it, curRect);
		if (it != itf)
			m_rect.SetOnlyCross(curRect);
		else
			m_rect = curRect;
	}

	//point.p.Set(56.9, 99.6, -15000);
	//CalcParameters(point);

	//if (m_bDebug)
	//	DebugOutput(L"SURPLUS REAL POINT", point);

	//m_rect.m_x.SetSpaceFromHalf(56.9, 1.0);
	//m_rect.m_y.SetSpaceFromHalf(99.6, 1.0);

	if (m_bDebug)
	{
		CString str;
		str.Format(L"SURPLUS: rect (%.2f  %.2f) (%.2f  %.2f) (%.3f  %.3f)\n", m_rect.m_x.m_left, m_rect.m_x.m_right,
			m_rect.m_y.m_left, m_rect.m_y.m_right, m_rect.m_z.m_left, m_rect.m_z.m_right);
		OutputDebugStringW(str);
	}

	TimeInterval();

	double kx = 0.1;
	double ky = 0.1;
	double kz = 1000.;
	int nx = (int)(m_rect.m_x.m_radius * 2 / kx) + 1;
	int ny = (int)(m_rect.m_y.m_radius * 2 / ky) + 1;
	int nz = (int)(m_rect.m_z.m_radius * 2 / kz) + 1;
	CSimplexPoint tmp;
	CSimplexPoint optimal;
	optimal.r = 10e+9;
	//optimal = point;
	for (int ix = 0; ix < nx; ix++)
	{
		for (int iy = 0; iy < ny; iy++)
		{
			for (int iz = 0; iz < nz; iz++)
			{
				tmp.p.Set(m_rect.m_x.m_left + ix * kx, m_rect.m_y.m_left + iy * ky,	m_rect.m_z.m_left + iz * kz, 0);
				CalcParameters(tmp);
				if (optimal.r > tmp.r && tmp.r > 0 && m_time.IsValueInsideSpace(tmp.p.t))
					optimal = tmp;

				//if (m_bDebug)
				//	DebugOutput(L"SURPLUS POINT", tmp);
			}
		}
	}

	point = optimal;

	DWORD dt2 = GetTickCount();

	if (m_bDebug)
	{
		OutputDebugStringW(L"\n\n");
		DebugOutput(L"SURPLUS OPTIMAL POINT", point);
		CString outstr;
		outstr.Format(L"work time %u ms\n", dt2 - dt1);
		OutputDebugStringW(outstr);
		outstr.Format(L"selection from %d points\n", nx * ny* nz);
	}

	return true;
}