//------------------------------------------------------------------------------
//	���� ThreadSRV.cpp
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
//   ���������� ������� SRV_Thread - ��� ��������� ������� ������. Ÿ �������� -
// ��� ��������� �� ��������� ��������� ����� ����������, ������� �������� �
// ���� ��������� �� ��������� ������� ���������� (��. ������� CreateZetThread
// ������-��������).
//   � ������� ������ ����� ���������� ������ ���������� ��������� ����������
// �������, ��� ������ ����������� ��������� ��������� ������. ����� ���������
// ���������� ������� � � �������� - ��� �������������� ���������� function �
// paramFunction ������� Create ������� ������.
//		��� ���������� ���� ������ �������� ��������� ��������� ������������� ����:
// 1. WM_NEW_DATA_SRV - ������ ������� � ���������� ��� ������,
//		��������� ��� ����������;
// 2. WM_END_JOB_SRV - ��� ������� ������, ��������� ��� ����������.
//		� ��� ��������� ������� ����� ��������� ���� ������.
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include <ZetThread\ThreadSRV.h>
//------------------------------------------------------------------------------
DWORD WINAPI SRV_Thread(PVOID pParam)
{// ������� �������� ������ �������
	bool bWait;
	// ����� ��������� ���������� ������-����������� CCustomZetThread
	ZetThreadParam0 *par0 = (ZetThreadParam0*)pParam;
	// ����� ��������� ���������� ������-�������� CCustomThreadSRV
	ZetThreadParamSRV0 *parSRV0 = (ZetThreadParamSRV0*)par0->pParamNext;
	// ����� ��������� ���������� ������� ������ CThreadSRV
	ZetThreadParamSRV1 *par1 = (ZetThreadParamSRV1*)parSRV0->pParamNext;

	while ( par0->FlagJob )
	{// ���� ������ -----------------------------------------------------
		par0->FlagViewStop = false;
		bWait = true;
		if (par1->bNoWaitMissingChannel)
		{// ������� ������, ���������� ������ ���
			// �������� ����� ���� ������� ----------------
			// ����� ���� �� ���� ������� ���� ������� NumFileUsed
			for (auto & it : par1->vectChan)
			{
				if ( parSRV0->pSRV->NumFileUsed(it.channel) == -1 )
				{// ������ ����������
					if (par0->hWndParent)
						::PostMessageW( par0->hWndParent, WM_END_JOB_SRV, 0, 0 );
					bWait = false;
					par0->FlagJob = false;
					break;	// ���������� ������ ������
				}
				bWait &= parSRV0->pSRV->CurrentTime(it.channel) >= parSRV0->Time;
			}
		}
		else	// ����� ����� (���� �����, ���� ��������� �����)
		{// ��� ��������� �����, �������� ����� 0-������, ������� ���� ������
			if ( parSRV0->pSRV->NumFileUsed(0) == -1 )
			{// ������ ����������
				if (par0->hWndParent)
					::PostMessageW( par0->hWndParent, WM_END_JOB_SRV, 0, 0 );
				bWait = false;
				par0->FlagJob = false;
				break;	// ���������� ������ ������
			}
			bWait = bWait && ( parSRV0->pSRV->CurrentTime(0) >=
							parSRV0->Time );
		}

		if (bWait)
		{// ����� �������� �����, ����� ������, ���� ���� ---------------
			// ���� ��������� ������ ������
			if (par1->bNeedReadData && par1->bNoWaitMissingChannel)
			{
				par1->QualityCode = 0;
				for (auto & it : par1->vectChan)
				{// ����� ������ �� ���� �������
					it.saArray.ReSize(par1->NumberKadrs);
					par1->QualityCode |= parSRV0->pSRV->GetData(it.channel,
						par1->Decade, parSRV0->Time,
						par1->NumberKadrs, it.saArray.pData);
				}
				par1->dTimeData = parSRV0->Time;
				// ������� ��������� ���� �������� ������, ��������� ������ � ���!
				if (par0->Function)
				{// ���� ������� ��������� ������ ������
					long kod = (*par0->Function)(par0->pParamFunction);
					Quality *pqc = (Quality*)&kod;
					if ( pqc->IsAllData() &&		// ������ ���
							parSRV0->hEventSynchro &&	// �������� SynchroView
							( WaitForSingleObject(parSRV0->hEventSynchro,
								par0->TimeOutSend) == WAIT_OBJECT_0 ) )
						::PostMessageW( par0->hWndParent, WM_NEW_DATA_SRV, 0, (LPARAM)kod );
				}
			}
			parSRV0->Time += parSRV0->TimeInterval;	// ��������� �������
		}
		else // if (bWait) ���� ����� ������ ------------------
			Sleep(par0->TimeSleep);	// ������ ��������, ����� �� �����, ���

		// ���� ���� ������������ ����� ----------------------------------
		if (par0->FlagStop)
		{
			par0->FlagViewStop = true;
			SuspendThread(par0->hThread);	// ��������� ������� � ����� while
		}
	}// while ( par0->FlagJob )
	return 0;
}
//------------------------------------------------------------------------------
// ����������� ������
//----------------------
CThreadSRV::CThreadSRV() : CCustomThreadSRV()
	, FAutoInterval(false)
{
	zParam0.pParamNext = &parSRV0;			// ������� ��������� ������
	parSRV0.pParamNext = &par1;

	// ��������� �� ���������
	par1.bNeedReadData = true;
	par1.Decade = 0;
	par1.NumberKadrs = 0;
	par1.bNoWaitMissingChannel = true;
}
//------------------------------------------------------------------------------
bool CThreadSRV::Create( CString nameThread, HWND hwnd, CDSRV *psrv,
						int numberChannels, int numberKadr, bool bautoInterval,
						FunctionThread function, PVOID paramFunction )
{
	bool ret = CCustomThreadSRV::Create(nameThread, hwnd, psrv,
			numberChannels, function, paramFunction ) && (numberChannels > 0);
	if (ret)
	{
		FAutoInterval = bautoInterval;
		SetNumberChannels(numberChannels);
		SetNumberKadrs(numberKadr);
		ret = CreateZetThread(&SRV_Thread);	// ������ ���������������� �����
	}
	return ret;
}
//------------------------------------------------------------------------------
//		���������� ������
//-----------------------
CThreadSRV::~CThreadSRV()
{// ���������� ������� ��������� ���������� �����, � ����� ������� �������,
	// ����� �������� ��������, ����� ����� ��������� � ��� �������� ��������
	SynchroFinishThread();
// 	DeleteMassivAll();
}
//------------------------------------------------------------------------------
void CThreadSRV::CreateMassivChan(int numChan)
{
	int size = par1.vectChan.size();
	if ((numChan >= 0) && (numChan != size))
	{
		par1.vectChan.resize(numChan);

		if (numChan > size)
		{
			auto it = par1.vectChan.data() + size;
			for (int i = size; i < numChan; ++i, ++it)
			{
				it->channel = i;
				parSRV0.pSRV->GetChannelParEx(i, it);
			}
		}
	}
}
//------------------------------------------------------------------------------
bool CThreadSRV::SetNumberChannels(int val)
{
	bool ret = (0 < val) && (val < parSRV0.pSRV->QuanChan());
	int n = val, num = parSRV0.pSRV->QuanChan();
	if (n < 1)
	{
		n = 1;
		ret = false;
	}
	else if (n > num)
	{
		n = num;
		ret = false;
	}
	if ((int)par1.vectChan.size() != n)
		CreateMassivChan(n);
	parSRV0.NumberChannels = n;		// ������ � �����!!!
	
	return ret;
}
//------------------------------------------------------------------------------
int CThreadSRV::GetChannels(int iChannels)
{
	return ((0 <= iChannels) &&
			(iChannels < (int)par1.vectChan.size())) ?
		par1.vectChan[iChannels].channel : -1;
}
//------------------------------------------------------------------------------
bool CThreadSRV::SetChannels(int iChannels, int val)
{	// �������� ������� iChannels
	bool ret = (0 <= iChannels) && (iChannels < (int)par1.vectChan.size());
	if (ret)
	{
		ret = (0 <= val) && (val < parSRV0.pSRV->QuanChan());
		long chan = ret ? val : -1;
		par1.vectChan[iChannels].channel = chan;
		parSRV0.pSRV->GetChannelParEx(chan, par1.vectChan.data() + iChannels);
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CThreadSRV::SetNumberKadrs(int val)
{
	bool ret = val > 0;
	if (ret)
	{
		par1.NumberKadrs = parSRV0.pSRV->BufferSize() / 2;
		ret = val <= par1.NumberKadrs;
		if (ret)
			par1.NumberKadrs = val;

		if (FAutoInterval)
			parSRV0.TimeInterval = par1.NumberKadrs / par1.vectChan[0].frequency;
	}
	return ret;
}
//------------------------------------------------------------------------------
float * CThreadSRV::GetDataPointer(int iChannel)
{
	return
		((0 <= iChannel) && (iChannel < (int)par1.vectChan.size())) ?
		(par1.vectChan.data() + iChannel)->saArray.pData :
		nullptr;
}
//------------------------------------------------------------------------------
bool CThreadSRV::Modify()
{
	bool ret(false);
	for (auto & it : par1.vectChan)
	{
		CHANNEL_FOR_KIT cfk;
		parSRV0.pSRV->GetChannelParEx(it.channel, &cfk);
		ret |= it.TestChangeStruct(cfk);
	}
	return ret;
}
//------------------------------------------------------------------------------