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
//	Prs - от Pressure, т.е. речь о датчиках давления
//	Hdr - от Hydrophone, т.е. речь о гидрофонах
//------------------------------------------------------------------------------
struct Lds_cfg_Const_TimeDetectOne
{// временные константы обнаружения утечки
	DWORD	timeDetect;		// время обнаружения, сек
	float	senst_m3_h;		// чувствительность в м3/час
	float	senst_pers;		// чувствительность в % от макс. расхода

	Lds_cfg_Const_TimeDetectOne()
		: timeDetect(0)
		, senst_m3_h(0.f)
		, senst_pers(0.f)
	{}
};
//------------------------------------------------------------------------------
struct Lds_cfg_Const_TimeDetect
{// временные константы обнаружения утечки в нестационарном режиме (ОТТ п.6.4)
	const BYTE numConst;	// кол-во чувствительностей обнаружения
	std::vector<Lds_cfg_Const_TimeDetectOne> ctd;

	Lds_cfg_Const_TimeDetect()
		: numConst(5)
	{
		ctd.resize(numConst);
		ctd[0].timeDetect = 300;	// 5 мин	=  6 - 1
		ctd[1].timeDetect = 600;	// 10 мин	= 12 - 2
		ctd[2].timeDetect = 900;	// 15 мин	= 18 - 3
		ctd[3].timeDetect = 1200;	// 20 мин	= 24 - 4
		ctd[4].timeDetect = 1500;	// 25 мин	= 30 - 5
	}
	~Lds_cfg_Const_TimeDetect()
	{
		ctd.clear();
	}
	void SetSensitivity(const float transMax)
	{// transMax - максимальный расход, задан в ТЗ
		for (auto it = ctd.begin(), ie = ctd.end(); it != ie; ++it)
		{
			it->senst_m3_h = it->senst_pers * transMax * 0.01f;
		}
	}
	double GetTimeDetect(const float intens_m3_h)
	{// время срабатывания СОУ по определённой интенсивности
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
{// временные константы обнаружения утечки в стационарном режиме (ОТТ п.6.4)
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
{// временные константы обнаружения утечки в нестационарном режиме (ОТТ п.6.4)
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
{// константы линейной части
	bool	bSoundSpeed_Table;	// скорость звука из таблиц (или измеренная)
	DWORD	timeBadParamPumpStart_minute;// длительность акустических переходных
								// процессов при запуске перекачки, мин
	float	soundSpeed_val;		// величина скорости звука, км/с
	float	soundSpeed_unc;		// неопределенность скорости звука, км/с
	float	maxPressure;		// максимальное давление, МПа
	float	maxTrans;			// максимальный расход, м3/час
	float	densityOil;			// плотность нефти, кг/м3
	float	innerDiameter;		// внутренний диаметр трубы, м
	float	minDeltaFreqPump;	// пороговое изменение частоты насоса, Гц
	float	pressureDetectStart;// давление для начала детектирования, МПа
	float	pressureDetectStop;	// давление для завершения детектирования, МПа

	Lds_cfg_Const_Line()
		: bSoundSpeed_Table(true)	// из таблиц
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
//		Параметры алгоритмов детектирования событий (_det_)
//------------------------------------------------------------------------------
struct Lds_cfg_Algr_det_FDWT
{// Параметры алгоритма, использующего быстрое цифровое вейвлет-преобразование
	bool bOnOff;				// алгоритм используется
	TypeWavelet type;			// тип материнского вейвлета
	int level;					// уровень преобразования
	float alfa;					// коэффициент альфа
	float minNumBand_Percent;	// мин. сработавших кол-во полос для события
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
{// Параметры алгоритма, использующего быстрое фурье-преобразование
	bool bOnOff;				// алгоритм используется
	TypeWeightFunction type;	// тип оконной весовой функции
	int order;					// порядок преобразования
	float alfa;					// коэффициент альфа
	float minNumBand_Percent;	// мин. сработавших кол-во полос для события

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
{// Параметры алгоритма, использующего долеоктавный частотный анализ
	bool bOnOff;				// алгоритм используется
	int base;					// основание
	float minFreq;				// минимальная частота
	float alfa;					// коэффициент альфа
	float minNumBand_Percent;	// мин. сработавших кол-во полос для события

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
{// Параметры алгоритма по давлению
	bool	bOnOff;				// алгоритм используется
	float	freqFiltr;			// частота среза ФНЧ, Гц
	float	deltaPressure;		// порог значимости изменения давления, МПа
	float	factorCalibrLine;	// наклон калибровочной кривой, (м3/час)/МПа

	Lds_cfg_Algr_det_Press()
		: bOnOff(true)
		, freqFiltr(5.f)
		, deltaPressure(0.f)
		, factorCalibrLine(-1300.f)
	{}
};
//------------------------------------------------------------------------------
//		Алгоритмы классификации событий (_cls_)
//------------------------------------------------------------------------------
struct Lds_cfg_Algr_cls_Prof
{// Параметры алгоритма гидроуклон
	bool	bOnOff;				// алгоритм используется

	Lds_cfg_Algr_cls_Prof()
		: bOnOff(true)
	{}
};
//------------------------------------------------------------------------------
struct Lds_cfg_Algr_cls_Wave
{// Параметры алгоритма по волне
	bool	bOnOff;		// алгоритм используется

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
{// Параметры алгоритма временного фильтра (используется всегда!!!)
	double	lifeTime;			// время жизни данных (30 мин)
	double	maxPause;			// максимальная пауза внутри утечки (10 сек)
	double	minDuration;		// минимальная длительность утечки (1 мин)
	double	timerResultFast;	// интервал быстрого таймера (10 сек)
	double	timerResultSlow;	// интервал медленного таймера (1 мин)
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
{// Параметры одного гидрофона
	bool				bOnOff;			// гидрофон используется
	Lds_cfg_Algr_det_FDWT	parFDWT;	// параметры FDWT
	Lds_cfg_Algr_det_FFT	parFFT;		// параметры FFT
	Lds_cfg_Algr_det_FOFT	parFOFT;	// параметры FOFT

	Lds_cfg_Param_Hdr()
		: bOnOff(true)
	//	, parFDWT()
	//	, parFFT()
	//	, parFOFT()
	{}
};
//------------------------------------------------------------------------------
struct Lds_cfg_Param_Prs
{// Параметры одного ДД
	bool				bOnOff;			// ДД используется
	Lds_cfg_Algr_det_Press	parPres;	// параметры Prof

	Lds_cfg_Param_Prs()
		: bOnOff(true)
	//	, parPres()
	{}
};
//------------------------------------------------------------------------------
struct Lds_cfg_Param_SS
{// параметры одной датчиковой системы (Sensors System)
	bool				bOnOff;		// ДС используется
	BYTE				ID;			// идентификатор ДС в линейной части (от 1)
	float				mileage;	// координата, км
	CString				nameSRV;	// корень имени каналов SRV
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
{// параметры одной линейной части (ЛЧ) нефтепровода
	bool				bOnOff;		// ЛЧ используется (насосы работают)
	bool				bDetect;	// требуется детектитрование утечек (Р > 0)
	BYTE				ID;			// идентификатор ЛЧ в нефтепроводе (от 1)
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
	{// задание максимального расхода
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
{// параметры трубопровода из одной ЛЧ
	bool				bOnOff;		// используется
	BYTE				ID;			// идентификатор трубопровода (от 1)
	Lds_cfg_Param_Line	line;

	Lds_cfg_Param_Pipe_One_Line()
		: bOnOff(true)
		, ID(1)
	//	, line()
	{}
};
//------------------------------------------------------------------------------
//struct Lds_cfg_Param_Pipe_All
//{// параметры нефтепровода
//	bool				bOnOff;		// используется
//	BYTE				ID;			// идентификатор нефтепровода (от 1)
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
	bool m_bInit;					// xml-файл прочитан успешно
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
	CString m_sFullFileNameXML;		// имя конфигурационного файла с папкой

	// времена детектирования
	Lds_cfg_ConstStat_TimeDetect m_timeConst_Stat;
	Lds_cfg_ConstNotStat_TimeDetect m_timeConst_NotStat;

	// все параметры СОУ линейной части нефтепровода
	Lds_cfg_Param_Pipe_One_Line m_parAll;
	
	// функции свойств
	bool IsInit() { return m_bInit; }	

	// функции ---------------------------------------------------
	// чтение конфигурационных данных и задание времён детектирования
	bool ReadCFG(CString fileNameXML = L"");
	
	// запись скорости звука в XML
	bool WriteSoundSpeed(
		const float mean, const float uncertainty, bool bTable);

	// получить номер канала SRV
	bool GetChanSRV(CDSRV *psrv, BYTE idSens, bool bPrs, bool bChanSupply,
		long &chanSRV);

	// получить имя канала SRV
	bool GetChanName(BYTE idSens, bool bPrs, bool bChanSupply,
		CString &name);
};
//------------------------------------------------------------------------------