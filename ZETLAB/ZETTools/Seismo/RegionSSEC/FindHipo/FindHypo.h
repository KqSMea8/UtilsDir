//------------------------------------------------------------------------------
//	Файл FindHypo.h
//------------------------------------------------------------------------------
/*	Класс для реализации алгоритма поиска гипоцентра землетрясения.
*/
//------------------------------------------------------------------------------
#pragma once
#include <Seismo\CustomAlgorithm\CustomAlgorithm.h>
#include <Seismo\StationSSEC\Struct_EventStation\Ssm_str_EventStation.zschema1.h>
#include <Seismo\RegionSSEC\FindHipo\Struct_CFG\Ssm_cfg_Algr_FindHypoProxy.h>
#include <Seismo\RegionSSEC\FindHipo\Struct_Dia\Ssm_dia_Algr_FyndHypo.zschema1.h>
#include <Seismo\RegionSSEC\FindHipo\Struct_Res\Ssm_res_Algr_FyndHypo.zschema1.h>
#include <FunctionSeismic\PointEarth.h>
#include <Metrology\Rectangle.h>
#include <Metrology\Space.h>
#include <list>
#include <mathZet.h>
#include <FunctionSeismic\Simplex.h>
#include <FunctionSeismic\Surplus.h>
//------------------------------------------------------------------------------
//#undef max
//------------------------------------------------------------------------------
namespace zet {
namespace algorithm {
namespace dsp {
namespace seismo {
using namespace std;
//------------------------------------------------------------------------------
class CFindHypo : public CCustomAlgorithm
{
private:
	Ssm_cfg_Algr_FindHypo m_config;
	Ssm_res_Algr_FindHypo m_result;

	void GetRectangleOneStation(
		Ssm_str_EventStation &resStation,
		DATE meanT0,
		CRectangle<double> &rect, 
		CSpace<double> &t0);

	//void GetRectangle3DOneStation(
	//	Ssm_str_res_Station &resStation,
	//	CRectangle3D<double> &rect);

	double CalcResidual(
		_In_ list<Ssm_str_EventStation> &resStationList,
		_In_ DATE meanT0,
		_In_ CPointEarth &point,
		_In_ double time);

	void FindEpicenterAndT0(list<Ssm_str_EventStation> &resStationList,
		DATE meanT0,
		CRectangle<double> &region, CSpace<double> &t0,
		double &epicenterLat, double &epicenterLon,
		double &residual);

	void FindDepth(list<Ssm_str_EventStation> &resStationList,
		DATE meanT0,
		const double epicenterLat, const double epicenterLon, const double t0,
		double &depth, double &residual);

	//DATE FindOptimalTime(
	//	_In_ list<Ssm_str_res_Station> &resStationList, 
	//	_In_ CPointEarth &point);

	long SimplexMethod(
		_In_ list<Ssm_str_EventStation> &resStationList,
		_Out_ double& r);

	long SurplusMethod(
		_In_ list<Ssm_str_EventStation> &resStationList,
		_Out_ double& r);

public:
	CFindHypo();
	CFindHypo(const CFindHypo &val);
	~CFindHypo();

	bool m_bNeedOutputDebug = false;
	Ssm_cfg_Algr_FindHypo_ns::Proxy m_cfgClass;

	void operator = (const CFindHypo &val);

	virtual long SetConfig(ZetHeader* pParam);
	long SetConfigFromProxy();

	long FindHypo(_In_ list<Ssm_str_EventStation> &resStationList);

};
//------------------------------------------------------------------------------
}//namespace seismo
}//namespace dsp
}//namespace algorithm
}//namespace zet
//------------------------------------------------------------------------------