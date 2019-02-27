//------------------------------------------------------------------------------
//	Файл FindHypo.cpp
//------------------------------------------------------------------------------
#ifndef _WIN32_WINNT
#ifndef _WIN32_WINNT_MAXVER
#include <winsdkver.h>
#endif
#define _WIN32_WINNT _WIN32_WINNT_MAXVER
#endif
//------------------------------------------------------------------------------
#include "FindHypo.h"
#include <crtdbg.h>
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
namespace zet {
namespace algorithm	{
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------
CFindHypo::CFindHypo()
{
	m_pConfig = &m_config.zet_header;
	m_pResult = &m_result.zet_header;
	
	ztOpenStructure(m_pConfig, ZETLAB_SSM_CFG_ALGR_FINDHYPO_TYPE, ZETLAB_SSM_CFG_ALGR_FINDHYPO_SIZE);
	ztCloseStructure(m_pConfig);

	ztOpenStructure(m_pResult, ZETLAB_SSM_RES_ALGR_FINDHYPO_TYPE, ZETLAB_SSM_RES_ALGR_FINDHYPO_SIZE);
}
//------------------------------------------------------------------------------
CFindHypo::CFindHypo(const CFindHypo &val)
{
	*this = val;
}
//------------------------------------------------------------------------------
CFindHypo::~CFindHypo()
{
}
//------------------------------------------------------------------------------
void CFindHypo::operator=(const CFindHypo &val)
{
	m_config = val.m_config;
	m_result = val.m_result;
}
//------------------------------------------------------------------------------
long CFindHypo::SetConfigFromProxy()
{
	long ret(NOERROR);
	m_config = m_cfgClass;
	if (0 != ztCloseStructure(m_pConfig))
		ret = Err_Copy_Structure;
	return ret;
}
//------------------------------------------------------------------------------
long CFindHypo::SetConfig(ZetHeader* pParam)
{// проверка пришедшей структуры
	long ret = CCustomAlgorithm::SetConfig(pParam);
	if (ret == NoError)
	{	// копирование этой структуры в Proxy с целью проверки полей структуры
		try
		{
			m_cfgClass = *((Ssm_cfg_Algr_FindHypo*)pParam);
		}
		catch (...)
		{
			ret = ErrorCodes::InvalidParams;
		}
		// копирование параметрров из Proxy в структуру класса
		SetConfigFromProxy();
	}
	return ret;
}
//------------------------------------------------------------------------------
void CFindHypo::GetRectangleOneStation(
	Ssm_str_EventStation &resStation,
	DATE meanT0,
	CRectangle<double> &rect,
	CSpace<double> &t0)
{
	bool bDist = resStation.distance > 0.;
	double half = bDist ? resStation.distance : 50.;
	half /= M_OneDegreeOfMeridianInkm;	// переход из км в градусы
	half *= 1.1;		// + 10% на погрешности Vp
	rect.m_x.SetSpaceFromHalf(resStation.latitude, half);

	half /= cosd(resStation.latitude);	// учёт длины широты
	rect.m_y.SetSpaceFromHalf(resStation.longitude, half);
	
	//DATE Tp = DATE(zet::tools::time::TimeStamp(resStation.zet_header.seconds,
	//	resStation.zet_header.nanoseconds, resStation.zet_header.ticks));
	DATE Tp(0.);
	ztGetStructureTimeUTC(&resStation.zet_header, &Tp);

	// переход ко времени в сек (0 сек - это DATE meanT0)
	double timeP = (Tp - meanT0) * M_NumberSecInDay;
	if (bDist)
	{
		double time = resStation.distance / resStation.velocityP;
		double half = 0.1 * time;	// 10% на погрешности Vp
		t0.SetSpaceFromHalf(timeP - time, half);
	}
	else
	{
		double time = 50. / resStation.velocityP;
		t0.m_right = timeP;
		t0.SetSpaceLeft(timeP - time);
	}
}
//------------------------------------------------------------------------------
/*void CFindHypo::GetRectangle3DOneStation(
	Ssm_str_res_Station &resStation, 
	CRectangle3D<double> &rect)
{
	bool bDist = resStation.distance > 0.;
	double half = bDist ? resStation.distance : 50.;
	half /= M_OneDegreeOfMeridianInkm;	// переход из км в градусы
	half *= 1.1;		// + 10% на погрешности Vp
	rect.m_x.SetSpaceFromHalf(resStation.latitude, half);

	half /= cosd(resStation.latitude);	// учёт длины широты
	rect.m_y.SetSpaceFromHalf(resStation.longitude, half);

	half = (bDist ? resStation.distance : 50.) * 1000. / 2.;
	rect.m_z.SetSpaceFromHalf(resStation.altitude, half);
}*/
//------------------------------------------------------------------------------
double CFindHypo::CalcResidual(
	_In_ list<Ssm_str_EventStation> &resStationList,
	DATE meanT0,
	_In_ CPointEarth &point,
	_In_ double time)
{
	double ret(0.);
	for_each(resStationList.begin(), resStationList.end(),
		[&](Ssm_str_EventStation &res)
		{
			CPointEarth pointStation(res.latitude,
				res.longitude, res.altitude);
			double distance_xy(0.);
			if (point.DistanceToPoint(pointStation, distance_xy))
			{
				distance_xy *= 0.001;
				//DATE Tp = DATE(zet::tools::time::TimeStamp(res.zet_header.seconds,
				//	res.zet_header.nanoseconds,
				//	res.zet_header.ticks));
				DATE Tp(0.);
				ztGetStructureTimeUTC(&res.zet_header, &Tp);
				double dt = (Tp - meanT0) * M_NumberSecInDay - time;
				double distance_vt = res.velocityP * dt;
				double residual = distance_xy - distance_vt;
				ret += residual * residual;
			}
		}
	);
	ret = sqrt(ret / resStationList.size());
	return ret;
}
//------------------------------------------------------------------------------
void CFindHypo::FindEpicenterAndT0(
	list<Ssm_str_EventStation> &resStationList,	// перечень станций с Tp
	DATE meanT0,
	CRectangle<double> &region,	// регион поиска
	CSpace<double> &t0,			// интервал время начала в очаге
	double &epicenterLat,		// найденная широта эпицентра
	double &epicenterLon,		// найденная долгота эпицентра
	double &residual)			// итоговая невязка
{
	int nt0  = (int)floor(t0.GetLength() / m_config.precisionT0);
	int nLat = (int)floor(region.m_x.GetLength() / m_config.precisionLatitude);
	int nLon = (int)floor(region.m_y.GetLength() / m_config.precisionLongitude);

	if (m_bNeedOutputDebug)
	{
		CString s;
		s.Format(L"SKSV: FindHypo. nT0 = %d, nLat = %d, nLon = %d\n", nt0, nLat, nLon);
		OutputDebugStringW(s);

		s.Format(L"SKSV: FindHypo. T0 = %f, %f\n", t0.m_left, t0.m_left + nt0 * (double)m_config.precisionT0);
		OutputDebugStringW(s);
		s.Format(L"SKSV: FindHypo. Lat = %f, %f\n", region.m_x.m_left, region.m_x.m_left + nLat * (double)m_config.precisionLatitude);
		OutputDebugStringW(s);
		s.Format(L"SKSV: FindHypo. Lon = %f, %f\n", region.m_y.m_left , region.m_y.m_left + nLon * (double)m_config.precisionLongitude);
		OutputDebugStringW(s);
	}
	// задано перед вызовом этой функции residual = 1.e77;
	for (int it0 = 0; it0 <= nt0; ++it0)
	{
		double curT0 = t0.m_left + it0 * (double)m_config.precisionT0;
		for (int iLat = 0; iLat <= nLat; ++iLat)
		{
			double curLat = region.m_x.m_left + iLat * (double)m_config.precisionLatitude;
			for (int iLon = 0; iLon <= nLon; ++iLon)
			{
				double curLon = region.m_y.m_left + iLon * (double)m_config.precisionLongitude;
				CPointEarth pointCur(curLat, curLon, 0);
				double resid = CalcResidual(resStationList, meanT0, pointCur, curT0);
				if (residual > resid)
				{
					epicenterLat = curLat;
					epicenterLon = curLon;
					t0.m_mean = curT0;
					residual = resid;
				}
			}
		}
	}
	t0.SetSpaceFromHalf(t0.m_mean, m_config.precisionT0);

	if (m_bNeedOutputDebug)
	{
		CString s;
		s.Format(L"SKSV: FindHypo. epicenter => Latitude = %f, Longitude = %f, residual = %f\n",
			epicenterLat, epicenterLon, residual);
		OutputDebugStringW(s);
	}
}
//------------------------------------------------------------------------------
void CFindHypo::FindDepth(
	list<Ssm_str_EventStation> &resStationList,
	DATE meanT0,
	const double epicenterLat,
	const double epicenterLon,
	const double t0,
	double &depth,
	double &residual)

{// поиск глубины гипоцентра
	double res(1.e77);
	double max_D(800.), min_D(0.);
	double dD = 100.;
	double find_D(400.);
	double delta_D = 10 * m_config.precisionDepth;

	int nD(9);
	int nD_2 = nD / 2;
	int step(0);

	residual = CalcResidual(resStationList, meanT0,
		CPointEarth(epicenterLat, epicenterLon, 0.), t0);
	do 
	{
		++step;
		double dd = nD_2 * dD;
		min_D = find_D - dd;
		max_D = find_D + dd;
		if (min_D < 0.)
			min_D = 0.;
		for (int i = 0; i < nD; ++i)
		{
			double cur_D = min_D + i * dD;
			if ( (0. < cur_D) && (cur_D <= max_D))
			{
				CPointEarth pointCur(epicenterLat, epicenterLon, -cur_D * 1000);
				res = CalcResidual(resStationList, meanT0, pointCur, t0);
				//if (m_bNeedOutputDebug)
				//{
				//	CString s;
				//	s.Format(L"SKSV: FindHypo. step = %d, depth  = %f, residual = %f\n",
				//		step, cur_D, res);
				//	OutputDebugStringW(s);
				//}
				if (residual > res)
				{
					residual = res;
					find_D = cur_D;
				}
			}
		}
		dD *= M_GoldenSection_1;
		if ((step == 1) && (find_D == 400.))
			find_D = 0.;
	} while (dD > delta_D);

	min_D = floor(find_D + 0.5) - 5.;
	if (min_D < 0.)
		min_D = 0.;
	for (int i = 0; i <= 10; ++i)
	{
		double cur_D = min_D + i;
		CPointEarth pointCur(epicenterLat, epicenterLon, -cur_D * 1000);
		res = CalcResidual(resStationList, meanT0, pointCur, t0);
		if (residual > res)
		{
			residual = res;
			find_D = cur_D;
		}
	}

	depth = find_D;
}
//------------------------------------------------------------------------------
/*DATE CFindHypo::FindOptimalTime(
	_In_ list<Ssm_str_res_Station> &resStationList,
	_In_ CPointEarth &point)
{// определение времени начала землетрясения, которое имеет минимальную невязку
	// в результате "долгих" алгебраических манипуляций была выведена формула
	double distance(0.);
	double time(0.);
	long count(0);
	for_each(resStationList.begin(), resStationList.end(), [&](Ssm_str_res_Station &res)
	{
		CPointEarth pointStation(res.latitude,
			res.longitude, res.altitude);
		double distance_xy(0.);
		double timep = DATE(zet::tools::time::TimeStamp(res.zet_header.seconds,
			res.zet_header.nanoseconds, res.zet_header.ticks));
		if (point.DistanceToPoint(pointStation, distance_xy) && timep > 0)
		{
			distance += distance_xy / res.velocityP;
			time += timep;
			count++;
		}
	});
	time = time / count - distance / count;
	return DATE(time);
}*/
//------------------------------------------------------------------------------
long CFindHypo::SimplexMethod(
	_In_ list<Ssm_str_EventStation> &resStationList,
	_Out_ double& r)
{// поиск гиппоцентра землетрясения по методу Нелдера-Мида
	r = NAN;
	if (m_bNeedOutputDebug)
		OutputDebugStringW(L"SKSV: FindHypo start SIMPLEX\n");

	DATE TimeT0 = 10e+9;
	auto itf = resStationList.begin();
	for (auto it = resStationList.begin(), ite = resStationList.end(); it != ite; ++it)
	{
		DATE TimeTemp(0.);
		if (0 == ztGetStructureTimeUTC(&it->zet_header, &TimeTemp))
		{
			if (TimeT0 > TimeTemp)
				TimeT0 = TimeTemp;
		}
	}

	TimeT0 -= 300. / M_NumberSecInDay;

	CString sTemp;
	CSimplex simplex;
	CSimplexPoint hyppocenter;
	list<CSimplexPoint> Hypos;

	bool res;
	DWORD dt1 = GetTickCount();

	sTemp.Format(L"All stations\n");
	if (m_bNeedOutputDebug)
		OutputDebugStringW(sTemp);
	simplex.SetStationList(resStationList);
	simplex.SetZeroTime(TimeT0);
	simplex.SetDebugOutput(m_bNeedOutputDebug);
	res = simplex.SimplexMethod(hyppocenter);
	Hypos.push_back(hyppocenter);
	
	if (hyppocenter.r > MAX_RESIDUAL)
	{
		if (resStationList.size() > 3)
		{
			list<Ssm_str_EventStation> StationList;
			list<Ssm_str_EventStation>::iterator resStationIter;
			list<Ssm_str_EventStation>::iterator StationIter;
			for (resStationIter = resStationList.begin(); resStationIter != resStationList.end(); ++resStationIter)
			{
				StationList.assign(resStationList.begin(), resStationList.end());
				for (StationIter = StationList.begin(); StationIter != StationList.end(); ++StationIter)
				{
					if (strcmp(StationIter->shortName, resStationIter->shortName) == 0)
					{
						sTemp.Format(L"Except station %s\n", CString(StationIter->shortName).GetString());
						if (m_bNeedOutputDebug)
							OutputDebugStringW(sTemp);
						StationList.erase(StationIter);
						break;
					}
				}
				simplex.SetStationList(StationList);
				simplex.SetZeroTime(TimeT0);
				simplex.SetDebugOutput(m_bNeedOutputDebug);
				res = simplex.SimplexMethod(hyppocenter);
				Hypos.push_back(hyppocenter);
			}
		}
	}
	
	list<CSimplexPoint>::iterator HyposIter;
	for (HyposIter = Hypos.begin(); HyposIter != Hypos.end(); ++HyposIter)
	{
		if (HyposIter->r < hyppocenter.r)
			hyppocenter = *HyposIter;
	}

	DWORD dt2 = GetTickCount();
	sTemp.Format(L"Work time %u ms\n", dt2 - dt1);
	if (m_bNeedOutputDebug)
		OutputDebugStringW(sTemp);

	//DATE ts = TimeT0 + hyppocenter.p.t * M_SecAsPartOfDay;
	ztSetStructureTimeUTC(&m_result.zet_header, hyppocenter.p.t);

	m_result.hypoLatitude = hyppocenter.p.x;
	m_result.hypoLongitude = hyppocenter.p.y;
	m_result.hypoDepth = hyppocenter.p.z / 1000.;
	r = hyppocenter.r;

	// уточняю магнитуду
	int countMag = 0;
	CPointEarth hypo(m_result.hypoLatitude, m_result.hypoLongitude,	-m_result.hypoDepth * 1000);
	for (auto it = resStationList.begin(), ite = resStationList.end(); it != ite; ++it)
	{
		float Tz = it->mainPeriodZ;
		if (Tz <= 0.f)
			Tz = 1.f;
		CPointEarth ss(it->latitude, it->longitude, it->altitude);
		double D;
		hypo.DistanceToPoint(ss, D);
		D *= 0.001;
		float curMagn = log10(it->peakMotionA * 1000 / Tz) +
			it->magn_A * log10((float)D / (float)M_OneDegreeOfMeridianInkm) +
			it->magn_B + it->magn_C;
		if (curMagn < 0.f)
			curMagn = 0.f;
		else if (curMagn > 9.f)
			curMagn = 9.f;

		CalcAverageOnFly(curMagn, ++countMag, m_result.magnitude);
	}

	ztCloseStructure(&m_result.zet_header);

	return NoError;
}
//------------------------------------------------------------------------------
long CFindHypo::SurplusMethod(
	_In_ list<Ssm_str_EventStation> &resStationList,
	_Out_ double& r)
{// поиск гиппоцентра землетрясения методом перебора
	r = NAN;
	if (m_bNeedOutputDebug)
		OutputDebugStringW(L"SKSV: FindHypo start SURPLUS\n");

	DATE TimeT0 = 10e+9;
	auto itf = resStationList.begin();
	for (auto it = resStationList.begin(), ite = resStationList.end(); it != ite; ++it)
	{
		DATE TimeTemp(0.);
		if (0 == ztGetStructureTimeUTC(&it->zet_header, &TimeTemp))
		{
			if (TimeT0 > TimeTemp)
				TimeT0 = TimeTemp;
		}
	}

	TimeT0 -= 400. / M_NumberSecInDay;

	CString sTemp;
	CSurplus surplus;
	CSimplexPoint hyppocenter;
	list<CSimplexPoint> Hypos;

	bool res;
	DWORD dt1 = GetTickCount();

	sTemp.Format(L"All stations\n");
	if (m_bNeedOutputDebug)
		OutputDebugStringW(sTemp);
	surplus.SetStationList(resStationList);
	surplus.SetZeroTime(TimeT0);
	surplus.SetDebugOutput(m_bNeedOutputDebug);
	res = surplus.SurplusMethod(hyppocenter);
	Hypos.push_back(hyppocenter);

	if (hyppocenter.r > MAX_RESIDUAL)
	{
		if (resStationList.size() > 3)
		{
			list<Ssm_str_EventStation> StationList;
			list<Ssm_str_EventStation>::iterator resStationIter;
			list<Ssm_str_EventStation>::iterator StationIter;
			for (resStationIter = resStationList.begin(); resStationIter != resStationList.end(); ++resStationIter)
			{
				StationList.assign(resStationList.begin(), resStationList.end());
				for (StationIter = StationList.begin(); StationIter != StationList.end(); ++StationIter)
				{
					if (strcmp(StationIter->shortName, resStationIter->shortName) == 0)
					{
						sTemp.Format(L"Except station %s\n", CString(StationIter->shortName).GetString());
						if (m_bNeedOutputDebug)
							OutputDebugStringW(sTemp);
						StationList.erase(StationIter);
						break;
					}
				}
				surplus.SetStationList(StationList);
				surplus.SetZeroTime(TimeT0);
				surplus.SetDebugOutput(m_bNeedOutputDebug);
				res = surplus.SurplusMethod(hyppocenter);
				Hypos.push_back(hyppocenter);
			}
		}
	}

	list<CSimplexPoint>::iterator HyposIter;
	for (HyposIter = Hypos.begin(); HyposIter != Hypos.end(); ++HyposIter)
	{
		if (HyposIter->r < hyppocenter.r)
			hyppocenter = *HyposIter;
	}

	DWORD dt2 = GetTickCount();
	sTemp.Format(L"Work time %u ms\n", dt2 - dt1);
	if (m_bNeedOutputDebug)
		OutputDebugStringW(sTemp);

	//DATE ts = TimeT0 + hyppocenter.p.t * M_SecAsPartOfDay;
	ztSetStructureTimeUTC(&m_result.zet_header, hyppocenter.p.t);

	m_result.hypoLatitude = hyppocenter.p.x;
	m_result.hypoLongitude = hyppocenter.p.y;
	m_result.hypoDepth = hyppocenter.p.z / 1000.;
	r = hyppocenter.r;

	// уточняю магнитуду
	int countMag = 0;
	CPointEarth hypo(m_result.hypoLatitude, m_result.hypoLongitude, -m_result.hypoDepth * 1000);
	for (auto it = resStationList.begin(), ite = resStationList.end(); it != ite; ++it)
	{
		float Tz = it->mainPeriodZ;
		if (Tz <= 0.f)
			Tz = 1.f;
		CPointEarth ss(it->latitude, it->longitude, it->altitude);
		double D;
		hypo.DistanceToPoint(ss, D);
		D *= 0.001;
		float curMagn = log10(it->peakMotionA * 1000 / Tz) +
			it->magn_A * log10((float)D / (float)M_OneDegreeOfMeridianInkm) +
			it->magn_B + it->magn_C;
		if (curMagn < 0.f)
			curMagn = 0.f;
		else if (curMagn > 9.f)
			curMagn = 9.f;

		CalcAverageOnFly(curMagn, ++countMag, m_result.magnitude);
	}

	ztCloseStructure(&m_result.zet_header);

	return NoError;
}
//------------------------------------------------------------------------------
// Результат работы функция заносит в структуру результатов своего класса
long CFindHypo::FindHypo
(_In_ list<Ssm_str_EventStation> &resStationList)
{
	double rSurplus(NAN);
	long errSurplus = SurplusMethod(resStationList, rSurplus);
	if (NoError == errSurplus && 0 != _finite(rSurplus) && MAX_RESIDUAL > rSurplus)
		return errSurplus;

	Ssm_res_Algr_FindHypo resSurplus = m_result;

	double rSimplex(NAN);
	long errSimplex = SimplexMethod(resStationList, rSimplex);
	if (NoError == errSimplex && 0 != _finite(rSimplex) && MAX_RESIDUAL > rSimplex)
		return errSimplex;

	Ssm_res_Algr_FindHypo resSimplex = m_result;

	long err(NoError);
	if (NoError == errSurplus && NoError == errSimplex)
	{
		if (0 != _finite(rSurplus) && 0 != _finite(rSimplex))
			m_result = rSurplus < rSimplex ? resSurplus : resSimplex;
		else if (0 != _finite(rSurplus))
			m_result = resSurplus;
		else if (0 != _finite(rSimplex))
			m_result = resSimplex;
		else
			err = Err_Calc;
	}
	else if (NoError == errSurplus)
		m_result = resSurplus;
	else if (NoError == errSimplex)
		m_result = resSimplex;
	else
		err = Err_Calc;

	return err;
}
//------------------------------------------------------------------------------
}//namespace seismo
}//namespace dsp
}//namespace algorithm
}//namespace zet
//------------------------------------------------------------------------------