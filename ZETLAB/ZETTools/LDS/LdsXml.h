//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
#include "afx.h"
#include <vector>
#include <pugixml-1.4\src\pugixml.hpp>
#include <ZETView_basic_class\CDSRV.h>
#include <ippFDWT\CustomFDWT.h>
#include <ippFFT\RealFFT.h>
//------------------------------------------------------------------------------
//	Prs - �� Pressure, �.�. ���� � �������� ��������
//	Hdr - �� Hydrophone, �.�. ���� � ����������
//------------------------------------------------------------------------------
struct Lds_cfg_Const_TimeDetectOne
{// ��������� ��������� ����������� ������
	DWORD	timeDetect;		// ����� �����������, ���
	float	senst_m3_h;		// ���������������� � �3/���
	float	senst_pers;		// ���������������� � % �� ����. �������

	Lds_cfg_Const_TimeDetectOne()
		: timeDetect(0)
		, senst_m3_h(0.f)
		, senst_pers(0.f)
	{}
};
//------------------------------------------------------------------------------
struct Lds_cfg_Const_TimeDetect
{// ��������� ��������� ����������� ������ � �������������� ������ (��� �.6.4)
	const BYTE numConst;	// ���-�� ����������������� �����������
	std::vector<Lds_cfg_Const_TimeDetectOne> ctd;

	Lds_cfg_Const_TimeDetect()
		: numConst(5)
	{
		ctd.resize(numConst);
		ctd[0].timeDetect = 300;	// 5 ���	=  6 - 1
		ctd[1].timeDetect = 600;	// 10 ���	= 12 - 2
		ctd[2].timeDetect = 900;	// 15 ���	= 18 - 3
		ctd[3].timeDetect = 1200;	// 20 ���	= 24 - 4
		ctd[4].timeDetect = 1500;	// 25 ���	= 30 - 5
	}
	~Lds_cfg_Const_TimeDetect()
	{
		ctd.clear();
	}
	void SetSensitivity(const float transMax)
	{// transMax - ������������ ������, ����� � ��
		for (auto it = ctd.begin(), ie = ctd.end(); it != ie; ++it)
		{
			it->senst_m3_h = it->senst_pers * transMax * 0.01f;
		}
	}
	double GetTimeDetect(const float intens_m3_h)
	{// ����� ������������ ��� �� ����������� �������������
		double ret(0.);
		for (auto it = ctd.begin(), ie = ctd.end(); it != ie; ++it)
		{
			if (intens_m3_h > it->senst_m3_h)
			{
				ret = it->timeDetect;
				break;
			}
		}
		return ret;
	}
};
//------------------------------------------------------------------------------
struct Lds_cfg_ConstStat_TimeDetect : public Lds_cfg_Const_TimeDetect
{// ��������� ��������� ����������� ������ � ������������ ������ (��� �.6.4)
	Lds_cfg_ConstStat_TimeDetect()
	{
		ctd[0].senst_pers = 9.f;
		ctd[1].senst_pers = 7.2f;
		ctd[2].senst_pers = 5.4f;
		ctd[3].senst_pers = 3.6f;
		ctd[4].senst_pers = 1.8f;
	}
};
//------------------------------------------------------------------------------
struct Lds_cfg_ConstNotStat_TimeDetect : public Lds_cfg_Const_TimeDetect
{// ��������� ��������� ����������� ������ � �������������� ������ (��� �.6.4)
	Lds_cfg_ConstNotStat_TimeDetect()
	{
		ctd[0].senst_pers = 20.f;
		ctd[1].senst_pers = 20.f;
		ctd[2].senst_pers = 15.f;
		ctd[3].senst_pers = 10.f;
		ctd[4].senst_pers = 6.f;
	}
};
//------------------------------------------------------------------------------
struct Lds_cfg_Const_Line
{// ��������� �������� �����
	bool	bSoundSpeed_Table;	// �������� ����� �� ������ (��� ����������)
	DWORD	timeBadParamPumpStart_minute;// ������������ ������������ ����������
								// ��������� ��� ������� ���������, ���
	float	soundSpeed_val;		// �������� �������� �����, ��/�
	float	soundSpeed_unc;		// ���������������� �������� �����, ��/�
	float	maxPressure;		// ������������ ��������, ���
	float	maxTrans;			// ������������ ������, �3/���
	float	densityOil;			// ��������� �����, ��/�3
	float	innerDiameter;		// ���������� ������� �����, �
	float	minDeltaFreqPump;	// ��������� ��������� ������� ������, ��
	float	pressureDetectStart;// �������� ��� ������ ��������������, ���
	float	pressureDetectStop;	// �������� ��� ���������� ��������������, ���

	Lds_cfg_Const_Line()
		: bSoundSpeed_Table(true)	// �� ������
		, timeBadParamPumpStart_minute(20)
		, soundSpeed_val(1.05f)
		, soundSpeed_unc(0.15f)
		, maxPressure(4.f)
		, maxTrans(100.f)
		, densityOil(840.2f)
		, innerDiameter(0.15f)
		, minDeltaFreqPump(0.05f)
		, pressureDetectStart(1.f)
		, pressureDetectStop(0.f)
	{}
};
//------------------------------------------------------------------------------
//		��������� ���������� �������������� ������� (_det_)
//------------------------------------------------------------------------------
struct Lds_cfg_Algr_det_FDWT
{// ��������� ���������, ������������� ������� �������� �������-��������������
	bool bOnOff;				// �������� ������������
	TypeWavelet type;			// ��� ������������ ��������
	int level;					// ������� ��������������
	float alfa;					// ����������� �����
	float minNumBand_Percent;	// ���. ����������� ���-�� ����� ��� �������
	float factorCalibrLine2;

	Lds_cfg_Algr_det_FDWT()
		: bOnOff(true)
		, type(db4)
		, level(7)
		, alfa(2.0f)
		, minNumBand_Percent(5.f)
		, factorCalibrLine2(0.03f)
	{}
};
//------------------------------------------------------------------------------
struct Lds_cfg_Algr_det_FFT
{// ��������� ���������, ������������� ������� �����-��������������
	bool bOnOff;				// �������� ������������
	TypeWeightFunction type;	// ��� ������� ������� �������
	int order;					// ������� ��������������
	float alfa;					// ����������� �����
	float minNumBand_Percent;	// ���. ����������� ���-�� ����� ��� �������

	Lds_cfg_Algr_det_FFT()
		: bOnOff(false)
		, type(twf_Hamming)
		, order(7)
		, alfa(2.0f)
		, minNumBand_Percent(5.f)
	{}
};
//------------------------------------------------------------------------------
struct Lds_cfg_Algr_det_FOFT
{// ��������� ���������, ������������� ������������ ��������� ������
	bool bOnOff;				// �������� ������������
	int base;					// ���������
	float minFreq;				// ����������� �������
	float alfa;					// ����������� �����
	float minNumBand_Percent;	// ���. ����������� ���-�� ����� ��� �������

	Lds_cfg_Algr_det_FOFT()
		: bOnOff(false)
		, base(12)
		, minFreq(1.0f)
		, alfa(2.0f)
		, minNumBand_Percent(5.f)
	{}
};
//------------------------------------------------------------------------------
struct Lds_cfg_Algr_det_Press
{// ��������� ��������� �� ��������
	bool	bOnOff;				// �������� ������������
	float	freqFiltr;			// ������� ����� ���, ��
	float	deltaPressure;		// ����� ���������� ��������� ��������, ���
	float	factorCalibrLine;	// ������ ������������� ������, (�3/���)/���

	Lds_cfg_Algr_det_Press()
		: bOnOff(true)
		, freqFiltr(5.f)
		, deltaPressure(0.f)
		, factorCalibrLine(-1300.f)
	{}
};
//------------------------------------------------------------------------------
//		��������� ������������� ������� (_cls_)
//------------------------------------------------------------------------------
struct Lds_cfg_Algr_cls_Prof
{// ��������� ��������� ����������
	bool	bOnOff;				// �������� ������������

	Lds_cfg_Algr_cls_Prof()
		: bOnOff(true)
	{}
};
//------------------------------------------------------------------------------
struct Lds_cfg_Algr_cls_Wave
{// ��������� ��������� �� �����
	bool	bOnOff;		// �������� ������������

	Lds_cfg_Algr_cls_Wave()
		: bOnOff(true)
	{}
};
//------------------------------------------------------------------------------
struct Lds_cfg_Algr_cls_Acst
{
	bool	bOnOff;

	Lds_cfg_Algr_cls_Acst()
		: bOnOff(true)
	{}
};
//------------------------------------------------------------------------------
struct Lds_cfg_Algr_cls_TimeFiltr
{// ��������� ��������� ���������� ������� (������������ ������!!!)
	double	lifeTime;			// ����� ����� ������ (30 ���)
	double	maxPause;			// ������������ ����� ������ ������ (10 ���)
	double	minDuration;		// ����������� ������������ ������ (1 ���)
	double	timerResultFast;	// �������� �������� ������� (10 ���)
	double	timerResultSlow;	// �������� ���������� ������� (1 ���)
	double	timeSRMC_before;	// 
	double	timeSRMC_after;		// 
	double	timerDeferNewThresh;

	Lds_cfg_Algr_cls_TimeFiltr()
		: lifeTime(1800)
		, maxPause(10)
		, minDuration(60)
		, timerResultFast(10)
		, timerResultSlow(60)
		, timeSRMC_before(2)
		, timeSRMC_after(100)
		, timerDeferNewThresh(15)
	{}
};
//------------------------------------------------------------------------------
struct Lds_cfg_Param_Hdr
{// ��������� ������ ���������
	bool				bOnOff;			// �������� ������������
	Lds_cfg_Algr_det_FDWT	parFDWT;	// ��������� FDWT
	Lds_cfg_Algr_det_FFT	parFFT;		// ��������� FFT
	Lds_cfg_Algr_det_FOFT	parFOFT;	// ��������� FOFT

	Lds_cfg_Param_Hdr()
		: bOnOff(true)
	//	, parFDWT()
	//	, parFFT()
	//	, parFOFT()
	{}
};
//------------------------------------------------------------------------------
struct Lds_cfg_Param_Prs
{// ��������� ������ ��
	bool				bOnOff;			// �� ������������
	Lds_cfg_Algr_det_Press	parPres;	// ��������� Prof

	Lds_cfg_Param_Prs()
		: bOnOff(true)
	//	, parPres()
	{}
};
//------------------------------------------------------------------------------
struct Lds_cfg_Param_SS
{// ��������� ����� ���������� ������� (Sensors System)
	bool				bOnOff;		// �� ������������
	BYTE				ID;			// ������������� �� � �������� ����� (�� 1)
	float				mileage;	// ����������, ��
	CString				nameSRV;	// ������ ����� ������� SRV
	Lds_cfg_Param_Hdr	parHdr;
	Lds_cfg_Param_Prs	parPrs;

	Lds_cfg_Param_SS()
		: bOnOff(true)
		, ID(1)
		, mileage(0.f)
		, nameSRV()
	//	, parHdr()
	//	, parPrs()
	{}
};
//------------------------------------------------------------------------------
struct Lds_cfg_TopLevel
{
	Lds_cfg_Algr_cls_TimeFiltr	time;
	Lds_cfg_Algr_cls_Acst		acst;
	Lds_cfg_Algr_cls_Prof		prof;
	Lds_cfg_Algr_cls_Wave		wave;
};
//------------------------------------------------------------------------------
struct Lds_cfg_Param_Line
{// ��������� ����� �������� ����� (��) ������������
	bool				bOnOff;		// �� ������������ (������ ��������)
	bool				bDetect;	// ��������� ��������������� ������ (� > 0)
	BYTE				ID;			// ������������� �� � ������������ (�� 1)
	Lds_cfg_Const_Line				constLine;
	Lds_cfg_ConstStat_TimeDetect	constTimeStat;
	Lds_cfg_ConstNotStat_TimeDetect	constTimeNotStat;
	Lds_cfg_TopLevel				topLevel;
	std::vector<Lds_cfg_Param_SS>	vectSS;

	Lds_cfg_Param_Line()
		: bOnOff(true)
		, bDetect(false)
		, ID(1)
	//	, constLineStat()
	//	, constLineNotStat()
	//	, topLevel()
	//	, vectSS()
	{
		constTimeStat.SetSensitivity(constLine.maxTrans);
		constTimeNotStat.SetSensitivity(constLine.maxTrans);
	}
	~Lds_cfg_Param_Line()
	{
		vectSS.clear();
	}
	void SetNumberOfSensorsSystem(DWORD num)
	{
		vectSS.resize(num);
	}
	void SetMaxTrans(float maxTrans)
	{// ������� ������������� �������
		if (maxTrans > 0.f)
		{
			constLine.maxTrans = maxTrans;
			constTimeStat.SetSensitivity(maxTrans);
			constTimeNotStat.SetSensitivity(maxTrans);
		}
	}
};
//------------------------------------------------------------------------------
struct Lds_cfg_Param_Pipe_One_Line
{// ��������� ������������ �� ����� ��
	bool				bOnOff;		// ������������
	BYTE				ID;			// ������������� ������������ (�� 1)
	Lds_cfg_Param_Line	line;

	Lds_cfg_Param_Pipe_One_Line()
		: bOnOff(true)
		, ID(1)
	//	, line()
	{}
};
//------------------------------------------------------------------------------
//struct Lds_cfg_Param_Pipe_All
//{// ��������� ������������
//	bool				bOnOff;		// ������������
//	BYTE				ID;			// ������������� ������������ (�� 1)
//	std::vector<Lds_cfg_Param_Line>	vectLine;
//
//	Lds_cfg_Param_Pipe_All()
//		: bOnOff(true)
//		, ID(1)
//	//	, vectLine()
//	{}
//	~Lds_cfg_Param_Pipe_All()
//	{
//		vectLine.clear();
//	}
//	void SetNumberOfLine(DWORD num)
//	{
//		vectLine.resize(num);
//	}
//};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class CLdsXml
{
private:
	bool m_bInit;					// xml-���� �������� �������
	void ReadLine(pugi::xml_node &hNode, Lds_cfg_Param_Line &line);
	void ReadConstLine(pugi::xml_node &hNode, Lds_cfg_Const_Line &cline);
	void ReadSS(pugi::xml_node &hNode, Lds_cfg_Param_SS &ss);
	void ReadHdr(pugi::xml_node &hNode, Lds_cfg_Param_Hdr &hdr);
	void ReadPrs(pugi::xml_node &hNode, Lds_cfg_Param_Prs &prs);
	void ReadTopLevel(pugi::xml_node &hNode, Lds_cfg_TopLevel &tl);

protected:

public:
	CLdsXml();
	~CLdsXml() {}
	CString m_sFullFileNameXML;		// ��� ����������������� ����� � ������

	// ������� ��������������
	Lds_cfg_ConstStat_TimeDetect m_timeConst_Stat;
	Lds_cfg_ConstNotStat_TimeDetect m_timeConst_NotStat;

	// ��� ��������� ��� �������� ����� ������������
	Lds_cfg_Param_Pipe_One_Line m_parAll;
	
	// ������� �������
	bool IsInit() { return m_bInit; }	

	// ������� ---------------------------------------------------
	// ������ ���������������� ������ � ������� ����� ��������������
	bool ReadCFG(CString fileNameXML = L"");
	
	// ������ �������� ����� � XML
	bool WriteSoundSpeed(
		const float mean, const float uncertainty, bool bTable);

	// �������� ����� ������ SRV
	bool GetChanSRV(CDSRV *psrv, BYTE idSens, bool bPrs, bool bChanSupply,
		long &chanSRV);

	// �������� ��� ������ SRV
	bool GetChanName(BYTE idSens, bool bPrs, bool bChanSupply,
		CString &name);
};
//------------------------------------------------------------------------------