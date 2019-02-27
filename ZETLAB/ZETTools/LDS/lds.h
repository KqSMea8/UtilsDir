//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <intsafe.h>
#include "mathZet.h"
#include "Metrology\SpaceProbability.h"
#include "Algorithms\SomeFeatures.h"
//------------------------------------------------------------------------------
#define SizeForTraining 300
//------------------------------------------------------------------------------
// �������������� ��������, ������������ � ��� (��� BYTE)
#define Lds_ID_Sensor_Press	1		// ������ �������� (����������� �)
#define Lds_ID_Sensor_Hydro	2		// �������� (������������ �)
//------------------------------------------------------------------------------
// ����� ����� ���������� ��� - ������� ����� � Lds_Type_Algr_All
#define Lds_Type_Algr_Prof		1	// ������ ������� ��������
#define Lds_Type_Algr_Wave		2	// �� ����� ��������
#define Lds_Type_Algr_Acst		4	// �� ������������ ��������

#define Lds_Num_Type_Algr		3	// ���-�� ����� ����������
//------------------------------------------------------------------------------
// ����� ���-���������� ��� - ������� ����� � Lds_Type_Algr_All
// ������� ----------------------
#define Lds_Algr_Prof_6			1	// ������� �������� �� 6 ���
#define Lds_Algr_Prof_12		2	// ������� �������� �� 12 ���
#define Lds_Algr_Prof_18		4	// ������� �������� �� 18 ���
#define Lds_Algr_Prof_24		8	// ������� �������� �� 24 ���
#define Lds_Algr_Prof_30		16	// ������� �������� �� 30 ���

#define Lds_Num_Type_SubAlgr_Prof	5	// ���-�� ���-���������� Prof
// ����� ------------------------
#define Lds_Algr_Wave_DWF		1	// ����� �� �������� �������-����������

#define Lds_Num_Type_SubAlgr_Wave	1	// ���-�� ���-���������� Wave
// �������� ---------------------
#define Lds_Algr_Acst			1	// ��������

#define Lds_Num_Type_SubAlgr_Acst	1	// ���-�� ���-���������� Acst
//------------------------------------------------------------------------------
// ������� ����������
#define Lds_ind_Algr_Prof		0	// ������� ��������
#define Lds_ind_Algr_Wave		1	// ����� ��������
#define Lds_ind_Algr_Acst		2	// ��������

// ������� ���-����������
// ������� --------------------------
#define Lds_ind_SubAlgr_Prof_6		0
#define Lds_ind_SubAlgr_Prof_12		1
#define Lds_ind_SubAlgr_Prof_18		2
#define Lds_ind_SubAlgr_Prof_24		3
#define Lds_ind_SubAlgr_Prof_30		4
// ����� ----------------------------
#define Lds_ind_SubAlgr_Wave_DWF	0
// �������� -------------------------
#define Lds_ind_SubAlgr_Acst		0
//------------------------------------------------------------------------------
enum Lds_type_Status_Event
{// ����������� ������ �������
	lds_tse_no,			// ������� �� ����������
	lds_tse_start,		// ������� ��������
	lds_tse_enable,		// ������� �����������
	lds_tse_end,		// ������� �����������
};
//------------------------------------------------------------------------------
enum Lds_type_Stationary
{
	lds_ts_No = -1,		// �������������� ���
	lds_ts_Unknown,		// ��� �������������� �� ���������
	lds_ts_Yes,			// �������������� ����
};
//------------------------------------------------------------------------------
enum Lds_type_Event
{// ��� ������� ��, ������������ ����� �������
	lds_te_False = -1,	// ������ �������
	lds_te_No,			// ��� �������
	lds_te_Press_Up,	// ������� � ����������� ��������
	lds_te_Press_Douw,	// ������� � ����������� �������� 
	lds_te_DetectEvent,	// �������� �������
	lds_te_Leak,		// ������
	lds_te_Press_Up_SRMC,	// ������� � ���������� �� ����
	lds_te_Press_Douw_SRMC,	// ������� � ���������� �� ����
	lds_te_Yes,			// �����-�� �������, ������������� ����� TimeYellow
	lds_te_Yes_SRMC,	// ������� � �����-�� ���������� �� ����
	lds_te_PumpStart_SRMC,	// ������ ���������
	lds_te_PumpStop_SRMC,	// ����� ���������
};
//------------------------------------------------------------------------------
enum Lds_type_ReStart : WORD
{// ���� ������� ReStart
	lds_trs_No,				// ������ �� ����
	lds_trs_ZeroCounter,	// ������ ��������� ��������
	lds_trs_NewThresholds,	// ����������� ������ ����� �������
	lds_trs_DeferNewThresh,	// ���������� ������ ����� �������
	lds_trs_Start_Hot,		// ������� ����������
	lds_trs_PumpStop,		// ��������� �������
	lds_trs_PumpStart,		// ������ �������
};
//------------------------------------------------------------------------------
enum Lds_type_event_SRMC
{// ���� ��������� � ����� ������ ������
	lds_SRMC_No,		// ��� ���������
	lds_SRMC_Start,		// ������ ���������
	lds_SRMC_Stop,		// ���������� ���������
	lds_SRMC_Up_Productive,		// ���������� ������������������ ���������
	lds_SRMC_Down_Productive,	// ���������� ������������������ ���������
};
//------------------------------------------------------------------------------
struct Lds_str_AlgrAll
{// ���������-��������� ������������ ����������. sizeof() = 4
	WORD	AlgrMain;	// ������� �����
	WORD	AlgrSub;	// ������� �����

	Lds_str_AlgrAll()
	{
		*this = 0;
	}
	~Lds_str_AlgrAll() {}

	Lds_str_AlgrAll(WORD algr_main, WORD algr_sub)
	{
		AlgrMain = algr_main;
		AlgrSub = algr_sub;
	}
	Lds_str_AlgrAll(const Lds_str_AlgrAll &val)
	{
		*this = val;
	}
	void operator = (const Lds_str_AlgrAll &val)
	{
		AlgrMain = val.AlgrMain;
		AlgrSub = val.AlgrSub;
	}
	void operator = (const DWORD &val)
	{
		*((DWORD*)this) = val;
	}
	operator DWORD ()
	{
		return *((DWORD*)this);
	}
};
//------------------------------------------------------------------------------
struct Lds_str_ObjectID
{//// ��������� � ����������������. sizeof() = 4
	// ���� �����-���� ID = 0, �� ��� ������, ��� �� �� �����
	BYTE sensor;		// ������� ����, ID �������
	BYTE type;			// ������� ����, ID ���� �������
	BYTE line;			// ������� ����, ID �������� ����� ������������
	BYTE pipe;			// ������� ����, ID ������������

	Lds_str_ObjectID()
	{
		*this = 0;
	}
	~Lds_str_ObjectID() {}

	Lds_str_ObjectID(BYTE _sensor, BYTE _type, BYTE _line, BYTE _pipe)
	{
		sensor = _sensor;
		type = _type;
		line = _line;
		pipe = _pipe;
	}
	Lds_str_ObjectID(const Lds_str_ObjectID &val)
	{
		*this = val;
	}
	operator DWORD ()
	{
		return *((DWORD*)this);
	}
	void operator = (const Lds_str_ObjectID &val)
	{
		sensor = val.sensor;
		type = val.type;
		line = val.line;
		pipe = val.pipe;
	}
	void operator = (const DWORD &val)
	{
		*((DWORD*)this) = val;
	}
};
//------------------------------------------------------------------------------
struct Lds_str_EventData
{
	float	value;		// ��������� ��������
	double	Time;		// ����� ����������� �������

	Lds_str_EventData()
		: value(0.f)
		, Time(0.)
	{}
	~Lds_str_EventData() {}

	Lds_str_EventData(const Lds_str_EventData &val)
	{
		*this = val;
	}
	void operator = (const Lds_str_EventData &val)
	{
		value = val.value;
		Time = val.Time;
	}
	void Clear()
	{
		value = 0.f;
		Time = 0.;
	}
};
//------------------------------------------------------------------------------
struct Lds_str_CustomData
{// ���������-��������� ������
	long	typeValue;	// -1 ���� value < 0, � +1 ���� value > 0
	float	average;	// ������� ��������
	float	valuePeak;	// ������� �������� ������� (dP ��� �������)
	float	intensity;	// �������� ������� (������������� � �^3/���)
	UINT	counter;	// ������� ��� ������� ��������
	Lds_type_Status_Event	status;		// ������ ������
	CSpaceSimple<double>	Times;		// ��������� ��������

	Lds_str_CustomData()
		: typeValue(-1)
		, average(0.f)
		, counter(0)
		, intensity(0.f)
		, status(lds_tse_no)
		, valuePeak(0.f)
		, Times()
	{}
	~Lds_str_CustomData() {}

	Lds_str_CustomData(const Lds_str_CustomData &val)
	{
		*this = val;
	}
	void operator = (const Lds_str_CustomData &val)
	{
		typeValue = val.typeValue;
		average = val.average;
		counter = val.counter;
		valuePeak = val.valuePeak;
		intensity = val.intensity;
		status = val.status;
		Times = val.Times;
	}
	void Add(float newVal)
	{
		if (counter == 0)
			intensity = 0.f;
		CalcAverageOnFly(newVal, ++counter, average);
		if (typeValue < 0)
		{// IsNotEqual - ������
			if ((valuePeak != 0.f) && (newVal != 0.f))
			{
				if ((valuePeak < 0.f) && (newVal < 0.f))
					valuePeak = min(valuePeak, newVal);
				else if ((valuePeak > 0.f) && (newVal > 0.f))
					valuePeak = max(valuePeak, newVal);
				else if ((valuePeak > 0.f) && (newVal < 0.f))
					valuePeak = newVal;
				//	else//((value < 0.f) && (newVal > 0.f))
				//		valuePeak			��� ���������
			}
			else if (valuePeak == 0.f)	// newVal != 0.f
				valuePeak = newVal;
		//	else //newVal == 0.f
		//		valuePeak			��� ���������
		}
		else
			valuePeak = max(valuePeak, newVal);
	}
	void Clear()
	{
		average = 0.f;
		counter = 0;
		valuePeak = 0.f;
		intensity = 0.f;
		status = lds_tse_no;
		Times.Clear();
	}
	bool TestStartWithEvent()
	{// ������� �����������, ���� ����� ��������.
	//	������� � �������� �� �������
		bool ret(false);
		switch (status)
		{
		case lds_tse_no:
		case lds_tse_end:
			status = lds_tse_start;
			ret = true;
			break;

		case lds_tse_start:
			status = lds_tse_enable;
			break;

		//case lds_tse_enable:
		//	status = lds_tse_enable;
		//	break;
		}
		return ret;
	}
	bool TestEndWithOutEvent()
	{// ������� �����������. ������� � �������� �� �������
		bool ret(false);
		switch (status)
		{
		case lds_tse_no:
			break;

		case lds_tse_start:
		case lds_tse_enable:
			status = lds_tse_end;
			ret = true;
			break;

		case lds_tse_end:
			status = lds_tse_no;
		//	break;
		}
		if (ret)
			counter = 0;
		return ret;
	}
};
//------------------------------------------------------------------------------
struct Lds_str_CustomData_Hdr : public Lds_str_CustomData
{
	float	numBandWithEvent;	// ���-�� ����� � ��������
	float	freqMin;			// �in ������� � ����� � ��������
	float	freqMax;			// ��� ������� � ����� � ��������

	Lds_str_CustomData_Hdr() : Lds_str_CustomData()
		, numBandWithEvent(0)
		, freqMin(0.f)
		, freqMax(0.f)
	{
		typeValue = 1;
	}
	Lds_str_CustomData_Hdr(const Lds_str_CustomData_Hdr &val)
		: Lds_str_CustomData(val)
	{
		*this = val;
	}
	void operator = (const Lds_str_CustomData_Hdr &val)
	{
		*((Lds_str_CustomData*)this) = val;
		numBandWithEvent = val.numBandWithEvent;
		freqMin = val.freqMin;
		freqMax = val.freqMax;
	}
	void Clear()
	{
		Lds_str_CustomData::Clear();
		numBandWithEvent = 0;
		freqMin = 0.f;
		freqMax = 0.f;
	}
};
//------------------------------------------------------------------------------
struct Lds_str_CustomLeak
{// ��������� � ������� � ������� �� ������ �������
	// ������������ ��� �������� ����� ������������ ���.
	DWORD	size;		// ������ ��������� � ������
	DWORD	idStr;		// ������������� ���������
	DATE	dataTime;	// ����� ������
	Lds_str_ObjectID	idSens;	// ������������� �������, ������������� ������

	Lds_str_CustomLeak()
		: idStr(0)
		, dataTime(0.)
	{
		size = sizeof(Lds_str_CustomLeak);
	}
	~Lds_str_CustomLeak() {}

	Lds_str_CustomLeak(const Lds_str_CustomLeak &val)
		: idStr(0)
	{
		size = sizeof(Lds_str_CustomLeak);
		*this = val;
	}
	void operator = (const Lds_str_CustomLeak &val)
	{
		dataTime = val.dataTime;
		idSens = val.idSens;
	}
};
//------------------------------------------------------------------------------
struct Lds_str_Leak_Prs : public Lds_str_CustomLeak
{// ��������� � ������� � ������� �� ������� ��������.
	// ������������ ��� �������� ����� ������������ ���.
	Lds_str_CustomData	data;	// ��������� ������

	Lds_str_Leak_Prs()
	{
		size = sizeof(Lds_str_Leak_Prs);
		idStr = 0x10010;
	}
	~Lds_str_Leak_Prs() {}

	Lds_str_Leak_Prs(const Lds_str_Leak_Prs &val)
	{
		size = sizeof(Lds_str_Leak_Prs);
		idStr = 0x10010;
		*this = val;
	}
	void operator = (const Lds_str_Leak_Prs &val)
	{
		*((Lds_str_CustomLeak*)this) = val;
		data = val.data;
	}
	void Clear()
	{
		data.Clear();
	}
};
//------------------------------------------------------------------------------
struct Lds_str_Leak_Hdr : public Lds_str_CustomLeak
{// ��������� � ������� � ������� �� ���������.
	// ������������ ��� �������� ����� ������������ ���.
	Lds_str_CustomData_Hdr	data;	// ��������� ������

	Lds_str_Leak_Hdr()
	{
		size = sizeof(Lds_str_Leak_Hdr);
		idStr = 0x10011;
	}
	~Lds_str_Leak_Hdr() {}

	Lds_str_Leak_Hdr(const Lds_str_Leak_Hdr &val)
	{
		size = sizeof(Lds_str_Leak_Hdr);
		idStr = 0x10011;
		*this = val;
	}
	void operator = (const Lds_str_Leak_Hdr &val)
	{
		*((Lds_str_CustomLeak*)this) = val;
		data = val.data;
	}
	void Clear()
	{
		data.Clear();
	}
};
//------------------------------------------------------------------------------
struct Lds_str_Fault
{// ���������, ���������� � �������������. sizeof() = 12
	// ������������ ��� �������� ����� ������������ ���.
	DWORD	size;		// ������ ��������� � ������
	DWORD	idStr;		// ������������� ���������
	DATE	dataTime;	// ����� ������
	Lds_str_ObjectID	idSens;	// ������������� �������
	Type_Fault			type;	// ��� �������������

	Lds_str_Fault()
		: idStr(0x10012)
		, dataTime(0.)
	{
		size = sizeof(Lds_str_Fault);
		type = fault_Ok;
	//	idSens = 0;			��. ����������� Lds_ObjectID
	}
	~Lds_str_Fault() {}

	Lds_str_Fault(const Lds_str_Fault &val)
		: idStr(0x10012)
	{
		size = sizeof(Lds_str_Fault);
		*this = val;
	}
	void operator = (const Lds_str_Fault &val)
	{
		dataTime = val.dataTime;
		idSens = val.idSens;
		type = val.type;
	}
};
//------------------------------------------------------------------------------
struct Lds_str_Stationary
{// ���������-��������� ��������������.
	// ������������ ��� �������� ����� ������������ ���.
	DWORD	size;		// ������ ��������� � ������
	DWORD	idStr;		// ������������� ���������
	DATE	dataTime;	// ����� ������
	Lds_str_ObjectID	idSens;	// ������������� �������
	Lds_type_Stationary	type;	// ��� ��������������

	Lds_str_Stationary()
		: idStr(0x10013)
		, dataTime(0.)
	{
		size = sizeof(Lds_str_Stationary);
	//	idSens = 0;			��. ����������� Lds_ObjectID
		type = lds_ts_Unknown;
	}
	~Lds_str_Stationary() {}

	Lds_str_Stationary(const Lds_str_Stationary &val)
		: idStr(0x10013)
	{
		size = sizeof(Lds_str_Stationary);
		*this = val;
	}
	void operator = (const Lds_str_Stationary &val)
	{
		dataTime = val.dataTime;
		idSens = val.idSens;
		type = val.type;
	}
};
//------------------------------------------------------------------------------
struct Lds_str_BadParam
{// ��������� c ������� ��������� �������� ��� �������� �/� ������������
	// ������������ ��� �������� ����� ������������ ���.
	DWORD	size;		// ������ ��������� � ������
	DWORD	idStr;		// ������������� ���������
	DATE	dataTime;	// ����� ������
	Lds_str_ObjectID	idSens;	// ������������� �������
	DWORD				value;	// ���� �������� - 0 ��� 1

	Lds_str_BadParam()
		: idStr(0x10014)
		, dataTime(0.)
	{
		size = sizeof(Lds_str_BadParam);
	//	idSens = 0;			��. ����������� Lds_ObjectID
		value = DWORD_MAX;		// � ������ ������ ���������
	}
	~Lds_str_BadParam() {}

	Lds_str_BadParam(const Lds_str_BadParam &val)
		: idStr(0x10014)
	{
		size = sizeof(Lds_str_BadParam);
		*this = val;
	}
	void operator = (const Lds_str_BadParam &val)
	{
		dataTime = val.dataTime;
		idSens = val.idSens;
		value = val.value;
	}
};
//------------------------------------------------------------------------------
struct Lds_str_ReStart
{
	Lds_type_ReStart	command;	// ������� - ������� ����� � 32 �����
	BYTE				type;		// ��� �������
	BYTE				id8;		// ��� �������������
	
	Lds_str_ReStart()
	{
		*this = 0;
	}
	~Lds_str_ReStart() {}
	Lds_str_ReStart(const Lds_str_ReStart &val)
	{
		*this = val;
	}
	Lds_str_ReStart(const long val)
	{
		*this = val;
	}
	void operator = (const Lds_str_ReStart &val)
	{
		*this = val;
	}
	operator long() { return *((long*)this); }
	void operator = (const long a)  { *((long*)this) = a; };
};
//------------------------------------------------------------------------------
