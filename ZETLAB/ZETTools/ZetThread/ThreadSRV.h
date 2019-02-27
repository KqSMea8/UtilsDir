//------------------------------------------------------------------------------
//	���� ThreadSRV.h
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
//		����� ������������ ��� ���������� ������ � �� ��������� ������
// ��������������� �������, �.�. � ���������� �������, ����������� ����� �����.
// �.�. ������� �� ������ ���������, � ����������� �������� �������������,
// ���-��� ������, ������� � �.�.
//		����� �������� ����������� ������ CCustomThreadSRV.
//		���������, ��� ������ ���� ������� ������� ���������� ���������, �������
// ��� ���� ������� ������� ����� ����������� ������ ��� �������������.
//		��� ���������� ���� ������ �������� 3 ��������� ������������� ����:
// 1. WM_NEW_DATA_SRV - ������ ������� � ���������� ��� ������,
//		��������� ��� ����������;
// 2. WM_ERROR_READ_SRV	- ������ ��� ������ ������,
//		��������� ��� ������ � ����� ������, ��� ������ �������� ���� ������
// 3. WM_ERROR_CALC_SRV - ������ ��� ��������� ������, ��������� ��� ������.
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ZetThread\CollectionOfKits\ChannelForKit.h>
#include <ZetThread\CustomThreadSRV.h>
//------------------------------------------------------------------------------
//   ���������, ���������� ������� ��������� ������, ��������� �� � ���������
// �������� ������ ��������� ����� ���������� (��. �����-��������).
struct ZetThreadParamSRV1
{
// 	int *pChannels;			// ������ ������� �������
	bool bNeedReadData;		// ������ ������ ����/�� ����
	int NumberKadrs;		// ���-�� ����������� ������
// 	float **ppMasData;		// ���������� ������ ���������� (���� ������ ������)
	long Decade;			// ������ ������ �������
	bool bNoWaitMissingChannel;	// �� ��� ��������� �����
	double dTimeData;		// ����� ������ ������
	long QualityCode;		// ��� �������� ������
	std::vector<CHANNEL_FOR_KIT> vectChan;	// ������ �������
	PVOID pSaveData;		// ����� ��������� ������� ���������� ���������
};
//------------------------------------------------------------------------------
class CThreadSRV : public CCustomThreadSRV
{
private:
	ZetThreadParamSRV1 par1;	// ������� ��������� ������
	void CreateMassivChan(int numChan);
// 	void DeleteMassivAll();
	double SetCurTime();
// 	bool bEnableMassChan;
// 	bool bEnableMassKadr;
	bool FAutoInterval;
protected:

public:
	CThreadSRV();
	virtual bool Create( CString nameThread, HWND hwnd, CDSRV *psrv,
					int numberChannels,  int numberKadr, bool bautoInterval,
					FunctionThread function, PVOID paramFunction = NULL );
	virtual ~CThreadSRV(void);

	// ������ �������
	bool GetAutoInterval() { return FAutoInterval; }
	void SetAutoInterval(bool val) { FAutoInterval = val; }

	bool SetNumberChannels(int val);

	int GetChannels(int iChannels);
	bool SetChannels(int iChannels, int val);

	long GetDecade()	{ return par1.Decade; }
	void SetDecade(long val)	{ par1.Decade = val; }

	HWND GetWindowHWND() { return zParam0.hWndParent; }

	bool GetNeedReadData() { return par1.bNeedReadData; }
	void SetNeedReadData(bool val) { par1.bNeedReadData = val; }
	
	int  GetNumberKadrs()	{ return par1.NumberKadrs; }
	bool SetNumberKadrs(int val);
	
	bool GetWaitMissingChannel() { return !par1.bNoWaitMissingChannel; }
	void SetWaitMissingChannel(bool val)
					{ par1.bNoWaitMissingChannel = !val; }

	double GetTimeData() { return par1.dTimeData; }
	long GetQualityData() { return par1.QualityCode; }

	// ����� ������

	// ������ ������
	virtual float* GetDataPointer(int iChannel);

	// ���������� true, ���� � ������� ���� ���������
	bool Modify();
};
//------------------------------------------------------------------------------