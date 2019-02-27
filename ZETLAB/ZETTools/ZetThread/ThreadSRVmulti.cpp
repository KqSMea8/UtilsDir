//------------------------------------------------------------------------------
//	���� ThreadSRVmulti.cpp
//	������ �� 08.11.2012
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
//   ���������� ������� SRV_ThreadMulti - ��� ��������� ������� ������.
// Ÿ �������� - ��� ��������� �� ��������� ��������� ����� ����������,
// ������� �������� � ���� ��������� �� ��������� ������� ����������
//   � ������� ������ ������� ����������� ����� � ������� ����������� �������.
// ��� ���������� ������� ��� �����, ��� ������� ����� 7000 - ���.
// �� ��������� ������� ����������� ������ � ��� ������� ������ ����������
// ��������� ���������� �������, ��� ������ ����������� ��������� ���������
// ������ ������. ��� ������� ���������� ��� �������� ���������� �����������
// ������ �� ������. ���� ��� AllData ����� ���� ����� 1 (��� ������ ����������),
// �� ��������� ���������� ����������� ������ � ������ ��������� ����,
// � ����� ����� ��������� ��������� WM_NEW_DATA_SRV.
//------------------------------------------------------------------------------
//		��� ���������� ���� �������� ������ �������� ��������� ���������:
//	WM_END_JOB_SRV - ��� ������� ������, ��������� ��� ����������;
//	WM_NEW_DATA_SRV - ������ �� ������ �� ������� ������� � ����������,
//		��������� ����� ������ � ��� �������� ���������� ���������.
//------------------------------------------------------------------------------
//		������ � ������� �������.
// 
#include "StdAfx.h"
#include "ThreadSRVmulti.h"
//------------------------------------------------------------------------------
DWORD WINAPI SRV_ThreadMulti(PVOID pParam)
{// ��������� ���������� ������� �������� ������
	int indexChan;
	long kodFunction;
	SendToSecGlobFunction parSend;
	bool bTimeZetN,			// ����� ����� ��� ���� ���������� �������
		bTime7000,			// ����� ����� ��� ���� ������� ����� 7000
		bChanZetN,			// ���� ������� ���������� ������
		bChan7000;			// ���� ������� ������ ����� 7000
	// ����� �������, ���� � ���� ��� ��� ����� true
	//  (bEnabled, bJob � bExist)

	// ����� ��������� ���������� ������-����������� CCustomZetThread
	ZetThreadParam0 *par0 = (ZetThreadParam0*)pParam;
	// ����� ��������� ���������� ������-�������� CCustomThreadSRV
	ZetThreadParamSRV0 *parSRV0 = (ZetThreadParamSRV0*)par0->pParamNext;
	// ����� ��������� ���������� ������� ������ CThreadSRVmulti
	ZetThreadParamSRVmulti *par1 = (ZetThreadParamSRVmulti*)parSRV0->pParamNext;
	// ������ �������������
	std::vector<MultiChannel> *pChannels = par1->pChannels;
	// ����� ��������� - �������� ��������� ���������� �������
	parSend.pParamNext = par0->pParamFunction;

	while ( par0->FlagJob )
	{// ���� �������� ������ -------------------------------------------
		par0->FlagViewStop = false;

		bChanZetN = bChan7000 = false;
		bTimeZetN = true;
		bTime7000 = par1->Support7000;
		//---------------------------------------------------------------
		// �������� ����� ���� ������� �������
		for(auto it=pChannels->begin(); it!=pChannels->end(); it++)
		{// ��������� ����-�� ������� ������
			if ( it->bEnabled && it->bJob && it->bExist)
			{// ���� ������� �����
				if ( parSRV0->pSRV->NumFileUsed(it->Channel) == -1 )
				{// ������ ����������
					if (par0->hWndParent)
						::PostMessageW( par0->hWndParent, WM_END_JOB_SRV, 0, 0 );
					par0->FlagJob = false;
					break;	// ������ �� ����� � �������� �����
				}
				if (it->b7000)	// ����� ����� 7000
				{
					if (par1->Support7000)
					{
						bChan7000 = true;
						bTime7000 = bTime7000 &&
							( parSRV0->pSRV->CurrentTime(it->Channel) >=
											par1->Time7000 );
					}
					else	// ��������� 7000 ���, �������
						it->bJob = false;
				}
				else			// ���������� �����
				{
					bChanZetN = true;
					bTimeZetN = bTimeZetN &&
						( parSRV0->pSRV->CurrentTime(it->Channel) >=
											parSRV0->Time );
				}
			}
		}
		// ---------------------------------------------------------
		// ���� ������� ������� ��� ���, �� ������ ����� 0-��� ������,
		// ������� ���� ������
		bool b1 = !bChanZetN;
		bool b2 = par1->Support7000 && !bChan7000;
		if ( b1 || b2 )
		{
			if ( parSRV0->pSRV->NumFileUsed(0) == -1 )
			{// ������ ����������
				if (par0->hWndParent)
					::PostMessageW( par0->hWndParent, WM_END_JOB_SRV, 0, 0 );
				par0->FlagJob = false;
				break;	// ������ �� ����� � �������� �����
			}
			if (b1)
				bTimeZetN = parSRV0->pSRV->CurrentTime(0) >= parSRV0->Time;
			if (b2)
				bTime7000 = parSRV0->pSRV->CurrentTime(0) >= par1->Time7000;
		}
		//--------------------------------------------------------------------
		// ����� �����, ������ � ��������� ������
		if ( (bChanZetN && bTimeZetN) ||
			 (par1->Support7000 && bChan7000 && bTime7000) )
		{
			indexChan = 0;
			for(auto it=pChannels->begin(); it!=pChannels->end(); it++,indexChan++)
			{	// ���� �� ���� �������
				if ( it->bEnabled && it->bJob && it->bExist && it->pData )
				{
					if (it->b7000)
					{
						if (bTime7000 && it->bRead7000)
						{	// ������ ����� 7000 �� ������ ���� ����������,
							// �� ������ �������� ������� ��������� ���
							//  ������� ����� bRead7000
							it->qualityCode = parSRV0->pSRV->GetData( it->Channel, it->Decade,
									par1->Time7000, it->NumberKadrs, it->pData );
							it->TimeData = par1->Time7000;
						}
					}
					else
					{
						if (bTimeZetN)
						{
							it->qualityCode = parSRV0->pSRV->GetData( it->Channel, it->Decade,
								parSRV0->Time, it->NumberKadrs, it->pData );
							it->TimeData = parSRV0->Time;
						}
					}
					// ��� ��������� ��������� ������ ���������� �� �������� QualityCode
					// ����� ����������� �� ��������� �������.
					if (par0->Function)
					{// ��������� ������� ������
						parSend.index = indexChan;
						parSend.pMChan = it._Ptr;
						kodFunction = (*par0->Function)(&parSend);	// ���-�� ����� (��������, ...)
						Quality *pkq = (Quality*)&kodFunction;
						if ( pkq->IsAllData() && par0->hWndParent  )
						{// ������� � ���������� ��������� ������
							if ( parSRV0->hEventSynchro &&	// �������� Synchro
									( WaitForSingleObject(parSRV0->hEventSynchro,
											par0->TimeOutSend) == WAIT_OBJECT_0 ) )
								::PostMessageW( par0->hWndParent, WM_NEW_DATA_SRV,
										(WPARAM)indexChan, (LPARAM)kodFunction );
						}
					}
				}// if ( it->bEnabled && it->bJob && it->bExist && it->pData )
			}// for (it=... - ���� �� ���� �������
			//------------------------------------------------------------
			// ��������� �������
			if (bTimeZetN)
				parSRV0->Time += parSRV0->TimeInterval;
			if (par1->Support7000 && bTime7000)
				par1->Time7000 += par1->TimeInterval7000;
		}// if (  (bChanZetN && bTimeZetN) || (bChan7000 && bTime7000) )
		else	// ����� �� �����, ���� �����	
			Sleep(par0->TimeSleep);	// ������ ��������, ���

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
CThreadSRVmulti::CThreadSRVmulti() : CCustomThreadSRV()
{
	FIndexCur = 0;
	FFreqADCMin = 100;
	m_Channels.clear();
	par1.Time7000 = 0;
	par1.TimeInterval7000 = 1;
	par1.pChannels = &m_Channels;
	parSRV0.pParamNext = &par1;
	par1.Support7000 = false;
}
//------------------------------------------------------------------------------
bool CThreadSRVmulti::Create(
	CString nameThread,			// ��� ������
	HWND hwnd,					// HWND ������������� ����
	CDSRV *psrv,				// ��������� �� ������
	int numberChannels,			// ���-�� �������
	FunctionThread function,	// ����� ��������� �������
	PVOID paramFunction )		// ��������� ��������� �������
{
	MultiChannel mc;
	bool ret = (numberChannels > 0) &&
		(numberChannels <= MAXCHAN) &&
		CCustomThreadSRV::Create(nameThread, hwnd, psrv, numberChannels,
						function, paramFunction);
	if (ret)
	{
		parSRV0.NumberChannels = numberChannels;
		for(int i=0; i<parSRV0.NumberChannels; i++)
			m_Channels.push_back(mc);
		ret = CreateZetThread(&SRV_ThreadMulti);// ������ ���������������� �����
	}
	return ret;
}
//------------------------------------------------------------------------------
CThreadSRVmulti::~CThreadSRVmulti()
{// ���������� ������� ��������� ���������� �����, � ����� ������� ������,
	// ����� �������� ��������, ����� ����� ��������� � ��� �������� ��������
	SynchroFinishThread();
	for(auto it=m_Channels.begin(); it!=m_Channels.end(); ++it)
	{
		if (it->pData)
			delete [] it->pData;
	}
	m_Channels.clear();
}
//------------------------------------------------------------------------------
bool CThreadSRVmulti::CalcNumKadrs(int index, int *pnum, bool testIndex)
{// ������ ���-�� ������ ��� ������ � �������� index.
	int num;
	long max;
	bool ret;
	if (testIndex)
		ret = TestIndex(index);
	else
		ret = true;

	if ( ret && m_Channels[index].bEnabled )
	{// ����� ����� ���������
		if (m_Channels[index].b7000)
		{
			float x = (float)par1.TimeInterval7000 *
				parSRV0.pSRV->Frequency( m_Channels[index].Channel );
			num = int(x);
			if ( x > float(num) )
				num++;
		}
		else
			num = int( (float)parSRV0.TimeInterval *
				parSRV0.pSRV->Frequency( m_Channels[index].Channel ) );
		max = parSRV0.pSRV->BufferSize() / 2;
		if (num > max)
		{
			num = max;
			ret = false;
		}
	}
	else
	{
		num = 0;
		ret = false;
	}
	*pnum = num;
	return ret;
}
//------------------------------------------------------------------------------
bool CThreadSRVmulti::SetChannelNumber(long numChan)
{// ��������� ������ ������ ������� �������� ������
	int num;
	m_Channels[FIndexCur].Channel = numChan;
	bool ret = ( numChan >= 0 ) && ( numChan < parSRV0.pSRV->QuanChan() );
	if (ret)
	{//����� ���� � �������
		ret = parSRV0.pSRV->IdChan(numChan, &m_Channels[FIndexCur].ChannelId)
				== 0;	// ID � ������ �������� ���������
		m_Channels[FIndexCur].bJob =
		m_Channels[FIndexCur].bEnabled = ret;
		if (ret)
		{
			long status = parSRV0.pSRV->GetStatus(numChan);
			m_Channels[FIndexCur].bExist = (status != 4) && (status != 5) && (status != 7);
			// 4 - ����������� ����� ���; 5 - ����� ������������ ���
			// 7 - ����������� ����� ����������������� �������
			if ( parSRV0.pSRV->Frequency(numChan) < FFreqADCMin )
				m_Channels[FIndexCur].b7000 = true;
			else
				m_Channels[FIndexCur].b7000 = false;
			CalcNumKadrs(FIndexCur, &num, false);
			if ( (num > 0) && ( m_Channels[FIndexCur].NumberKadrs != num ) )
			{
				if (m_Channels[FIndexCur].pData)
					delete [] m_Channels[FIndexCur].pData;
				m_Channels[FIndexCur].pData = new float [num];
				ippsZero_32f(m_Channels[FIndexCur].pData, num);
				m_Channels[FIndexCur].NumberKadrs = num;
			}
		}
		else
		{
			m_Channels[FIndexCur].bExist = false;
			m_Channels[FIndexCur].ChannelId = -1;
		}
	}
	else
	{
		m_Channels[FIndexCur].bJob =
		m_Channels[FIndexCur].bExist =
		m_Channels[FIndexCur].bEnabled = false;
		m_Channels[FIndexCur].ChannelId = -1;
	}
	return ret;	// �������� ����� ���������� ��� ���
}
//------------------------------------------------------------------------------
bool CThreadSRVmulti::SetIndexCur(int index, bool test)
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
void CThreadSRVmulti::SetTimeInterval(double val)
{
	int nNew;
	if ( val > 0.)
	{
		double interval = int( (val + 0.05) / 0.1 ) * 0.1;
		if ( parSRV0.TimeInterval != interval ) //���������� ��-�� ����������� ������ ��������� 2014.06.23 pvs studio
//		if (IsNotEqual (parSRV0.TimeInterval, interval, 0.0001)) ���������� ������
		{
			parSRV0.TimeInterval = interval;
			int i=0;
			for(auto it=m_Channels.begin(); it!=m_Channels.end(); it++,i++)
			{
				if ( it->bEnabled && it->bExist && !it->b7000 )
				{
					CalcNumKadrs(i, &nNew, false);
					if ( it->NumberKadrs != nNew )
					{
						if (it->pData)
							delete [] it->pData;
						it->pData = new float [nNew];
						it->NumberKadrs = nNew;
					}
				}
			}
		}
	}
}
//------------------------------------------------------------------------------
void CThreadSRVmulti::SetTimeInterval7000(double val)
{
	int nNew;
	if ( val > 0.)
	{
		double interval = int(val + 0.05);
		if ( par1.TimeInterval7000 != interval ) //���������� ��-�� ����������� ������ ��������� 2014.06.23 pvs studio
//		if ( IsNotEqual (par1.TimeInterval7000, interval, 0.0001) )//���������� ������
		{
			par1.TimeInterval7000 = interval;
			int i=0;
			for(auto it=m_Channels.begin(); it!=m_Channels.end(); it++,i++)
			{
				if ( it->bEnabled && it->bExist && it->b7000 )
				{
					CalcNumKadrs(i, &nNew, false);
					if ( it->NumberKadrs != nNew )
					{
						if (it->pData)
							delete [] it->pData;
						it->pData = new float [nNew];
						it->NumberKadrs = nNew;
					}
				}
			}
		}
	}
}
//------------------------------------------------------------------------------
long CThreadSRVmulti::AddChannel(long chan)
{// ���������� ������ � ������� chan. ������������ ��������:
// ����������� ����� ���������� �������
//  0 - ����� �������� �������
// -1 - ��� ���� ������������ �����
// -2 - ����� ��������, �� ����� chan - ��� �������������� �����
	long ret;
	static MultiChannel mc;
	if (parSRV0.NumberChannels < MAXCHAN)
	{// ���� ������� ������ 128
		m_Channels.push_back(mc);
		FIndexCur = parSRV0.NumberChannels++;
		m_Channels[FIndexCur].bEnabled = true;
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
float* CThreadSRVmulti::GetDataPointer(int index)
{
	float *pret;
	bool flag = TestIndex(index);
	if (flag)
		pret = m_Channels[index].pData;
	else
		pret = NULL;
	return pret;
}
//------------------------------------------------------------------------------
void CThreadSRVmulti::Modify()
{
	int i = 0, num;
	long sta;
	for(auto it=m_Channels.begin(); it!=m_Channels.end(); it++, i++)
	{
		if (it->bEnabled)	// ���� �� ������������� �������
		{// ������: 4 - ����������� ����� ���, 5 - ����� ������������ ���
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
				CalcNumKadrs(i, &num, false);
				if ( it->NumberKadrs != num)
				{
					if (it->pData)
						delete [] it->pData;
					it->pData = new float [num];
					it->NumberKadrs = num;
				}
			}
		}
	}
}
//------------------------------------------------------------------------------
bool CThreadSRVmulti::StartChannel(int index)
{// ���� ������ �������, �� ���������� ������
	bool ret = TestIndex(index);
	if (ret)	// ���� ������ �������
		m_Channels[index].bJob = true;
	return ret;	// true ���� ������ � ����� ������ ������� �������
}
//------------------------------------------------------------------------------
bool CThreadSRVmulti::StopChannel(int index)
{// ���� ������ �������, �� � ����� ������ - ����� ����� bJob
	bool ret = TestIndex(index);
	if (ret)
		m_Channels[index].bJob = false;
	return ret;
}
//------------------------------------------------------------------------------
double CThreadSRVmulti::SetCurTime7000(long chan, long fast)
{
	long ch;
	double dt;
	switch(fast)
	{
//	case 0: dt = 10.; break;
	case 1: dt = 1.; break;
	case 2: dt = 0.1; break;
	default : dt = 10.;
	}
	if ( (chan >= 0) && (chan < parSRV0.pSRV->QuanChan() ) )
		ch = chan;
	else
		ch = 0;
	par1.Time7000 =
			int( parSRV0.pSRV->CurrentTime(chan) * dt + 0.5 ) / dt;
	return par1.Time7000;
}
//------------------------------------------------------------------------------
void CThreadSRVmulti::SetNewTime(long chan, long fast)
{
	CCustomThreadSRV::SetNewTime(chan, fast);
	par1.Time7000 = par1.TimeInterval7000 + SetCurTime7000(chan, fast);
}
//------------------------------------------------------------------------------
void CThreadSRVmulti::ZeroTime()
{
	CCustomThreadSRV::ZeroTime();
	par1.Time7000 = 0.;
}
//------------------------------------------------------------------------------
void CThreadSRVmulti::SetTime(double val)
{
	CCustomThreadSRV::SetTime(val);
	par1.Time7000 = val;
}
//------------------------------------------------------------------------------