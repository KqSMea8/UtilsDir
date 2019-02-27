//------------------------------------------------------------------------------
//	���� ThreadSRVmultiSelf.cpp
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
//   ���������� ������� SRV_ThreadMulti - ��� ��������� ������� ������.
// Ÿ �������� - ��� ��������� �� ��������� ��������� ����� ����������,
// ������� �������� � ���� ��������� �� ��������� ������� ����������.
//   � ������� ������ ������� ����������� ����� � ������� ����������� �������.
// � ������� ������ ��� �����! �� ��������� ������� ����������� ������ � ���
// ������� ������ ���������� ��������� ���������� �������, ��� ������ �����������
// ��������� ��������� ������ ������. ��� ������� ���������� ��� ��������
// ���������� ����������� ������ �� ������. ���� ��� AllData ����� ���� ����� 0
// (��� ������ ����������), �� ��������� ���������� ����������� ������ � ������
// ��������� ����, � ����� ����� ��������� ��������� WM_NEW_DATA_SRV.
//------------------------------------------------------------------------------
//		��� ���������� ���� �������� ������ �������� ��������� ���������:
//	WM_END_JOB_SRV - ��� ������� ������, ��������� ��� ����������;
//	WM_NEW_DATA_SRV - ������ �� ������ �� ������� ������� � ����������,
//		��������� ����� ������ � ��� �������� ���������� ���������.
//------------------------------------------------------------------------------
//
#include "StdAfx.h"
#include <ZetThread\ThreadSRVmultiSelf.h>
//------------------------------------------------------------------------------
DWORD WINAPI SRV_ThreadMultiSelf(PVOID pParam)
{// ��������� ���������� ������� �������� ������
	int indexChan;
	CodeQuality retFunction;
	bool bIsSyncGUI(false);
	SendToSecGlobFunction parSend;
	// ����� �������, ���� � ���� ��� ��� ����� true
	//  (bEnabled, bJob � bExist)

	// ����� ��������� ���������� ������-����������� CCustomZetThread
	ZetThreadParam0 *par0 = (ZetThreadParam0*)pParam;
	// ����� ��������� ���������� ������-�������� CCustomThreadSRV
	ZetThreadParamSRV0 *parSRV0 = (ZetThreadParamSRV0*)par0->pParamNext;
	// ����� ��������� ���������� ������� ������ CThreadSRVmulti
	ZetThreadParamSRVmultiSelf *par1 = (ZetThreadParamSRVmultiSelf*)parSRV0->pParamNext;
	// ������ �������������
	std::vector<MultiChannelSelf> *pChannels = par1->pChannels;
	// ����� ��������� - �������� ��������� ���������� �������
	parSend.pParamNext = par0->pParamFunction;

	while ( par0->FlagJob )
	{// ���� �������� ������ -------------------------------------------
		par0->FlagViewStop = false;
		// �������� �� ����-�� Modify -----------------------------------
		WaitForSingleObject(parSRV0->hEventModify, par0->TimeOutSend);
		//---------------------------------------------------------------
		// �������� NumFileUsed ���� ������������ �������
		for(auto it=pChannels->begin(), itEnd=pChannels->end(); it!=itEnd; ++it)
		{// ��������� ����-�� ������� ������
			bool flag = true;
			if ( it->bEnabled && it->bJob && it->bExist)
			{// ���� ������� �����
				flag = false;
				if ( parSRV0->pSRV->NumFileUsed(it->Channel) == -1 )
				{// ������ ����������
					::PostMessageW( par0->hWndParent, WM_END_JOB_SRV, 0, 0 );
					par0->FlagJob = false;
					break;	// ������ �� ����� � �������� �����
				}
			}
			if (flag)
				parSRV0->pSRV->NumFileUsed(0);
		}
		//---------------------------------------------------------------
		if (par0->FlagJob)
		{// ���� ���������� ���
			indexChan = 0;
			for(auto it=pChannels->begin(), itEnd = pChannels->end(); it!=itEnd; ++it,++indexChan)
			{// ���� �� ���� �������
				if (par0->FlagStop)	
					break;
				if ( it->bEnabled && it->bJob && it->bExist)
				{// ���� ������� �����
					// �������� ����� �� ������ -14 ------------
					double timecur = parSRV0->pSRV->CurrentTime(it->Channel);
					double dtime = timecur - it->DataTime.GetTime();
					if (fabs(dtime) > MaxDeltaTimeInSec)
					{// �������� �����! �������� �����
						it->DataTime.SetStartTime(timecur);
					}
					
					// while ����� ��� ����, ����� �������� �� ������������ ������ �������
				//	if (parSRV0->pSRV->CurrentTime(it->Channel) >= it->DataTime.GetTime() ) )
					BYTE whileCount = 0;
					while ( par0->FlagJob && !par0->FlagStop &&
							(parSRV0->pSRV->CurrentTime(it->Channel) >=
													it->DataTime.GetTime() ) )
					{
						if(bIsSyncGUI)
						{// ����� GUI �������� ���� ������
							if ( !it->b7000 || (it->b7000 && par1->Support7000) )
							{
								it->RetGetData = parSRV0->pSRV->GetData(
									it->Channel, it->Decade, it->DataTime.GetTime(),
									it->NumberCounts, it->pData );
								if (it->RetGetData < 0)
									ippsSet_32f(std::numeric_limits<float>::quiet_NaN(), it->pData,
												(int)it->NumberCounts);
								parSend.index = indexChan;
								parSend.isCurrentChannel = indexChan == *par1->pFiIndexCur;
								parSend.pMChan = it._Ptr;
								retFunction = (*par0->Function)(&parSend);	// ���-�� ����� (��������, ...)
								if ( retFunction.IsAllData() )
								{// ������� � ���������� ��������� ������
									bIsSyncGUI = false;
									::PostMessageW( par0->hWndParent, WM_NEW_DATA_SRV,
														(WPARAM)indexChan, (LPARAM)retFunction );
								}
								else if (retFunction.NeedSynchro)
								{
									bIsSyncGUI = false;
								}
							}
							// ��������� �������
							it->DataTime.IncrementTime();
						}
						else
						{// ����� GUI ��������, ���� �����
							bIsSyncGUI = WaitForSingleObject(parSRV0->hEventSynchro, par0->TimeOutSend) == WAIT_OBJECT_0;
						}
						if (++whileCount > WhileCountMax)
							break;	// ����� �� while
					}	// while
				}	// if ( it->bEnabled && ...
			}	// for(auto it=pChannels->begin(); ...
			// ���������� ����� ----------------------------------------------
			if (!par0->FlagStop)					// ������������� ����� �� ����
				Sleep(par0->TimeSleep);			// ���� ������ ���������
			else
			{// ���� ���������� �����
				par0->FlagViewStop = true;
				SuspendThread(par0->hThread);// ��������� ������� � ����� while
			}
		}	// if (par0->FlagJob)
	}// while ( par0->FlagJob )
	return 0;
}
//------------------------------------------------------------------------------
CThreadSRVmultiSelf::CThreadSRVmultiSelf() : CCustomThreadSRV()
, m_constFreqADCminFor_0_1(10.f)
, m_constFreqADCminFor_1_0(1.f)
{
	FIndexCur = 0;
	FFreqADCMin = 50;
	m_Channels.clear();
	par1.pChannels = &m_Channels;
	par1.Support7000 = true;
	par1.pFiIndexCur = &FIndexCur;
	parSRV0.pParamNext = &par1;
}
//------------------------------------------------------------------------------
bool CThreadSRVmultiSelf::Create(
	CString nameThread,			// ��� ������
	HWND hwnd,					// HWND ������������� ����
	CDSRV *psrv, 				// ��������� �� ������
	int numberChannels,			// ���-�� �������
	FunctionThread function,	// ����� ��������� �������
	PVOID paramFunction )		// ��������� ��������� �������
{
	bool ret = (numberChannels > 0) &&
		CCustomThreadSRV::Create(nameThread, hwnd, psrv, numberChannels,
									function, paramFunction);
	if (ret)
	{
		parSRV0.NumberChannels = numberChannels;
		try
		{
			m_Channels.resize( (size_t)parSRV0.NumberChannels );
		}
		catch (CMemoryException*)
		{
			parSRV0.NumberChannels = 0;
			m_Channels.clear();
			ret = false;	
		}
		if (ret)	// ������ ���������������� �����
			ret = CreateZetThread(&SRV_ThreadMultiSelf);
	}
	return ret;
}
//------------------------------------------------------------------------------
CThreadSRVmultiSelf::~CThreadSRVmultiSelf()
{// ���������� ������� ��������� ���������� �����, � ����� ������� ������,
	// ����� �������� ��������, ����� ����� ��������� � ��� �������� ��������
	SynchroFinishThread();
}
//------------------------------------------------------------------------------
//bool CThreadSRVmultiSelf::CalcNumKadrs(int index, bool testIndex)
//{// ������ ���-�� ������ ��� ������
//	DWORD num = 0;
//	bool ret = false;
//	if (testIndex)
//		ret = TestIndex(index);
//	else
//		ret = true;
//
//	if (ret)
//	{
//		MultiChannelSelf *pMChan = &m_Channels[index];
//		if (pMChan->bEnabled)
//		{// ����� ����� ���������
//			if (pMChan->bAutoNumberCounts)
//			{
//				double x = pMChan->DataTime.GetTimeInterval() *
//					pMChan->freqADC;
//				if (pMChan->b7000)
//				{// ��������� ������� �������������, ����� ����� ��� ������,
//					// ��� ���� ��� ����� �������� ������
//					num = (DWORD)(int)floor(x);
//					if (x > (double)num)
//						++num;
//				}
//				else
//					num = (DWORD)(int)floor(x + 0.5);
//			}
//			else
//				num = pMChan->NumberCounts;
//
//			// �������� �� ���������� ������� ������
//			if (num > pMChan->NumberBufferSRV)
//				num = pMChan->NumberBufferSRV;
//
//			// �������� ������� �������
//			if (num > 0)
//				ret = pMChan->ReSize(num);
//
//			if (ret)
//				pMChan->NumberCounts = num;
//			else
//				pMChan->NumberCounts = 0;
//		}
//	}
//	return ret;
//}
//------------------------------------------------------------------------------
long CThreadSRVmultiSelf::GetChannelDecade(int index)
{
	return TestIndex(index) ? m_Channels[index].Decade : -1;
}
//------------------------------------------------------------------------------
void CThreadSRVmultiSelf::SetChannelDecade(int index, long val)
{
	if( TestIndex(index) )
	{
		if (val < 0)
		{
			val = 0;
		}
		else if (val >= MAXDECADE)
		{
			val = MAXDECADE - 1;
		}
		m_Channels[index].Decade = val;
		m_Channels[index].NumberBufferSRV = parSRV0.pSRV->DecadeBufferSize(val) / 2;
	}
}
//------------------------------------------------------------------------------
bool CThreadSRVmultiSelf::SetChannelNumber(long numChan)
{// ��������� ������ ������ ������� �������� ������
	MultiChannelSelf *pMChan = &m_Channels[FIndexCur];
	bool ret = ( numChan >= 0 ) && ( numChan < parSRV0.pSRV->QuanChan() );
	pMChan->Channel = numChan;
	if (ret)
	{//����� ���� � �������
		pMChan->bJob = pMChan->bEnabled = ret =
			parSRV0.pSRV->IdChan(numChan, &pMChan->ChannelId) == 0;
		if (ret)
		{// ID � ������ �������� ���������;
			long status = parSRV0.pSRV->GetStatus(numChan);
			pMChan->bExist = (status != 4) && (status != 5) && (status != 7);
			// 4 - ����������� ����� ���; 5 - ����� ������������ ���
			// 7 - ����������� ����� ����������������� �������
			pMChan->freqADC = parSRV0.pSRV->Frequency(numChan);
			pMChan->b7000 = pMChan->freqADC < FFreqADCMin;
			pMChan->NumberBufferSRV = parSRV0.pSRV->DecadeBufferSize(pMChan->Decade) / 2;
			pMChan->SetNumberCounts();
		//	CalcNumKadrs(FIndexCur, false);
		}
		else
		{
			pMChan->bExist = false;
			pMChan->ChannelId = -1;
		}
	}
	else
	{
		pMChan->bJob = pMChan->bExist = pMChan->bEnabled = false;
		pMChan->ChannelId = -1;
	}
	return ret;	// �������� ����� ���������� ��� ���
}
//------------------------------------------------------------------------------
bool CThreadSRVmultiSelf::SetChannelNumberCounts(DWORD val)
{// ������� �������� ������ ���-�� ����������� ������ "������"
	return m_Channels[FIndexCur].SetNumberCounts(val);
}
//------------------------------------------------------------------------------
bool CThreadSRVmultiSelf::SetChannelNumberCountsAuto()
{// ������� �������� ������ ���-�� ����������� ������ ����� ������� ����������
//	� CDataTime
	return m_Channels[FIndexCur].SetNumberCounts();
}
//------------------------------------------------------------------------------
bool CThreadSRVmultiSelf::SetIndexCur(int index, bool test)
{// ������� �������� �������
	bool ret;
	if (test)
		ret = TestIndex(index);
	else
		ret = true;
	if (ret)
		FIndexCur = index;
	return ret;
}
//------------------------------------------------------------------------------
bool CThreadSRVmultiSelf::Modify()
{
	bool ret = false, flag;	// ���� ��������� ��� ���
	int i = 0;
	long sta;
	StopThreadForModifySRV();
	for(auto it=m_Channels.begin(), ie=m_Channels.end(); it!=ie; ++it, ++i)
	{
		if (it->bEnabled)	// ���� �� �������� �������
		{
			parSRV0.pSRV->IdChan(it->Channel, &sta);
			flag = sta != it->ChannelId;		// ����� ������ ��������?
			ret = ret || flag;
			if (flag)
				it->Channel = parSRV0.pSRV->detectChanNum(it->ChannelId);
			if (it->Channel != -1)
			{
				sta = parSRV0.pSRV->GetStatus(it->Channel);
				it->bExist = (sta != 4) && (sta != 5) && (sta != 7);
				// 4 - ����������� ����� ���; 5 - ����� ������������ ���
				// 7 - ����������� ����� ����������������� �������
			}
			else
				it->bExist = false;
			if (it->bExist)
			{// �������� ����������-�� ������� ������������� �� ����� ������
				float freq = parSRV0.pSRV->Frequency(it->Channel);
				flag = it->freqADC != freq;
				ret = ret || flag;
				if (flag)
				{
					it->freqADC = freq;
					if (it->bAutoNumberCounts)
						it->SetNumberCounts();
				}
			}
		}
	}
	GoOnThreadAfterModifySRV();
	return ret;
}
//------------------------------------------------------------------------------
long CThreadSRVmultiSelf::AddChannel(long chan)
{// ���������� ������ � ������� chan. ������������ ��������:
	// ����������� ����� ���������� �������
	//  0 - ����� �������� �������
	// -1 - ��� ���� ������������ �����
	// -2 - ����� ��������, �� ����� chan - ��� �������������� �����
	long ret;
	MultiChannelSelf mc;
	if (parSRV0.NumberChannels < MAXCHAN)
	{// ���� ������� ������ 128
		m_Channels.push_back(mc);
		FIndexCur = parSRV0.NumberChannels++;
		if ( SetChannelNumber(chan) )
			ret = 0;
		else
			ret = -2;
	}
	else
		ret = -1;
	return ret;
}
//------------------------------------------------------------------------------
float* CThreadSRVmultiSelf::GetDataPointer(int index)
{
	float *pret(nullptr);
	if ( TestIndex(index) )
		pret = m_Channels[index].pData;
	else
		pret = NULL;
	return pret;
}
//------------------------------------------------------------------------------
bool CThreadSRVmultiSelf::StartChannel(int index)
{// ���� ������ �������, �� ���������� ������
	bool ret = TestIndex(index);
	if (ret)	// ���� ������ �������
	{
		MultiChannelSelf *pMChan = &m_Channels[index];
		pMChan->bJob = true;
		pMChan->DataTime.SetStartTime( parSRV0.pSRV->CurrentTime(pMChan->Channel) );
	}
	return ret;	// true ���� ������ � ����� ������ ������� �������
}
//------------------------------------------------------------------------------
bool CThreadSRVmultiSelf::StopChannel(int index)
{// ���� ������ �������, �� � ����� ������ - ����� ����� bJob
	bool ret = TestIndex(index);
	if (ret)
		m_Channels[index].bJob = false;
	return ret;
}
//------------------------------------------------------------------------------
void CThreadSRVmultiSelf::SetTime(double val)
{
	for(auto it=m_Channels.begin(); it!=m_Channels.end(); ++it)
		it->DataTime.SetStartTime( parSRV0.pSRV->CurrentTime(it->Channel) );
}
//------------------------------------------------------------------------------