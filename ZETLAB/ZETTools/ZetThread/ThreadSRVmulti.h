//------------------------------------------------------------------------------
//	���� ThreadSRVmulti.h
//	������ �� 08.11.2012
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
#include <vector>
#include <Intel\ipp\ipps.h>
#include <ZetThread\CustomThreadSRV.h>
//------------------------------------------------------------------------------
struct MultiChannel
{// ��������� ���������� �������
	long Channel;	// ����� ������
	long ChannelId;	// ������������� ������
	bool bEnabled;	// ����� ����� ����������, �.�. � ������ �c�� ID
	bool bJob;		// ����� ������� ������ � ��������� ������
	bool bExist;	// ����� ���������� � ������� � �� ��������
	bool b7000;		// ����, ��� ����� ���������� ����� 7000 (������ ������� ���)
	bool bRead7000;	// ����, ��� ����� ������ ������ (������ ��� ����� 7000)
	int NumberKadrs;// ���-�� ����������� ������
	long Decade;	// ������
	long qualityCode;// ��� �������� ��������� ������
	float *pData;	// ��������� ��� ��������� ������
	double TimeData;// ����� ������ ������
	MultiChannel()
	{
		Channel =
		ChannelId = -1;
		bEnabled =
		bJob =
		bExist =
		b7000 =
		bRead7000 = false;
		NumberKadrs = 0;
		Decade = 0;
		qualityCode = 0;
		pData = NULL;
		TimeData = 0.;
	}
};
//------------------------------------------------------------------------------
struct SendToSecGlobFunction
{// ��������� ����������, ������������ �� ��������� ���������� �������
	MultiChannel *pMChan;
	int index;
	PVOID pParamNext;
};
//------------------------------------------------------------------------------
struct ZetThreadParamSRVmulti
{// ��������� ������
	bool Support7000;		// ���������� ��������� ����� 7000
	double Time7000;		// ��������� ����� ������� (�� 0)
	double TimeInterval7000;// �������� ������������� ������� �������
	std::vector<MultiChannel> *pChannels;
};
//------------------------------------------------------------------------------
class CThreadSRVmulti : public CCustomThreadSRV
{
private:
	int FIndexCur;
	float FFreqADCMin;	// ����������� ������� ������������� ����������� ������
	std::vector<MultiChannel> m_Channels;
	ZetThreadParamSRVmulti par1;
	bool CalcNumKadrs(int index, int *pnum, bool testIndex = true);
	double SetCurTime7000(long chan, long fast);

protected:

public:
	CThreadSRVmulti();
	virtual bool Create( CString nameThread, HWND hwnd, CDSRV *psrv,
		int numberChannels,
		FunctionThread function, PVOID paramFunction = NULL );
	virtual ~CThreadSRVmulti(void);


	// ������ �������
	std::vector<MultiChannel>::iterator GetIteratorBegin() {
		return m_Channels.begin(); }
	std::vector<MultiChannel>::iterator GetIteratorEnd() {
		return m_Channels.end(); }	
	MultiChannel* GetPointerMultiChannelCur() {
		return &m_Channels[FIndexCur]; }

	bool GetSupport7000() {
		return par1.Support7000; }
	void SetSupport7000(bool val) {
		par1.Support7000 = val; }

	bool GetChannel7000() {
		return m_Channels[FIndexCur].b7000; }

	virtual void SetTime(double val);

	long GetChannelNumber() {
		return m_Channels[FIndexCur].Channel; }
	bool SetChannelNumber(long num);

	int GetChannelNumberKadrs() {
		return m_Channels[FIndexCur].NumberKadrs; }

	int GetIndexCur() {
		return FIndexCur; }
	bool SetIndexCur(int index, bool test = true);
	
	double GetTimeInterval7000() { return par1.TimeInterval7000; }
	virtual void SetTimeInterval(double val);
	void SetTimeInterval7000(double val);

	// ����� ������

	// ������ ������
	long AddChannel(long chan);
	virtual float* GetDataPointer(int index);
	void Modify();
	bool StartChannel(int index);
	bool StopChannel(int index);
	bool TestIndex(int index) {
		return ( index >= 0 ) && ( index < int(m_Channels.size()) ); }

	virtual void SetNewTime(long chan, long fast);
	virtual void ZeroTime();
};
//------------------------------------------------------------------------------