//--------------------------------------------------------------------------
// ���� ShiftBufferSTA_LTA.h
// ������ �� 05.03.2012
// ��� "����", ����������, �������� �.�.
//--------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <Buffer\ShiftBuffer.h>
#include <Intel\ipp\ipps.h>
//--------------------------------------------------------------------------
class CShiftBufferSTA_LTA
{
private:
	int m_NumberSTA;		// ���-�� ����� � ��������� STA
	int m_NumberLTA;		// ���-�� ����� � ��������� LTA
	int m_NumberPoint;		// ��������� ���-�� ����� � ����� ����������
	int m_curNum;			// ������� ���-�� ����� ��� �������������
	int m_curNumPage;		// ������� ���-�� ������� ��� �������������
	float* m_pSTA_LTA;		// ������ ��������� �������� (������ FSizePage)
	bool m_bEnableSTA_LTA;	// ���-�� ������� ����� ������ FNumberPoint

	double m_amplSTA;			// ��������� � ��������� STA
	double m_amplLTA;			// ��������� � ��������� LTA
	double m_normFactor;		// ������������� �������� = NumberLTA/NumberSTA
	CShiftBuffer<float> *m_pSignal;

	void InitVariable();	// ������������� ����������

protected:

public:
	CShiftBufferSTA_LTA();
	virtual ~CShiftBufferSTA_LTA();
	bool Create(int size, int sizePage, int numberSTA, int numberLTA);

	int m_nCur_STA_LTA;		// ����� ����� ��� ��������� ��������� STA/LTA

	// ������� �������	
	float* GetGlobalAddres() { return m_pSignal->GetGlobalAddres(); }
	float* GetPointerLastPage() { return m_pSignal->GetPointerLastPage(); }
	float* GetPointerLastElement() { return m_pSignal->GetPointerLastElement(); }

	bool GetEnableSTA_LTA() { return m_bEnableSTA_LTA; }

	int GetSize() { return m_pSignal->GetSize(); }
	bool SetSize(int size);			// ����� ������ ������

	int GetSizePage() { return m_pSignal->GetSizePage(); }
	void SetSizePage(int val);		// ������ ���� ���������� ������
	
	int GetNumberSTA() { return m_NumberSTA; }
	void SetNumberSTA(int val);

	int GetNumberLTA() { return m_NumberLTA; }
	void SetNumberLTA(int val);

	// ������� ������
	void Calculation();		// ������� �� ������ ��������� ��������
	void Clear();			// ��������� � ���������� � ������
	void CopyNewData(float *pDataPage);	// ����������� ������ ����� ������
	float* GetPointerSTA_LTA() { return m_pSTA_LTA; }
	void NewData(float *pDataPage);	// ����� � ����������� ����� ������
	void ShiftData() { m_pSignal->ShiftData(); }	// ������ ����� ������
};
//--------------------------------------------------------------------------