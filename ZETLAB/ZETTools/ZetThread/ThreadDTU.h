//--------------------------------------------------------------------------
//	������ ZetWriteDTU.h
//	������ �� 15.11.2011
//	����������, ��� "����", �������� �.�.
//--------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include "ATLComTime.h"
#include <WriterDTU\WriterDTU.h>
#include <ZetThread\CustomZetThread.h>
#include <Buffer\ShiftBufferDATE.h>
//--------------------------------------------------------------------------
struct ParamTotal
{// ��������� ����� ����������, �������� � ���������� �������
	HWND hParent;
	CShiftBufferDATE *pBufDate;		// ��������� �� ����� ���
};
//--------------------------------------------------------------------------
struct ParamPrivat
{// ��������� ������� ����������, �������� � ������ ������ (����)
	CWriterDTU *pWriter;
//	int NumberChannels;		// ���-�� ������� (�������� � �������, �� ����� 3)
//	CString pCommentary[3];	// ������ ���� �������
//	CString pConversion[3];	// ������ ������������ �������
	float  *ppData[3];		// ������ ���������� �� ������

//	float FminFiltr;		// ������ ������� ���������� ��������
//	float FmaxFiltr;		// ������� ������� ���������� ��������
//	float FreqADC;			// ������� ������������� ��� � ��
//	float Tadc;				// ������ ������������� ��� � ���

//	int NumberFile;			// ����� ����� DTU
//	CString PathFile;		// ���� ����� DTU
//	CString NameFile;		// ������ ��� ����� DTU (�����������, ����� ������
							// ����� ���������� ��� � ���������)
	DATE T0;				// ����� ������ �������
	DATE T1;				// ����� ����� �������
};
//--------------------------------------------------------------------------
struct ZetThreadParamDTU	// ��������� ������
{// ����� ���� ��������� ����� ������� ��� �������� ���������� ������� ������
	struct ParamTotal  *pTotal;
	struct ParamPrivat *pPrivat;
};
//-------------------------------------------------------------------------
class CThreadDTU : public CCustomZetThread
{
private:
	ZetThreadParamDTU zParam1;

protected:
public:
	CThreadDTU();
	virtual ~CThreadDTU(void);
	virtual bool Create( CString nameThread, HWND hwnd, 
							ParamTotal *parTot );

	ParamPrivat m_ParamPrivat;
	CWriterDTU m_WriterDTU;
	// ������ ������/������ ������� -----------------	
	// ������ ������ --------------------------------
};
//--------------------------------------------------------------------------