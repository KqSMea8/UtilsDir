//--------------------------------------------------------------------------
//	������ CDetectorShiftSTA_LTA_SRV.h
//	������ �� 10.11.2011
//	����������, ��� "����", �������� �.�.
//--------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <sta_lta\DetectorShiftSTA_LTA.h>
#include <ZETView_basic_class\CDSRV.h>
//--------------------------------------------------------------------------
class CDetectorShiftSTA_LTA_SRV : public CDetectorShiftSTA_LTA
{
private:
	long NumberVirtChannel;
	bool FbNeedVirtChannel;		// ��������� ����������� �����
	int FStartPointVirtChannel;	// ��������� ����� ������������ ������
	double FdTimeVirtChannel;	// �����  ������������ ������
	CDSRV *pSRV;
	float *pFreqADC;

protected:

public:
	CDetectorShiftSTA_LTA_SRV();
	//virtual ~CDetectorShiftSTA_LTA_SRV();
	bool Create(int numberSignal, int size, int sizePage,
				int numberSTA, int numberLTA,
				HWND hwnd, UINT wm_EVENT_START, UINT wm_EVENT_END,
				bool needVirtChan, int numVirtChan,
				CDSRV *psrv, float *pfreqADC,
				CShiftBufferDATE *pbufDate = NULL);
	
	// ������� �������
	bool GetNeedVirtChannel() { return FbNeedVirtChannel; }
	void SetNeedVirtChannel(bool val) { FbNeedVirtChannel = val; }

	virtual void SetNumberSTA(int val);
	virtual bool SetSize(int size);
	virtual void SetSizePage(int val);

	// ������� ������
	virtual void Calculation(float **ppData, double localTimeLastPage);
};
//--------------------------------------------------------------------------