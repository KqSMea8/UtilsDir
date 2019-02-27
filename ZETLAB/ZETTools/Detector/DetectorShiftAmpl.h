//--------------------------------------------------------------------------
// ���� DetectorShiftAmpl.h
// ������ �� 13.06.2012
// ��� "����", ����������, �������� �.�.
//--------------------------------------------------------------------------
//		�����, ����������� ����������� �������� ��������������� �������
//--------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include "Constants.h"
#include "Intel\ipp\ipps.h"
#include "Buffer\ShiftBuffer.h"
//--------------------------------------------------------------------------
class CDetectorShiftAmpl
{
private:
	int FNumberSignals;		// ���-�� �������� ���������
	int FNumberSignals_1;	// ���-�� �������� ��������� - 1
	int NumPage;			// ���-�� ������� � ������
	int NumPageCur;			// ������� �������� � ������
	float FThreshold;		// ����� ��������������
	float FFactorThreshold;	// ���������
	UINT wmEvent;			// wm �������
	HWND hWnd;				// ���������� ����
	bool FbAutoThreshold;	// ���� ����������
	bool FbInit;			// ����, ��� �������� ������������������
	bool bJob;				// ����, ��� �������� ������ ��������
	CShiftBuffer<float> *pBufAmpl,	// ��������� ����� ���������
		*pBufSummAmpl, *pBufSummAmpl2;
	float summAmpl, summAmpl2, *p2;

	void CalcAutoThreshold();
	void ZeroVariable();

protected:

public:
	CDetectorShiftAmpl();
	virtual ~CDetectorShiftAmpl();
	bool Create(int numberSignal, int size, int sizePage,
				HWND hwnd, UINT wm_EVENT);	

	// ������� �������
	bool GetInitDetector() { return FbInit; }

	float GetThreshold() { return FThreshold; }
	void  SetThreshold(float val)
	{ if (!FbAutoThreshold && (val > 0.f) )	FThreshold = val; }

	bool GetAutoThreshold() { return FbAutoThreshold; }
	void SetAutoThreshold(bool val);

	int GetNumberSignals() { return FNumberSignals; }
	void SetNumberSignals(int newNumber)
	{ if (newNumber > 0) FNumberSignals = newNumber; }

	int GetSize() { return pBufAmpl->GetSize(); }
	bool SetSize(int size);

	int GetSizePage() { return pBufAmpl->GetSizePage(); }
	void SetSizePage(int val);

	float* GetPointerForDataAmpl()
	{ return pBufAmpl->GetGlobalAddres(); }

	// ������� ������
	void Calculation(float **ppData);
	void Reset();
};
//--------------------------------------------------------------------------