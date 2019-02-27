//------------------------------------------------------------------------------
//	������ ZetWriteDTU.cpp
//	������ �� 15.11.2011
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
//		����� ������������ ��� ������ ������ � dtu-����. 
//		���� ����� ������� � ������� UTF-8.
//------------------------------------------------------------------------------
//		���� ��������� �� ������� � ������������ ���� (WPARAM � PostMessage)
// 1 - ������ ��� �������� �����
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include "ThreadDTU.h"
//------------------------------------------------------------------------------
DWORD WINAPI WriteDtuThread(PVOID pParam)
{
	//CString str; �� ������������
	const int maxSize = 16000;
	int i, n, n0, n1, numKadr = 0;
	DATE TimeFirstPoint;	// ����� ������� ����� � �����
	DATE TimeEndPoint;		// ����� ���������� ����� � �����
	DATE TimeEvent;			// ������������ �������
	DATE TimeFirstCur;		// ����� ������ ����� ������� �������
	DATE TimeEndCur;		// ����� ��������� ����� ������� �������

	ZetThreadParam0   *par0 = (ZetThreadParam0*)pParam;
	ZetThreadParamDTU *par1 = (ZetThreadParamDTU*)par0->pParamNext;
	ParamPrivat *parPri = par1->pPrivat;
	ParamTotal  *parTot = par1->pTotal;

	// ����� �������� ������ = �������� ������� ����� ����� ������ ������
	DWORD sleepTime = DWORD( parTot->pBufDate->GetSizePage() /
								parPri->pWriter->m_FreqADC * 250 );

	/////////////////////////////////////////////////////////////////////
	// ������ ��������� ���������� ������ � �����
	double a = 2.;
	double a3 = 0.8 * parTot->pBufDate->GetSize() /
						parPri->pWriter->m_FreqADC * SecAsPartOfDay;
	TimeEvent = parPri->T1 - parPri->T0;
	if ( TimeEvent < 0. )
	{// ������ ������������� ������
		par0->FlagViewStop = true;
		return 0;
	}
	TimeFirstPoint = parPri->T0 - a * TimeEvent;
	TimeEndPoint   = parPri->T1 + a * TimeEvent;
	if ( ( TimeEndPoint - TimeFirstPoint ) > a3 )
	{
		if ( TimeEvent > a3 )
		{// ������ ������� ������
			TimeFirstPoint = parPri->T0;
			TimeEndPoint   = TimeFirstPoint + a3;
		}
		else
		{// �������� �������� "a"
			a = 0.5 * ( a3 / TimeEvent - 1. );
			TimeFirstPoint = parPri->T0 - a * TimeEvent;
			TimeEndPoint   = parPri->T1 + a * TimeEvent;
		}
	}
	double dt = SecAsPartOfDay / parPri->pWriter->m_FreqADC;
	numKadr = int( ( TimeEndPoint - TimeFirstPoint ) / dt  );

	if ( numKadr > maxSize )
		numKadr = maxSize;

	/////////////////////////////////////////////////////////////////////
	// ��� ����� �������� ����� ������ ���� ---------------------------
	bool flag = true;
	do
	{
		TimeFirstCur = *parTot->pBufDate->GetGlobalAddres();
		TimeEndCur = *parTot->pBufDate->GetPointerLastElement();

		n0 = int( (TimeFirstPoint - TimeFirstCur) / dt );
		n1 = int( (TimeEndPoint   - TimeFirstCur) / dt );

		if ( n0 < 0 )
		{// ������ ������� ��� ����� �� ���������, ���� ��� ��������
			n0 = 0;
			if ( n1 > parTot->pBufDate->GetSize() )
				n1 = parTot->pBufDate->GetSize();
			flag = false;
		}
		else if ( n0 <= parTot->pBufDate->GetSizePage() )
		{// ������ ������� ������ �� �������� ��� ��������� ������,
		// ���� ��� ����
			if ( n1 > parTot->pBufDate->GetSize() )
				n1 = parTot->pBufDate->GetSize();
			flag = false;
		}
		else
		{// ��������� ������ �������� �� �0
			if ( n1 < parTot->pBufDate->GetSize() )
				flag = false;
		}

		if ( !flag )
		{
			n = n1 - n0;
			if ( numKadr < n )
				numKadr = n;
		//	parPri->pWriter->SetNumberSignals();
			parPri->pWriter->SetNumberKadrs(numKadr);
			for(i=0; i<parPri->pWriter->GetNumberSignals(); i++)
				parPri->pWriter->CopyData(i, parPri->ppData[i] + n0);
			parPri->pWriter->SetFirstTimeDATE( parTot->pBufDate->GetGlobalAddres()[n0] );
		}
		Sleep(sleepTime);
	}
	while( flag && par0->FlagJob && !par0->FlagStop );

	// ���� �� ����� do-while ����� �� FlagJob/FlagStop, �� ���� �� �����
	if ( par0->FlagJob && !par0->FlagStop )
	{
		if ( parPri->pWriter->Write() && par0->hWndParent )
		{
			::PostMessageW( par0->hWndParent, WM_WRITE_DTU, (WPARAM)par0->ztId,
					(LPARAM)parPri->pWriter->m_NameFileDTU.GetBuffer() );
			parPri->pWriter->m_NameFileDTU.ReleaseBuffer();
		}// ���� ���� �������
		else
		{	// ���� �� ������� --------------------------------------------
			if ( par0->hWndParent )
				::PostMessageW( par0->hWndParent, WM_ERROR_THREAD_DTU, 1, 0 );
		}
	}	// if ( par0->FlagJob && !par0->FlagStop )	
	// ���������� ������ ������
	par0->FlagViewStop = true;
	par0->FlagJob = false;
	return 0;
}
//------------------------------------------------------------------------------
CThreadDTU::CThreadDTU() : CCustomZetThread()
{
	zParam0.pParamNext = &zParam1;
}
//------------------------------------------------------------------------------
CThreadDTU::~CThreadDTU(void)
{// ���������� ������� ��������� ���������� �����, � ����� ������� �������,
	// ����� �������� ��������, ����� ����� ��������� � ��� �������� ��������
	SynchroFinishThread();
}
//------------------------------------------------------------------------------
bool CThreadDTU::Create( CString nameThread, HWND hwnd, ParamTotal *parTot )
{	// �.�. NULL, �� CCustomZetThread::Create ���������� ������ false, ������� ���:
	CCustomZetThread::Create(nameThread, hwnd, NULL);
	zParam1.pTotal = parTot;
	zParam1.pPrivat = &m_ParamPrivat;
	m_ParamPrivat.pWriter = &m_WriterDTU;
	return CreateZetThread(&WriteDtuThread);
}
//------------------------------------------------------------------------------