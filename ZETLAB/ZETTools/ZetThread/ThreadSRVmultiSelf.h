//------------------------------------------------------------------------------
//	���� ThreadSRVmultiSelf.h
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
//		����� ������������ ��� ���������� ������ � �� ��������� � �����������
// ������� �������. �.�. �������, �������� �� ������ ���������,
// � ������� ��������� �������������, ������ ���-��� ������ � �.�.
//		���������, ��� ������ ������� �� ������� ���������� ���������!!!
//		��������� ���������� ������ ���� ������� ������ ���� ������ ���������
// ��������, ������� � ������� ���������� ������� ���������� ������,
// ��������: 0,1 ���.
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <limits>
#include <Intel\ipp\ipps.h>
#include <CodeQuality.h>
#include <Dialog_ZET\DataTime.h>
#include <ZetThread\CustomThreadSRV.h>
//------------------------------------------------------------------------------
#define WhileCountMax		100
#define MaxDeltaTimeInSec	60.
//------------------------------------------------------------------------------
struct MultiChannelSelf
{// ��������� ���������� �������
	bool bEnabled;	// ����� ����� ����������, �.�. � ������ �c�� ID
	bool bJob;		// ����� ������� ������ � ��������� ������
	bool bExist;	// ����� ���������� � ������� � �� ��������
	bool b7000;		// ����, ��� ����� ���������� ����� 7000 (������ ������� ���)
	bool bAutoNumberCounts;	// ���-�� ������ ������������ m_dt � DataTime
	long Channel;	// ����� ������
	long ChannelId;	// ������������� ������
	DWORD NumberCounts;// ���-�� ����������� ��������
	DWORD NumberBufferSRV;	// ������ ������ ������ ������ ������
	long Decade;	// ������
	long RetGetData;// ��� �������� ��������� ������
	DWORD DataSize;	// ������ ������� pData
	float *pData;	// ��������� ��� ��������� ������
	float freqADC;	// ������� ������������� ������
	CDataTime DataTime;	// ����� ������ ������

	MultiChannelSelf()
		: Channel(-1)
		, ChannelId(-1)
		, bEnabled(false)
		, bJob(false)
		, bExist(false)
		, b7000(false)
		, bAutoNumberCounts(true)
		, NumberCounts(0)
		, NumberBufferSRV(0)
		, Decade(0)
		, RetGetData(0)
		, DataSize(0)
		, freqADC(0)
		, pData(NULL)
	//	, DataTime ��������� ����������� ������
	{}
	MultiChannelSelf(const MultiChannelSelf &val)
	{
		Channel = val.Channel;
		ChannelId = val.ChannelId;
		bEnabled = val.bEnabled;
		bJob = val.bJob;
		bExist = val.bExist;
		b7000 = val.b7000;
		bAutoNumberCounts = val.bAutoNumberCounts;
		NumberCounts = val.NumberCounts;
		NumberBufferSRV = val.NumberBufferSRV;
		Decade = val.Decade;
		RetGetData = val.RetGetData;
		freqADC = val.freqADC;
		DataTime = val.DataTime;
		if (val.DataSize > 0)
		{
			DataSize = val.DataSize;
			pData = new float [DataSize];
			ippsCopy_32f(val.pData, pData, DataSize);
		}
		else
		{
			DataSize = 0;
			pData = nullptr;
		}
	}
	~MultiChannelSelf()
	{
		if (DataSize > 0)
		{
			if (nullptr != pData)
			{
				delete[] pData;
				pData = nullptr;
			}
			DataSize = 0;
		}
	}

	void operator = (const MultiChannelSelf &val)
	{
		bEnabled = val.bEnabled;
		bJob = val.bJob;
		bExist = val.bExist;
		b7000 = val.b7000;
		bAutoNumberCounts = val.bAutoNumberCounts;
		Channel = val.Channel;
		ChannelId = val.ChannelId;
		NumberCounts = val.NumberCounts;
		NumberBufferSRV = val.NumberBufferSRV;
		Decade = val.Decade;
		RetGetData = val.RetGetData;
		freqADC = val.freqADC;
		DataTime = val.DataTime;

		if (val.DataSize > 0)
		{
			ReSize(val.DataSize);
			ippsCopy_32f(val.pData, pData, DataSize);
		}
	}

	bool ReSize(DWORD size)
	{
		bool ret = DataSize >= size;
		if (!ret)
		{
			if (DataSize > 0)
			{
				delete[] pData;
				pData = nullptr;
			}
			try
			{
				pData = new float [size];
				DataSize = size;
				ret = true;
			}
			catch (CMemoryException*)
			{
				pData = nullptr;
				DataSize = 0;
			//	ret = false;	���
			}
		}
		return ret;
	}

	bool SetNumberCounts(DWORD val = 0)
	{// 0 - ������ �������������� ����� � ������������ � Data.m_dt,
	//	����� ������ ����� � �����.
		// ��� ������� ���� ����� �������� � �������������� ������ ������ �����
		// ������� ������ (�.�. ����� ������� ������� ������������� � ������) �
		// ����� ������� ���������� �������� DataTime.
		DWORD num(0);
		bAutoNumberCounts = val == 0;
		bool ret(true);
		if (bAutoNumberCounts)
		{
			double x = DataTime.GetTimeInterval() * freqADC;
			if (b7000)
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
			num = val;

		// �������� �� ���������� ������� ������
		if (NumberBufferSRV > 0)
		{
			if (num > NumberBufferSRV)
				num = NumberBufferSRV;
		}
		// �������� ������� �������
		if (num > 0)
			ret = ReSize(num);

		if (ret)
			NumberCounts = num;
		else
			NumberCounts = 0;
		return ret;
	}
};
//------------------------------------------------------------------------------
struct SendToSecGlobFunction
{// ��������� ����������, ������������ �� ��������� ���������� �������
	MultiChannelSelf *pMChan;
	int index;
	bool isCurrentChannel;
	PVOID pParamNext;
};
//------------------------------------------------------------------------------
struct ZetThreadParamSRVmultiSelf
{// ��������� ������
	bool Support7000;		// ���������� ��������� ����� 7000
	long* pFiIndexCur;
	std::vector<MultiChannelSelf> *pChannels;
};
//------------------------------------------------------------------------------
class CThreadSRVmultiSelf : public CCustomThreadSRV
{
private:
	long FIndexCur;
	// ����������� ������� ������������� ����������� ������
	float FFreqADCMin;
	// ��� ������� ������������� ��� ����������� ������ ��� ���������� � 0,1 ���
	const float m_constFreqADCminFor_0_1;
	// ��� ������� ������������� ��� ����������� ������ ��� ���������� � 1 ���
	const float m_constFreqADCminFor_1_0;
	ZetThreadParamSRVmultiSelf par1;
	std::vector<MultiChannelSelf> m_Channels;
//	bool CalcNumKadrs(int index, bool testIndex);

protected:

public:
	CThreadSRVmultiSelf();
	virtual bool Create( CString nameThread, HWND hwnd, CDSRV *psrv,
		int numberChannels,
		FunctionThread function, PVOID paramFunction = NULL );
	virtual ~CThreadSRVmultiSelf(void);


	// ������ �������
	std::vector<MultiChannelSelf>::iterator GetIteratorBegin() {
		return m_Channels.begin(); }
	std::vector<MultiChannelSelf>::iterator GetIteratorEnd() {
		return m_Channels.end(); }	
	MultiChannelSelf* GetPointerMultiChannelCur() {
		return &m_Channels[FIndexCur]; }

	float GetMinFreqADCforAverage_0_1sec() { return m_constFreqADCminFor_0_1; }
	float GetMinFreqADCforAverage_1sec() { return m_constFreqADCminFor_1_0; }

	bool GetSupport7000() {
		return par1.Support7000; }
	void SetSupport7000(bool val) {
		par1.Support7000 = val; }

	long GetChannelDecade(int index);
	void SetChannelDecade(int index, long val);

	bool GetChannel7000() {
		return m_Channels[FIndexCur].b7000; }

	virtual void SetTime(double val);

	long GetChannelNumber() {
		return m_Channels[FIndexCur].Channel; }
	bool SetChannelNumber(long num);

	int GetChannelNumberCounts() {
		return m_Channels[FIndexCur].NumberCounts; }
	bool SetChannelNumberCounts(DWORD val);
	bool SetChannelNumberCountsAuto();

	long GetIndexCur() { return FIndexCur; }
	bool SetIndexCur(int index, bool test = true);

	void SetMinFreqADC(float freq) { if (freq > 0.f) FFreqADCMin = freq; };
	float GetMinFreqADC() { return FFreqADCMin; };
	
	// ����� ������

	// ������ ������
	long AddChannel(long chan);
	virtual float* GetDataPointer(int index);
	bool Modify();
	bool StartChannel(int index);
	bool StopChannel(int index);
	bool TestIndex(int index) {
		return ( index >= 0 ) && ( index < int(m_Channels.size()) ); }
};
//------------------------------------------------------------------------------