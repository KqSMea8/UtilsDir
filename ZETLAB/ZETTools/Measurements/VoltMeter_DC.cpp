//------------------------------------------------------------------------------
//		����� �VoltMeter_DC - "��������� ����������� ����".
// ������ ����� ��� �� ����: ��� �����, ���� ������� �������������, ���
// ����������. ����� ������� ��������� ���� �� �����.
//
//		������ � �������� ��������.
// � ������ ������������ ��� ������ ����� 0,1 ��� (� ���� �������
// MultiChannelSelf.DataTime.m_averageTimeCode=0), � ��������� �����������
// � ������������ � MultiParamSelf.fast ������� ������.
//
//		������ � �������� ����� 7000 - �� �������� � MultiChannelSelf.DataTime.
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <Intel\ipp\ippcore.h>
#include "VoltMeter_DC.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//------------------------------------------------------------------------------
UINT ThreadShield(LPVOID pParam)
{
	char lerr = 0;
	try 
	{
		while( Shield(1, &lerr) < 0 )
			Sleep(100);
	}
	catch (char *str)
	{
		UNREFERENCED_PARAMETER(str);
	}

	if ( lerr & 1 )
	{
		Sleep( 20 * abs(lerr) );
		::PostMessage(*((HWND*)pParam), WM_CLOSE, 0, 0);
	}
	return 0;
}
//------------------------------------------------------------------------------
//			������ �������� � ���. �������������� �����.
//		���������� ������
// ������� �������� �������� ����� ������. ���� ������ �������, ��������������
// ������������� ����������. �� ����������� ��������� ������ �� �������������
// ����������� �������������� �������������.
//		������ ����� 7000
// ������� ���������� ������ (���� ��� �� ���� ������). ��� ������� ��-�� ����,
// ��� � ���� ������� ����� ���� �� ����� ������� ������������� (��������:
// 4,17 ��). ������� ������ ������ ������ �� ����� MultiParam.bRead7000. ����
// ���� ��������������� �� ��������� ������� (������� � � ���� ������ ��������)
// ��� �������, ��� MultiParam.numberCur = MultiParam.number - 1. �.�. ���
// ���������� ������. ���� ������ - ��� 1 ��� ��� ����� 7000. ������������ ����
// ������ ������ ���� �� ����� �������������� ��� ����� ������ ������� ����������.
// ��� ���� ��������� ������ ����� �������������, �� ��� �������� �� �����
// ������� �������������. ���� ��������� ������ �� �����.
//-----------------------------
long GlobalFunctionMultiSelfS(PVOID pParam)
{	// ��������� ��������� �� ��������� ������, � �������� ��������� ������
	ZetThreadParam2 *ppar = (ZetThreadParam2*)pParam;
	std::vector<CHANNEL_STRUCT_READ_CALC*> * pList =
		(std::vector<CHANNEL_STRUCT_READ_CALC*> *)ppar->pChannels;
	CHANNEL_STRUCT_READ_CALC *pm = *(pList->begin() + ppar->index);
	
	pm->qualityAll.AllData = Quality_AllData_False;

	if (pm->GetFlag(Flag_Change_Level))
		pm->Init();

	if ((pm->GetFlag(Flag_Change_Freq)) ||
		(pm->GetFlag(Flag_Change_Level_DC)) )
		pm->ReStart();
	
	pm->ResetFlag(Flag_Change_Level);
//	pm->ResetFlag(Flag_Change_Freq); - � ����� �������
	pm->ResetFlag(Flag_Change_Level_DC);

	pm->time = pm->dataTime.GetTime();

	if (pm->GetFlag(Flag_Chan_7000))
	{// ����� ����� 7000 ---------------------------------------
		// ���� ������ ------------------------
		pm->qualityAll = 0;
		if (pm->retGetData == 0)
		{// ������� ������
			if (ippsMean_32f(pm->saArray.pData, (int)pm->saArray.size, &pm->mean, ippAlgHintNone)
				!= ippStsNoErr)
			{
			//	pm->mean = std::numeric_limits<float>::quiet_NaN();
				pm->mean = NAN;
				pm->qualityAll.Error = 1;
			}
		}
		else if (pm->retGetData == 1)
		{// � ������ ���� NaN, ���������� �� ���
			int n(0);
			pm->mean = 0.f;
			for (UINT i = 0; i < pm->saArray.size; ++i)
			{
				float x = pm->saArray.pData[i];
				if (_isnan(x) == 0)
				{
					pm->mean += x;
					++n;
				}
			}
			if (n > 0)
			{
				pm->mean /= n;
			}
			else
			{
				pm->mean = std::numeric_limits<float>::quiet_NaN();
				pm->qualityAll.Error = 1;
			}
		}
		else
		{// ������ ������
			pm->mean = std::numeric_limits<float>::quiet_NaN();
			pm->qualityAll.Error = 1;
		}
		// ���������� -------------------------------
		pm->nPageCur = 0;
		pm->qualityAll.AllData = Quality_AllData_True;
	}
	else
	{// ������� ����� ------------------------------------------
		// ������� -----------------------------------
		CodeQuality qual(0);
		if (pm->retGetData != 0)
			qual.Error = 1;
		if (qual.Error == 0)
		{
			if (ippsMean_32f(pm->saArray.pData, (int)pm->saArray.size,
				pm->meanCur + pm->nPageCur, ippAlgHintNone)
							== ippStsNoErr)
			{
				pm->counterNotNaN[pm->nPageCur] = 1;
			}
			else
			{
				pm->counterNotNaN[pm->nPageCur] = 0;
			}
		}
		else
			pm->counterNotNaN[pm->nPageCur] = 0;

		pm->qualityCur[pm->nPageCur] = qual;

		// �������� ���������� ------------------------------------------
		++pm->nPageCur;
		switch (pm->fast)
		{
		case 0:		// ���������� 0.1
			pm->nPageCur = 0;
			pm->qualityAll = pm->qualityCur[0];
			pm->qualityAll.AllData = Quality_AllData_True;
			pm->mean = pm->meanCur[0];
			break;
		case 1:		// ���������� 1.0 ���
			if (pm->nPageCur >= pm->nPageMax)
			{
				pm->mean = 0.f;
				int m(0);
				pm->qualityAll = 0;
				for (int i = 0; i < pm->nPageMax; ++i)
				{
					pm->qualityAll |= pm->qualityCur[i];
					int n = pm->counterNotNaN[i];
					if (n > 0)
					{
						pm->mean += pm->meanCur[i];
						m += n;
					}
				}
				if (m > 0)
					pm->mean /= m;
				else
					pm->mean = std::numeric_limits<float>::quiet_NaN();

				pm->nPageCur = 0;
				pm->qualityAll.AllData = Quality_AllData_True;
			}
			else
				pm->qualityAll.AllData = Quality_AllData_False;
			break;
		case 2:		// ���������� 10 ���
			if (pm->nPageCur >= pm->nPageMax)
			{
				pm->mean = 0.f;
				int m(0);
				pm->qualityAll = 0;
				for (int i = 0; i < pm->nPageMax; ++i)
				{
					pm->qualityAll |= pm->qualityCur[i];
					int n = pm->counterNotNaN[i];
					if (n > 0)
					{
						pm->mean += pm->meanCur[i];
						m += n;
					}
				}
				if (m > 0)
					pm->mean /= m;
				else
					pm->mean = std::numeric_limits<float>::quiet_NaN();
				// ���������� ����������� ��������
				UINT size = 90 * sizeof(float);
				memmove_s(pm->meanCur, size, pm->meanCur + 10, size);

				size = 90 * sizeof(int);
				memmove_s(pm->counterNotNaN, size, pm->counterNotNaN + 10, size);

				size = 90 * sizeof(CodeQuality);
				memmove_s(pm->qualityCur, size, pm->qualityCur + 10, size);

				//for (int i = 0; i < 90; ++i)
				//{
				//	int k = i + 10;
				//	pm->meanCur[i] = pm->meanCur[k];
				//	pm->counterNotNaN[i] = pm->counterNotNaN[k];
				//	pm->qualityCur[i] = pm->qualityCur[k];
				//}
				pm->nPageCur = 90;
				pm->qualityAll.AllData = Quality_AllData_True;
			}
			else
				pm->qualityAll.AllData = Quality_AllData_False;
			break;
		}
	}
	// ������� ���������. ��� ������ ��� �� �����
	if (pm->qualityAll.AllData == Quality_AllData_True)
	{
		pm->NewValue(pm->mean);
	}

	// ��������� �������. �����, � ����� �������
	if (pm->GetFlag(Flag_Change_Freq))
	{
		pm->SetNumberCounts();
		pm->ResetFlag(Flag_Change_Freq);
	}
	return pm->qualityAll;
}
//------------------------------------------------------------------------------
CVoltMeter_DC::CVoltMeter_DC( UINT nIDTemplate, CWnd* pParent, CString exeName,
	CString cfgFilePreamble, ULONG nZPriority)
	: VoltMeterBase(
	nIDTemplate,		// ������������� ���������
	pParent,			// ��������� �� ���������
	exeName,			// ��� exe-����� ���������
	cfgFilePreamble,	// ��� ��������� ����������������� �����
	nZPriority)			// ��������� ��������� � Zet-������ (�� 0 �� 3)
//	true,				// ������������� ������� ������
//	0, 					// ���-�� ����������� ����������� �������
//	false,				// ������������� ���
//	0 )					// ����� ������������� ���
{
	m_bPlacementCFG = true;
	pSaveData = pSaveDataNew = &m_sParam;	// ��������� �� ��������� Save
	m_sizeSaveData = sizeof(SaveStructure);
	ZeroMemory(pSaveData, m_sizeSaveData);
	m_sParam.SetName(m_sExeName + L".exe");
	m_sParam.size = sizeof(m_sParam);
	m_sCommentAndOrder = TranslateString(L"��������� ����������� ����");

	ippInit();
}
//------------------------------------------------------------------------------
void CVoltMeter_DC::OnDestroy()
{
	KillTimer(1);	// �� ����� ��������

	// �������� ��������� ���������
	if (m_pThread)
	{
		delete m_pThread;
		m_pThread = NULL;
	}

	for (auto & it : m_vectParam)
	{
		delete it;
	}

	VoltMeterBase::OnDestroy();
}
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CVoltMeter_DC, VoltMeterBase)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_TIMER()

	ON_MESSAGE(WM_NEW_DATA_SRV, OnSetUpdate)
END_MESSAGE_MAP()
//------------------------------------------------------------------------------
BOOL CVoltMeter_DC::OnInitDialog()
{
	VoltMeterBase::OnInitDialog();

	m_sSec = TranslateString(L"�");
	m_sFast_0_1 = TranslateString(L"������") + L" 0.1 " + m_sSec;
	m_sFast_1_0 = TranslateString(L"��������") + L" 1 " + m_sSec;
	m_sFast_10 = TranslateString(L"�������������") + L" 10 " + m_sSec;
	m_sAbsent = TranslateString(L"�����������");
	m_sDisabled = TranslateString(L"��������");

	m_nMaxForDecada0 = m_psrv->DecadeBufferSize(0) / 2;
	
	// ������� ������ ����� �������
	m_pThread = new CThreadSRVmultiSelfS();
	m_pThread->Create(L"", m_hWnd, m_psrv,
		(std::vector<CHANNEL_STRUCT_READ*>*)&m_vectParam,
		&GlobalFunctionMultiSelfS);
	m_pThread->SetSupport7000(true);

	// ����� ������� AddChannelByNumber (������ ��������� ������� ������)
	long chan(0);
	if (m_bJobManual || m_bJobPanel)
		chan = m_psrv->detectChanNum(m_sParam.id);
	AddChannelByNumber(chan, false, true);

	// ����� ��������� �����
	m_pThread->Start();

	//------------------------------------------------
	AfxBeginThread(ThreadShield, &m_hWnd);
	return TRUE;  // return TRUE unless you set the focus to a control
}
//------------------------------------------------------------------------------
void CVoltMeter_DC::SetStartTime(CHANNEL_STRUCT_READ_CALC * ps)
{
	m_psrv->NumFileUsed(ps->channel);
	double time = m_psrv->CurrentTime(ps->channel);
	if (!ps->GetFlag(Flag_Chan_7000) && (ps->fast == 2))
	{// � ���� ������ � dt � aver = 0.1 c. ��������� ����������
		if (time > 10.)
			time -= 10.;
	}
	ps->dataTime.SetStartTime(time);

	if ((ps->dataTime.GetAverageCode() == 0) && (ps->fast != 0))
		ps->dataTime.IncrementTime();	// 0.1 c

	ps->nPageCur = 0;
	ps->vrp.ReStart();
}
//------------------------------------------------------------------------------
void CVoltMeter_DC::AddChannelByGUID(GUID id, BOOL bValueCur, bool bChangeInWin)
{
	AddChannelByNumber(m_psrv->detectChanNum(id), bValueCur, bChangeInWin);
}
//------------------------------------------------------------------------------
void CVoltMeter_DC::AddChannelByNumber(long channel, BOOL bValueCur, bool bChangeInWin)
{
	m_vectParam.push_back(nullptr);
	m_indxCur = m_vectParam.size() - 1;
	auto it = m_vectParam.rbegin();
	*it = new CHANNEL_STRUCT_READ_CALC;
	m_pCurParam = *it;
	
//	ZeroMemory(m_pCurParam, sizeof(CHANNEL_STRUCT_READ_CALC));	// ��� ���� it->fast = 0
//	*((uint32_t*)&m_pCurParam->size) = SIZE_CHANNEL_STRUCT;

	// �������� ��������� � CHANNEL_STRUCT_READ
	m_pCurParam->fast = 1;	// �� ��������� 1 ���
	m_pCurParam->decade = 0;
	m_pCurParam->numKadrMax = m_nMaxForDecada0;
	m_pCurParam->dataTime.SetMode10sec(m10s_every_1sec);
	
	if ((0 <= channel) && (channel < m_psrv->QuanChan()))
	{
		m_pCurParam->channel = channel;
		m_psrv->IdChan(channel, &m_pCurParam->id);
		// �������� ��������� � CHANNEL_STRUCT
		m_psrv->GetChannelPar(channel, (BYTE*)m_pCurParam, SIZE_CHANNEL_STRUCT);

		// ������� ���� ��������� �� 7000
		m_pCurParam->SetFlag(Flag_Chan_7000,
			m_pCurParam->frequency <= m_pThread->GetMinFreqADC());

		// ��������� �� CFG, ����� ������. ����� ������ fast
		if (bValueCur)
			m_pCurParam->SetFast(m_curFast);		// �� ���������
		else
			m_pCurParam->SetFast(m_sParam.fast);	// �� CFG

		// ��������� �������
		if (m_pCurParam->GetFlag(Flag_Chan_7000))
		{
			m_pCurParam->dataTime.SetCalcNumberMode(true);
			m_pCurParam->dataTime.SetAverageCodeMode(m_pCurParam->fast, m_pCurParam->frequency);
		}
		else
		{
			m_pCurParam->dataTime.SetCalcNumberMode(false);
			m_pCurParam->dataTime.SetAverageCodeMode(0);	// ����� ������ �������� �� 0,1 ���
		}

		SetStartTime(m_pCurParam);
		m_pCurParam->Init();

		SetFlagsForNewChanOk();

		if (bChangeInWin)
			ViewNewChan_iw(true);
	}
	else
	{
		m_pCurParam->ResetFlag(Flag_Chan_Exist);	// ����� �� ����������
		m_pCurParam->channel = -1;
		m_pCurParam->name[0] = 0;
		m_pCurParam->measure[0] = 0;
		if (bChangeInWin)
			ViewTitle_iw(true);
	}
}
//------------------------------------------------------------------------------
void CVoltMeter_DC::SetFlagsForNewChanOk()
{
	m_pCurParam->PlaceFlag(Flag_Chan_OnOff);
	m_pCurParam->PlaceFlag(Flag_Chan_Exist);

	m_pCurParam->PlaceFlag(Flag_Change_Status);
	m_pCurParam->PlaceFlag(Flag_Change_Freq);
	m_pCurParam->PlaceFlag(Flag_Change_Level);
	m_pCurParam->PlaceFlag(Flag_Change_Level_DC);
}
//------------------------------------------------------------------------------
void CVoltMeter_DC::OnStructureDefault()
{
	long m(0);
	GetDefaultChannel(m_nOrder, m, m_sParam.id);
	m_sParam.fast = 1;
	m_sParam.GetPlacement(m_hWnd);
}
//------------------------------------------------------------------------------
void CVoltMeter_DC::OnStructureNew()
{
	New_channel();
	New_fast();
}
//------------------------------------------------------------------------------
void CVoltMeter_DC::ViewTitle_iw(bool bClearMean)
{
	Set_Title_iw(m_pCurParam);
	Set_Channel_iw(m_pCurParam->channel);
	if (bClearMean)
		ResetOutData_iw();
}
//------------------------------------------------------------------------------
void CVoltMeter_DC::ViewNewChan_iw(bool bClearMean)
{
	ViewTitle_iw(bClearMean);

	Set_Fast_iw(m_pCurParam->fast);
	Set_NumFast_iw(m_pCurParam->numFast);
	SetDigitalNumber_iw(m_pCurParam);
}
//------------------------------------------------------------------------------
LRESULT CVoltMeter_DC::OnSetUpdate(WPARAM wParam, LPARAM lParam)
{
	if (m_pThread == NULL)
		return -1;

	DWORD index = (DWORD)(int)wParam;	// ������ ������, �� �������� �������� ������
	long quality = (long)lParam;

	CHANNEL_STRUCT_READ_CALC *pm = *(m_vectParam.begin() + index);

	// ����������� ����������� ---------------------------------------------
	// � m_strMean ��������� ������ � ������� PreTranslateMessage
	float mean = pm->mean;
	m_strMean = pm->vrp.m_param.sValueCur;
	double time = pm->time;
		
	m_pThread->SetSynchro();		// ��������� ������� �����

	// ����������� ---------------------------------------------------------
	if (index == m_indxCur)
	{
		Set_HorScale_iw(m_psrv->CurLevel(m_pCurParam->channel));
		Set_Mean_iw(m_strMean);
		Quality_iw(quality);
	}

	//CString s;
	//s.Format(L"VoltMeterDC: indx = %d, time = %.1f, timeChan = %.1f, fast = %d, size = %d, nPageNax = %d, T_dt = %.2f, T_aver = %.2f\n",
	//	index, time, m_psrv->CurrentTime(pm->channel),
	//	pm->fast, pm->sArray.size, pm->nPageMax,
	//	pm->dataTime.GetTimeInterval(), pm->dataTime.GetTimeAverage());
	//OutputDebugStringW(s);

	// ��������� � UNIT ----------------------------------------------------
	if (m_bJobUnit)
	{
		long ret = m_punitParent->UnitWrite(1, &mean, index);

		//CString s;
		//s.Format(L"VoltMeterDC: UnitWrite-1: time = %.2f, index = %d, mean = %f\n",
		//	time, index, mean);
		//OutputDebugStringW(s);
	}
	// ��������� � UNIT-2 --------------------------------------------------
	else if (m_bJobUnit2)
	{
		//CString s;
		//s.Format(L"VoltMeterDC, data to UNIT-2: time = %.2f, index = %d, value = %f\n",
		//	time, index, mean);
		//OutputDebugStringW(s);

	//	��������� - �����
	//	WriteUnitResult_p0(index, m_mean); - ������ �� ������������

		// ��������� - ������� ��������� �����
		WideResult<float> wrsF;
		wrsF.quality = quality;
		wrsF.time = time;
		wrsF.value = mean;
		// * 100 - �.�. �������������� ���������
		WriteUnitResult_p1(index * 100, &wrsF);
	}
	return 0;
}
//------------------------------------------------------------------------------
void CVoltMeter_DC::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{// ������ ����� ���������� �� UNIT
		KillTimer(1);
		OnModifySRV();		// ���������� ���������� �������
	}
	else
		VoltMeterBase::OnTimer(nIDEvent);
}
//------------------------------------------------------------------------------
BOOL CVoltMeter_DC::PreTranslateMessage(MSG* pMsg)
{
	if ( (pMsg->message == WM_CHAR) && ( (int)pMsg->wParam == 3) )
		CopyStringToClipboard(m_strMean);	// ��� 3 - ��� "Ctrl + C"
	return VoltMeterBase::PreTranslateMessage(pMsg);
}
//------------------------------------------------------------------------------
void CVoltMeter_DC::StopAllThread()
{	// ��������� ������������ ����� �������
	m_pThread->StopAndWait();
}
//------------------------------------------------------------------------------
void CVoltMeter_DC::ReStart()
{
	OnSetTimeSRV(0);
	for (auto & it : m_vectParam)
	{
		it->ReStart();
		SetStartTime(it);
	}
	m_pThread->Start();
}
//------------------------------------------------------------------------------
// ����� ����� �� m_sParam
void CVoltMeter_DC::New_channel()
{
	if (m_pCurParam->id == m_sParam.id)
		return;

	// ������ ������
	m_pCurParam->id = m_sParam.id;
	m_pCurParam->channel = m_psrv->detectChanNum(m_sParam.id);

	if (m_pCurParam->channel != -1)
	{
		m_psrv->GetChannelPar(m_pCurParam->channel, (BYTE*)m_pCurParam, SIZE_CHANNEL_STRUCT);
		
		// ������� ���� ��������� �� 7000
		m_pCurParam->SetFlag(Flag_Chan_7000,
			m_pCurParam->frequency <= m_pThread->GetMinFreqADC());

		// ����� ������������ ��������� �� CFG
		m_pCurParam->SetFast(m_pCurParam->fast);

		// ������ ��������� ���������
		if (m_pCurParam->GetFlag(Flag_Chan_7000))
		{
			m_pCurParam->dataTime.SetCalcNumberMode(true);
			m_pCurParam->dataTime.SetAverageCodeMode(m_pCurParam->fast, m_pCurParam->frequency);
		}
		else
		{
			m_pCurParam->dataTime.SetCalcNumberMode(false);
			m_pCurParam->dataTime.SetAverageCodeMode(0);
		}
		// ����� ������
		SetStartTime(m_pCurParam);

		m_pCurParam->Init();

		SetFlagsForNewChanOk();

		ViewNewChan_iw(true);
	}
	else
	{
		m_pCurParam->ResetFlag(Flag_Chan_Exist);
		m_pCurParam->name[0] = 0;
		m_pCurParam->measure[0] = 0;
		ViewTitle_iw(true);
	}
}
//------------------------------------------------------------------------------
// ����� �������� �� m_sParam
void CVoltMeter_DC::New_fast()
{//����������
	auto old = m_pCurParam->fast;
	m_pCurParam->SetFast(m_sParam.fast);

	// ������ ��������� ���������
	if (m_pCurParam->GetFlag(Flag_Chan_7000))
	{
		m_pCurParam->dataTime.SetCalcNumberMode(true);
		m_pCurParam->dataTime.SetAverageCodeMode(m_pCurParam->fast, m_pCurParam->frequency);
	}
	else
	{
		m_pCurParam->dataTime.SetCalcNumberMode(false);
		m_pCurParam->dataTime.SetAverageCodeMode(0);
	}
	if (old != m_pCurParam->fast)
	{
		m_pCurParam->Init();
		SetStartTime(m_pCurParam);
	}
	// �������� �������� �� ����� ����� �����
	m_pCurParam->PlaceFlag(Flag_Change_Freq);

	Set_Fast_iw(m_pCurParam->fast);
	Set_NumFast_iw(m_pCurParam->numFast);
}
//------------------------------------------------------------------------------
void CVoltMeter_DC::OnSetTimeSRV(long GlobalPar)
{
	Sleep(100);
	for (auto & it : m_vectParam)
	{
		if (it->GetFlag(Flag_Chan_Exist) &&
			it->GetFlag(Flag_Chan_OnOff))
		{
			m_psrv->NumFileUsed(it->channel);
			SetStartTime(it);
		}
	}
}
//------------------------------------------------------------------------------
void CVoltMeter_DC::OnFileModeSRV(long GlobalPar)
{// 
	StopAllThread();
	OnModifySRV();		// ���� �������� ���� �������
	ReStart();
}
//------------------------------------------------------------------------------
void CVoltMeter_DC::OnStartFileSRV(long GlobalPar)
{
	if (GlobalPar == 0)
		OnSetTimeSRV(0);

	for (auto & it : m_vectParam)
	{
		it->ReStart();
	}
}
//------------------------------------------------------------------------------
void CVoltMeter_DC::OnModifySRV()
{// ��������� Modify ��� ������ ���� �������� ���������� � �������

	for (auto & it : m_vectParam)
	{
		it->channel = m_psrv->detectChanNum(it->id);
		if (it->channel != -1)
		{
			CHANNEL_STRUCT cs = *it;
			it->PlaceFlag(Flag_Chan_OnOff);
			it->PlaceFlag(Flag_Chan_Exist);
			m_psrv->GetChannelParEx(it->channel, it);
			it->TestChangeStruct(cs);
			if (it->GetFlag(Flag_Change_Freq))
			{// ������� ���� ��������� �� 7000
				it->SetFlag(Flag_Chan_7000,
					it->frequency <= m_pThread->GetMinFreqADC());
				// ����� ������������ ��������� �� CFG
				it->SetFast(it->fast);
				// ������ ��������� ���������
				if (it->GetFlag(Flag_Chan_7000))
				{
					it->dataTime.SetCalcNumberMode(true);
					it->dataTime.SetAverageCodeMode(it->fast, it->frequency);
				}
				else
				{
					it->dataTime.SetCalcNumberMode(false);
					it->dataTime.SetAverageCodeMode(0);
				}
				// ����� ������
				SetStartTime(it);

				it->Init();
			}
		}
		else
		{
			it->ResetFlag(Flag_Chan_Exist);
			it->name[0] = 0;
			it->measure[0] = 0;
		}
	}

	View_NewChan_iw();		// ����� �������� ������� SRV, �� ��� ����������
	// ����� ���-�� ���������� � �������� ������
	if (m_pCurParam->channel == -1)
		Set_ChannelOff_iw(0);
	else
	{
		long status = m_pCurParam->status;
		if ((status == 4) ||
			(status == 5) ||
			(status == 7) )
			Set_ChannelOff_iw(1);
		else
			ViewNewChan_iw(!m_pCurParam->GetFlag(Flag_Chan_Exist));
	}
}
//------------------------------------------------------------------------------
void CVoltMeter_DC::OnFirstModifySRV_0()
{// ������ Modify c GlobalPar == 0, ����� ���������� ������
	//StopAllThread();
	//m_pThread->StopThreadForModifySRV();
}
//------------------------------------------------------------------------------
CHANNEL_STRUCT_READ_CALC* CVoltMeter_DC::GetCurParam(int index)
{
	return *(m_vectParam.begin() + index);
}
//------------------------------------------------------------------------------
void CVoltMeter_DC::OnReadyUnitParent(long par)
{
	long param;
	double value;
	m_punitParent->UnitParam(&param, &value);
	long val = (long)value;
	switch(param)
	{
	case 0:	 SetUnitParam_p0(val);	break;
	case 1:	 SetUnitParam_p1(val);	break;
	case -1:	 SetUnitParam_m1(val);	break;
	case -2: SetUnitParam_m2(val);	break;
	case -3: SetUnitParam_m3(val);	break;
	case -4: SetUnitParam_m4(val);	break;
	case -5: SetUnitParam_m5(val);	//break;
	}
}
//------------------------------------------------------------------------------
void CVoltMeter_DC::OnReadyUnit2Param()
{
	long val;
	switch(m_paramUnit2.number)
	{
	case 0:
		if (m_paramUnit2.type == tdu_32s)
			if (m_paramUnit2.Extract_val(val) == NOERROR)
				SetUnitParam_p0(val);
		break;
	case 1:
		if (m_paramUnit2.type == tdu_32s)
			if (m_paramUnit2.Extract_val(val) == NOERROR)
				SetUnitParam_p1(val);
		break;
	case -1:	
		if (m_paramUnit2.type == tdu_32s)
			if (m_paramUnit2.Extract_val(val) == NOERROR)
				SetUnitParam_m1(val);
		break;
	case -2:
		if (m_paramUnit2.type == tdu_32s)
			if (m_paramUnit2.Extract_val(val) == NOERROR)
				SetUnitParam_m2(val);
		break;
	case -3:
		if (m_paramUnit2.type == tdu_32s)
			if (m_paramUnit2.Extract_val(val) == NOERROR)
				SetUnitParam_m3(val);
		break;
	case -4:
		if (m_paramUnit2.type == tdu_32s)
			if (m_paramUnit2.Extract_val(val) == NOERROR)
				SetUnitParam_m4(val);
		break;
	case -5:
		if (m_paramUnit2.type == tdu_32s)
			if (m_paramUnit2.Extract_val(val) == NOERROR)
				SetUnitParam_m5(val);
		//break;
	}
// 	CString s;
// 	s.Format(L"VoltMeterDC, data of UNIT-2: command = %d, param = %d\n", m_paramUnit2.number, val);
// 	OutputDebugStringW(s);
}
//------------------------------------------------------------------------------
void CVoltMeter_DC::SetUnitParam_p0(const long val)
{//��������� ������ �������� ������
	if ((0 <= val) && (val < m_psrv->QuanChan()))
	{
		m_psrv->IdChan(val, &m_sParam.id);
		New_channel();
	}
	else
	{
		m_pCurParam->ResetFlag(Flag_Chan_Exist);
		m_pCurParam->name[0] = 0;
		m_pCurParam->measure[0] = 0;
		m_pCurParam->id = GUID_NULL;
		ViewTitle_iw(true);
	}
	//m_punitSrv->SetParamBack_32s(0, m_pCurParam->Channel);
}
//------------------------------------------------------------------------------
void CVoltMeter_DC::SetUnitParam_p1(const long val)
{//��������� ������, ��������
	m_sParam.fast = (uint32_t)val;
	New_fast();
//	m_punitSrv->SetParamBack_32s(1, m_pmParCur->fast);
}
//------------------------------------------------------------------------------
void CVoltMeter_DC::SetUnitParam_m1(const long val)
{// ������� � �������������� �����. �������������� ������� ���-�� �������
	if ((1 < val) && (val <= MAXCHAN))
	{
		StopAllThread();
		for (long i = 0; i < (val - 1); ++i)
		{
			AddChannelByNumber(-1, true, false);
		}
		// ��������� ������� �����
		m_indxCur = 0;
		m_pCurParam = *m_vectParam.begin();
		ReStart();
	}
//	m_punitSrv->SetParamBack_32s(-1, (long)m_multi.size());
}
//------------------------------------------------------------------------------
void CVoltMeter_DC::SetUnitParam_m2(const long val)
{ // ������� �������� ������� ������ ��� �������� ��������� ����������
	if (m_indxCur != val)
	{
		if (TestIndx(val))
		{
			m_indxCur = val;
			m_pCurParam = GetCurParam(m_indxCur);
			ViewNewChan_iw(true);
		}
	}
	//	m_punitSrv->SetParamBack_32s(-2, m_indxCur );
}
//------------------------------------------------------------------------------
void CVoltMeter_DC::SetUnitParam_m3(const long val)
{ // ��������� ������
	if (TestIndx(val))
	{
		GetCurParam(val)->ResetFlag(Flag_Chan_OnOff);
	//	m_punitSrv->SetParamBack_32s(-3, val);
	}
//	m_punitSrv->SetParamBack_32s(-3, -1);
}
//------------------------------------------------------------------------------
void CVoltMeter_DC::SetUnitParam_m4(const long val)
{// ������ ������
	if (TestIndx(val))
	{
		GetCurParam(val)->PlaceFlag(Flag_Chan_OnOff);
		//	m_punitSrv->SetParamBack_32s(-3, val);
	}
	//	m_punitSrv->SetParamBack_32s(-3, -1);
}
//------------------------------------------------------------------------------
void CVoltMeter_DC::SetUnitParam_m5(const long val)
{// ���������� ������
	if (m_vectParam.size() < MAXCHAN)
	{
		StopAllThread();
		AddChannelByNumber(val, true, true);
		ReStart();
	}
//	m_punitSrv->SetParamBack_32s(-5, -1);
}
//------------------------------------------------------------------------------
//void CVoltMeter_DC::WriteUnitResult_p0(
//	const long shift, const float val)
//{
//	m_punitSrv->WriteResult_32f(shift, val);
//}
//------------------------------------------------------------------------------
void CVoltMeter_DC::WriteUnitResult_p1(
	const long shift, WideResult<float> *pval)
{
	m_punitSrv->WriteResult_wrs_32f(shift + 1, pval);
}
//------------------------------------------------------------------------------