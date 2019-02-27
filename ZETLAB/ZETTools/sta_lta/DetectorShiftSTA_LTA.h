//--------------------------------------------------------------------------
// ���� CDetectorShiftSTA_LTA.h
// ������ �� 05.03.2012
// ��� "����", ����������, �������� �.�.
//--------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <Constants.h>
#include <Buffer\ShiftBufferDATE.h>
#include <sta_lta\ShiftBufferSTA_LTA.h>
//--------------------------------------------------------------------------
enum Calculation_Ret_Value
{
	crv_StartEvent = -1,
	crv_NoEvent,
	crv_EndEvent
};
//--------------------------------------------------------------------------
class CDetectorShiftSTA_LTA
{
private:

protected:
	bool bEvent;			// ���� �������
	int FNumberSignals;		// ���-�� �������� ���������
	int FNumberSignals_1;	// ���-�� �������� ��������� - 1
	int FNumberLTA;			// ���-�� ����� � ��������� LTA
	int FNumberSTA;			// ���-�� ����� � ��������� STA
	int FAllNumber;			// = FNumberLTA + FNumberSTA
	int FCountInEvent;		// ���-�� ����� � �������
	float Factor;			// ��������� ������������ ���������
	float *m_pData_in;
	UINT wmEventStart;		// wm ������� "�������� �������"
	UINT wmEventEnd;		// wm ������� "����������� �������"
	double factorAmpl;		// �������� ������������ ���������
	HWND hWnd;				// ���������� ����
	CShiftBuffer<float> *m_pSTA_LTA;	// ����� STA_LTA
	CShiftBufferSTA_LTA **m_ppBufAll;	// ������ �������
	DATE T0, T1;
	CShiftBufferDATE *pBufDate;

	bool CreateAllBufSTA_LTA(int numSignal, int size, int sizePage,
					int numberSTA, int numberLTA);
	void DeleteAllBufSTA_LTA();

public:
	CDetectorShiftSTA_LTA();
	virtual ~CDetectorShiftSTA_LTA();
	bool Create(int numberSignal, int size, int sizePage,
				int numberSTA, int numberLTA,
				HWND hwnd, UINT wm_EVENT_START, UINT wm_EVENT_END,
				CShiftBufferDATE *pbufDate = NULL);
	
	bool m_bRemovalOneComponentEvent;	// �������� ���������������� �������
	float m_Threshold;		// ����� ��������������

	// ������� �������
	bool GetEnableSTA_LTA() { return m_ppBufAll[0]->GetEnableSTA_LTA(); }

public:
	int GetNumberSignals() { return FNumberSignals; }
	bool SetNumberSignals(int newNumber);

	int GetNumberSTA() { return FNumberSTA; }
	virtual void SetNumberSTA(int val);

	int GetNumberLTA() { return FNumberLTA; }
	void SetNumberLTA(int val);

	float* GetPointerForData(int iSignal)
		{ return m_ppBufAll[iSignal]->GetGlobalAddres(); }

	float* GetPointerSTA_LTA() { return m_pSTA_LTA->GetGlobalAddres(); }
	float* GetPointerLastPage() { return m_pSTA_LTA->GetPointerLastPage(); }

	int GetSize() { return m_pSTA_LTA->GetSize(); }
	virtual bool SetSize(int size);

	int GetSizePage() { return m_pSTA_LTA->GetSizePage(); }
	virtual void SetSizePage(int val);

	float GetFactor() { return Factor; }
	void SetFactor(float val) {
		if ((0.5f <= val) && (val <= 1.f)) Factor = val;	}

	// ������� ������
	Calculation_Ret_Value Calculation(float **ppData, int *pIndex = NULL);
	void Clear();
};
//--------------------------------------------------------------------------