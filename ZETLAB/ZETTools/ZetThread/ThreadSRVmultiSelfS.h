//------------------------------------------------------------------------------
//	���� ThreadSRVmultiSelf.h
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
//		����� ������������ ��� ���������� ������ � �� ��������� � �����������
// ������� �������. �.�. �������, �������� �� ������ ���������,
// � ������� ��������� �������������, ������ ���-��� ������ � �.�.
//		���������, ��� ������ ������� �� ������� ���������� ���������!!!
//		��������� ���������� ������ � ������� ������ ����� ���� �����.
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <limits>
#include <vector>
#include <chaninfo.h>
#include <Flags.h>
#include <Buffer\SmartArray.h>
#include <Intel\ipp\ipps.h>
#include <CodeQuality.h>
#include <Dialog_ZET\DataTime.h>
#include <ZetThread\CustomThreadSRV.h>
//------------------------------------------------------------------------------
#define WhileCountMax		100
#define MaxDeltaTimeInSec	(60.)
//------------------------------------------------------------------------------
#define Flag_Chan_OnOff			0x1		// 1 - ������ � �������
#define Flag_Chan_Exist			0x2		// 2 - ����� ����������
#define Flag_Chan_7000			0x4		// 3 - ����� � ��������� freqADC

#define Flag_Change_Chan		0x8		// 4 - ������ �����
#define Flag_Change_Status		0x10	// 5 - ��������� ������ ������

#define Flag_Change_Freq		0x20	// 6 - ���. �������
#define Flag_Change_Level		0x40	// 7 - ���. ����������� ��������
#define Flag_Change_Level_DC	0x80	// 8 - ���. ����. ������������
#define Flag_Change_Level_dB	0x100	// 9 - ���. ������� ����. ��� dB
#define Flag_Change_Name		0x200	// 10 - ���. ��� ������
#define Flag_Change_Measure		0x400	// 11 - ���. ����������� ������
//------------------------------------------------------------------------------
// ��������� ���������� ������ �������. ��������� �� CHANNEL_STRUCT.
// �������� ����������� ��������� (����� CHANNEL_STRUCT) � ���������,
// ����������� ��� ������ ������ �� �������
//
struct CHANNEL_STRUCT_READ : public CHANNEL_STRUCT
{
	long channel;			// ����� ������
	long decade;			// ������
	long retGetData;		// ��� �������� ��������� ������
	DWORD numKadrMin;		// ����������� ���-�� ����������� ��������
	DWORD numKadrMax;		// ������������ ���-�� ����������� ��������
	Flags<uint32_t> bFlags;	// 32 ����� ������
	SmartArray<float> saArray;// ������ ��� ������ ������
	CDataTime dataTime;		// ����� ������ ������

	CHANNEL_STRUCT_READ() : CHANNEL_STRUCT()
		, channel(-1)
		, decade(0)
		, retGetData(0)
		, numKadrMin(1)
		, numKadrMax(200000)	// = m_psrv->DecadeBufferSize(0);
	{}

	~CHANNEL_STRUCT_READ() {}

	CHANNEL_STRUCT_READ(const CHANNEL_STRUCT_READ & val) :
		CHANNEL_STRUCT(*((CHANNEL_STRUCT*)&val))
	{
		channel = val.channel;
		decade = val.decade;
		retGetData = val.retGetData;
		numKadrMin = val.numKadrMin;
		numKadrMax = val.numKadrMax;
		bFlags = val.bFlags;
		dataTime = val.dataTime;
		saArray = val.saArray;
	}

	CHANNEL_STRUCT_READ & operator = (const CHANNEL_STRUCT_READ & val)
	{
		*((CHANNEL_STRUCT*)this) = *((CHANNEL_STRUCT*)&val);

		channel = val.channel;
		decade = val.decade;
		retGetData = val.retGetData;
		numKadrMin = val.numKadrMin;
		numKadrMax = val.numKadrMax;
		bFlags = val.bFlags;
		dataTime = val.dataTime;
		saArray = val.saArray;

		return *this;
	}

	CHANNEL_STRUCT_READ & operator = (const CHANNEL_STRUCT & val)
	{
		*((CHANNEL_STRUCT*)this) = val;
		return *this;
	}
	
	bool ReSize(DWORD size)
	{
		return saArray.ReSize(size);
	}

	bool GetFlag(uint32_t flag)
	{
		return bFlags.GetFlag(flag);
	}

	void SetFlag(uint32_t flag, bool val)
	{
		bFlags.SetFlag(flag, val);
	}

	void PlaceFlag(uint32_t flag)
	{
		bFlags.PlaceFlag(flag);
	}

	void ResetFlag(uint32_t flag)
	{
		bFlags.ResetFlag(flag);
	}

	// newVal = 0 - ������ �������������� ����� � ������������ � Data.m_dt.
	// ��� ������� ���� ����� �������� � �������������� ������ ������ �����
	// ������� ������ (�.�. ����� ������� ������� ������������� � ������) �
	// ����� ������� ���������� �������� DataTime.
	bool SetNumberCounts(DWORD newVal = 0)
	{
		DWORD num(0);
		if (newVal == 0)
		{
			double x = dataTime.GetCalcNumberMode() ?
				dataTime.GetTimeAverage() : dataTime.GetTimeInterval();
			x *= frequency;
			if (GetFlag(Flag_Chan_7000))
			{// ��������� ������� �������������, ����� ����� ��� ������,
				// ��� ���� ��� ����� �������� ������
				num = (DWORD)(int)floor(x);
				if (x > (double)num)
					++num;
			}
			else
				num = (DWORD)(int)floor(x + 0.5);
		}
		else
		{
			num = newVal;
		}

		// �������� �� ���������� ������� ������
		if (numKadrMax > 0)
		{
			if (num > numKadrMax)
				num = numKadrMax;
		}
		if (numKadrMin > 0)
		{
			if (num < numKadrMin)
				num = numKadrMin;
		}

		return ReSize(num);
	}

	// ���� ���������� ���� ��������� �������, �� ���������������� ����
	// ������������� ������ � ���� ��������� ������� ������������
	void DoChangeStatus()
	{
		if (GetFlag(Flag_Change_Status))
		{
			SetFlag(Flag_Chan_Exist,
				(status != 4) && (status != 5) && (status != 7));
			ResetFlag(Flag_Change_Status);
		}
	}
	
	// �������� �������� ����� ��������� � ��������� ��������������� ������
	// �������� id �� �����������! ��� ���� ������ ����� ������� ���� �������
	// ���������� true ���� ��������� ������
	bool TestChangeStruct(CHANNEL_STRUCT & pstr)
	{
		bool ret(false);
		if (pstr != *(LPCHANNEL_STRUCT)this)
		{
			if (pstr.id == id)
			{
				if (pstr.status != status)
				{
					ret = true;
					PlaceFlag(Flag_Change_Status);
				}
				if (pstr.frequency != frequency)
				{
					ret = true;
					PlaceFlag(Flag_Change_Freq);
				}
				if (wcscmp(pstr.name, name) != 0)
				{
					ret = true;
					PlaceFlag(Flag_Change_Name);
				}
				if (wcscmp(pstr.measure, measure) != 0)
				{
					ret = true;
					PlaceFlag(Flag_Change_Measure);
				}
				if ((pstr.subLevel != subLevel) ||
					(pstr.minLevel != minLevel) ||
					(pstr.maxLevel != maxLevel) ||
					(pstr.sense != sense))
				{
					ret = true;
					PlaceFlag(Flag_Change_Level);
				}
				if (pstr.dc != dc)
				{
					ret = true;
					PlaceFlag(Flag_Change_Level_DC);
				}
				if (pstr.referense != referense)
				{
					ret = true;
					PlaceFlag(Flag_Change_Level_dB);
				}
			}
			else
			{
				ret = true;
				PlaceFlag(Flag_Change_Chan);
			}
		}
		return ret;
	}
};
//------------------------------------------------------------------------------
struct ZetThreadParam1
{// ��������� ������, ������������ � ��������� ���������� �������
	bool bSupport7000;		// ���������� ��������� ����� 7000
	std::vector<CHANNEL_STRUCT_READ*> *pChannels;
};
//------------------------------------------------------------------------------
struct ZetThreadParam2
{// ��������� ����������, ������������ �� ��������� ���������� �������
	int index;
	std::vector<CHANNEL_STRUCT_READ*> *pChannels;
	PVOID pParamNext;		// ���������, ������������ �� ������ ����������
};
//------------------------------------------------------------------------------
class CThreadSRVmultiSelfS : public CCustomThreadSRV
{
private:
	// ����������� ������� ������������� ����������� ������
	const float m_freqADCMin;
	// ��� ������� ������������� ��� ����������� ������ ��� ���������� � 0,1 ���
	const float m_constFreqADCminFor_0_1;
	// ��� ������� ������������� ��� ����������� ������ ��� ���������� � 1 ���
	const float m_constFreqADCminFor_1_0;

	ZetThreadParam1 parSRV1;	// ��������� ������� ���������� ������

	// ��������� ���������� ������������ �������, 
	// ����� SmartArray<BYTE> - ��� ��������� CHANNEL_STRUCT_READ_CALC,
	// ������� ������ ����� �� ����� (� ������ ��������� ��� ����)
	std::vector<CHANNEL_STRUCT_READ*> *m_pChannels;

protected:

public:
	CThreadSRVmultiSelfS();
	virtual bool Create(
		CString nameThread,
		HWND hwnd,
		CDSRV *psrv,
		std::vector<CHANNEL_STRUCT_READ*> * pChannels,
		FunctionThread function,
		PVOID paramFunction = nullptr);

	virtual ~CThreadSRVmultiSelfS();

	// ������ �������
	void SetMinFreqADC(float freq);
	float GetMinFreqADC() { return m_freqADCMin; };

	float GetMinFreqADCforAverage_0_1sec() { return m_constFreqADCminFor_0_1; }
	float GetMinFreqADCforAverage_1sec() { return m_constFreqADCminFor_1_0; }

	bool GetSupport7000() { return parSRV1.bSupport7000; }
	void SetSupport7000(bool val) { parSRV1.bSupport7000 = val; }
	
	// ������� ������� ������ ��� �������� ������
	virtual void SetTime(double val);
};
//------------------------------------------------------------------------------