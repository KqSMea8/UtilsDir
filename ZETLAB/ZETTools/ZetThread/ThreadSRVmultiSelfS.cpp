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
#include <ZetThread\ThreadSRVmultiSelfS.h>
//------------------------------------------------------------------------------
DWORD WINAPI SRV_ThreadMultiSelfS(PVOID pParam)
{// ��������� ���������� ������� �������� ������
	int indexChan(0);
	CodeQuality retFunction(0);
	bool bIsSyncGUI(false);

	// ���������� ��������� -----------------------------------------
	// ����� ��������� ���������� ������-����������� CCustomZetThread
	ZetThreadParam0 *par0 = (ZetThreadParam0*)pParam;
	par0->statusEx = running;
	// ����� ��������� ���������� ������-�������� CCustomThreadSRV
	ZetThreadParamSRV0 *parSRV0 = (ZetThreadParamSRV0*)par0->pParamNext;
	// ����� ��������� ���������� ������� ������ CThreadSRVmulti
	ZetThreadParam1 *parSRV1 = (ZetThreadParam1*)parSRV0->pParamNext;
	// ������ ���������� �������
	std::vector<CHANNEL_STRUCT_READ*> *pChannels = parSRV1->pChannels;

	// ���������, ������������ ��������� ���������� ������� ----------
	ZetThreadParam2 parSend;
	//parSend.index - ����� ����� ����� ������� ��������� �������
	parSend.pChannels = pChannels;
	parSend.pParamNext = par0->pParamFunction;	// ��������� �� ��������� ��� nullptr

	// �������� -------------------------------------------------------
	while ( par0->FlagJob )
	{// ���� �������� ������ -------------------------------------------
		par0->FlagViewStop = false;
		// �������� �� ����-�� Pause -----------------------------------
		par0->statusEx = waitObject;
		WaitForSingleObject(parSRV0->hEventModify, par0->TimeOutSend);
	//	auto retw = WaitForSingleObject(parSRV0->hEventModify, par0->TimeOutSend);
		par0->statusEx = working;
		//---------------------------------------------------------------
		// �������� NumFileUsed ���� ������������ �������
		bool flag = true;	// � ����� ����� ����� NumFileUsed
		if ((pChannels != nullptr) && !pChannels->empty())
		{
			for (auto & it : *pChannels)
			{	// ��������� ����-�� ������� ������
				auto xz = it->channel;
				auto bf = it->bFlags;
				if (it->GetFlag(Flag_Chan_OnOff) &&
					it->GetFlag(Flag_Chan_Exist) &&
					!it->GetFlag(Flag_Change_Chan))
				{// ���� ������� �����
					flag = false;
					if (parSRV0->pSRV->NumFileUsed(it->channel) == -1)
					{// ������ ����������
						::PostMessageW(par0->hWndParent, WM_END_JOB_SRV, 0, 0);
						par0->FlagJob = false;
						break;	// ������ �� ����� � �������� �����
					}
				}
			}
		}
		// ������� ������� ���, ����� NumFileUsed
		if (flag)
			parSRV0->pSRV->NumFileUsed(0);
		//---------------------------------------------------------------
		if (par0->FlagJob && (pChannels != nullptr) && !pChannels->empty())
		{// ����� ��� ����� � ������ ������
			indexChan = 0;
			for (auto & it : *pChannels)
			{// ���� �� ���� �������, �� ������ ������ � ������� �������
				par0->statusEx = working;
				if (par0->FlagStop)	
					break;
				// �������� ������� � ��������� ����� Flag_Chan_Exist
				it->DoChangeStatus();

				if (it->GetFlag(Flag_Chan_OnOff) &&
					it->GetFlag(Flag_Chan_Exist) &&
					!it->GetFlag(Flag_Change_Chan))
				{// ����� �������
					par0->statusEx = dataTime;
					// �������� ����� �� ������ -14 ------------
					double timecur = parSRV0->pSRV->CurrentTime(it->channel);
					double dtime = timecur - it->dataTime.GetTime();
					if (fabs(dtime) > MaxDeltaTimeInSec)
					{// �������� �����! �������� �����
						it->dataTime.SetStartTime(timecur);
					}
					// while ����� ��� ����, ����� �������� �� ������������ ������ �������
					BYTE whileCount(0);
					while (
						par0->FlagJob &&
						!par0->FlagStop &&
							(parSRV0->pSRV->CurrentTime(it->channel) >=
								it->dataTime.GetTime()))
					{
						if(bIsSyncGUI)
						{// ����� GUI �������� ���� ������
							if (!it->GetFlag(Flag_Chan_7000) ||
								(it->GetFlag(Flag_Chan_7000) && (parSRV1->bSupport7000)))
							{
								// �������� ������� �������������
								if (it->GetFlag(Flag_Change_Freq))
									it->SetNumberCounts();

								par0->statusEx = dataRead;
								it->retGetData = parSRV0->pSRV->GetData(
									it->channel, it->decade, it->dataTime.GetTime(),
									it->saArray.size, it->saArray.pData);
								if (it->retGetData < 0)
									ippsSet_32f(std::numeric_limits<float>::quiet_NaN(),
										it->saArray.pData, (int)it->saArray.size);
								
								parSend.index = indexChan;
								par0->statusEx = dataProcessing;
								retFunction = (*par0->Function)(&parSend);	// ���-�� ����� (��������, ...)
								par0->statusEx = working;
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
							it->dataTime.IncrementTime();
						}//if (bIsSyncGUI)
						else
						{// ����� GUI ��������, ���� �����
							par0->statusEx = waitSynshroGUI;
							bIsSyncGUI = WaitForSingleObject(parSRV0->hEventSynchro, par0->TimeOutSend) == WAIT_OBJECT_0;
						}
						if (++whileCount > WhileCountMax)
							break;	// ����� �� while
					}	// while (par0->FlagJob ...)
				}	// if ( it->bEnabled && ...
				++indexChan;
			}	// for(auto it=pChannels->begin(); ...
		}	// if (par0->FlagJob && (pChannels != nullptr) && !pChannels->empty())
		// ���������� ����� while ----------------------------------------------
		if (!par0->FlagStop)					// ������������� ����� �� ����, ��� ����
		{
			par0->statusEx = sleeping;
			Sleep(par0->TimeSleep);			// ���� ������ ���������
		}
		else
		{// ���� ���������� �����
			par0->FlagViewStop = true;
			par0->statusEx = suspend;
			SuspendThread(par0->hThread);// ��������� ������� � ����� while
		}
	}// while ( par0->FlagJob )
	par0->statusEx = ending;
	return 0;
}
//------------------------------------------------------------------------------
CThreadSRVmultiSelfS::CThreadSRVmultiSelfS() : CCustomThreadSRV()
	, m_freqADCMin(50.f)
	, m_constFreqADCminFor_0_1(10.f)
	, m_constFreqADCminFor_1_0(1.f)
	, m_pChannels(nullptr)
{
	parSRV0.pParamNext = &parSRV1;

	parSRV1.pChannels = nullptr;
	parSRV1.bSupport7000 = true;
}
//------------------------------------------------------------------------------
bool CThreadSRVmultiSelfS::Create(
	CString nameThread,			// ��� ������
	HWND hwnd,					// HWND ������������� ����
	CDSRV *psrv, 				// ��������� �� ������
	std::vector<CHANNEL_STRUCT_READ*> * pChannels,	// ��������� �� ��������� �������
	FunctionThread function,	// ����� ��������� �������
	PVOID paramFunction)		// ��������� ��������� �������
{
	bool ret(pChannels != nullptr);
	if (ret)
	{
		m_pChannels = pChannels;
		parSRV1.pChannels = m_pChannels;
		bool ret =
			CCustomThreadSRV::Create(nameThread, hwnd, psrv,
				(int)pChannels->size(), function, paramFunction);
		if (ret)
			ret = CreateZetThread(&SRV_ThreadMultiSelfS);
	}
	return ret;
}
//------------------------------------------------------------------------------
CThreadSRVmultiSelfS::~CThreadSRVmultiSelfS()
{// ���������� ������� ��������� ���������� �����, � ����� ������� ������,
	// ����� �������� ��������, ����� ����� ��������� � ��� �������� ��������
	SynchroFinishThread();

	//if ((m_pChannels != nullptr) && !m_pChannels->empty())
	//{
	//	for (auto & it : *m_pChannels)
	//	{
	//		it->sArray.Clear();
	//	}
	//}
}
//------------------------------------------------------------------------------
void CThreadSRVmultiSelfS::SetMinFreqADC(float freq)
{
	if (freq > 0.f)
		*((float*)&m_freqADCMin) = freq;
}
//------------------------------------------------------------------------------
void CThreadSRVmultiSelfS::SetTime(double val)
{
	if ((m_pChannels != nullptr) && !m_pChannels->empty())
	{
		for (auto & it : *m_pChannels)
		{
			it->dataTime.SetStartTime(parSRV0.pSRV->CurrentTime(it->channel));
		}
	}
}
//------------------------------------------------------------------------------