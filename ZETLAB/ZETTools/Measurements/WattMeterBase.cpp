#include "stdafx.h"
#include "WattMeterBase.h"
#include <Intel\ipp\ippcore.h>
#include "CalcPowerFunctions.h"
#include <mathZet.h>

CThreadSRVmultiSelf *g_pThread;
float g_frequency = 0.f;			// вычисленная частота
float g_res[4] = { 0.f };			// массив результатов
CDSRV* g_pServer = nullptr;
int firstchannum = 0;
bool g_IsNaN = false;				// флаг указывающий на наличие NaN во входных данных
int g_endperiod = 0;				// конец последнего периода синусоидального сигнала
int g_startperiod = 0;				// начало первого периода синусоидального сигнала
float *g_massive_f = nullptr;		// массив значений передаваемых в виртуальный канал
vector <float*> g_mass;				// вектор указателей на массива дынных по каналам
int g_size_mass = 0;				// размер массива 
double g_ftime = 0.f;				// время по которому считываются данные по каналам
int g_channels = 0;					// количество каналов
//------------------------------------------------------------------------------
UINT ThreadShield(LPVOID pParam)
{
	char lerr = 0;
	try
	{
		while (Shield(1, &lerr) < 0)
			Sleep(100);
	}
	catch (char *str)
	{
		UNREFERENCED_PARAMETER(str);
	}

	if (lerr & 1)
	{
		Sleep(20 * abs(lerr));
		::PostMessage(*((HWND*)pParam), WM_CLOSE, 0, 0);
	}
	return 0;
}
//------------------------------------------------------------------------------
long GlobalFunctionMultiSelf(PVOID pParam)
{
	SendToSecGlobFunction *ppar = (SendToSecGlobFunction*)pParam;
	MultiChannelSelf *pMChan = ppar->pMChan;
	MultiParamSelf *pm1, *pm2;
	int dc = g_channels - (int)g_mass.size();
	if (dc > 0)
	{
		for (int i = 0; i < dc; ++i)
			g_mass.push_back(nullptr);
	}
	if (ppar->index % 2 != 0)
	{
		pm2 = (((std::vector<MultiParamSelf>*)ppar->pParamNext)->begin() + ppar->index)._Ptr;
		if (g_mass.at(ppar->index) && g_mass.at(ppar->index - 1) && g_massive_f)
		{
			ippsCopy_32f(pMChan->pData, g_mass[(ppar->index)] + pm2->numberPageCur * pMChan->NumberCounts, pMChan->NumberCounts);
			if (++pm2->numberPageCur >= pm2->numberPage)
			{
				pm2->pCodeQuality[pm2->numberPageCur] = pMChan->RetGetData;
				pm2->time = pMChan->DataTime.GetTime();
				int res = 0;
				if (firstchannum == 0)
				{
					int a = 0;
				}
				res = NoiseDetect(g_mass[(ppar->index - 1)], g_mass[(ppar->index)], pm2->numberPage * pMChan->NumberCounts,
					g_pServer->MinLevel(firstchannum), g_pServer->MaxLevel(firstchannum), g_pServer->MinLevel(pMChan->Channel), g_pServer->MaxLevel(pMChan->Channel));
				if (1 == res)
				{
					g_IsNaN = false;
					g_frequency = FreqCalc(g_mass[(ppar->index - 1)], pMChan->NumberCounts * pm2->numberPage, g_pServer->GetFrequency(pMChan->Channel), &g_startperiod, &g_endperiod);
					CalcPower(g_mass[(ppar->index - 1)], g_mass[(ppar->index)], g_startperiod, g_endperiod, g_res);
				}
				else if (0 == res)
				{
					g_IsNaN = false;
					*g_res = 0.f;
					*(g_res + 1) = 0.f;
					*(g_res + 2) = 0.f;
				}
				else if (-1 == res)
				{
					g_IsNaN = true;
				}
				ippsSet_32f(*g_res, g_massive_f, pMChan->NumberCounts);
				g_pServer->PutData(0, pMChan->DataTime.GetTime(), pMChan->NumberCounts, g_massive_f);
				ippsSet_32f(*(g_res + 1), g_massive_f, pMChan->NumberCounts);
				g_pServer->PutData(1, pMChan->DataTime.GetTime(), pMChan->NumberCounts, g_massive_f);
				ippsSet_32f(*(g_res + 2), g_massive_f, pMChan->NumberCounts);
				g_pServer->PutData(2, pMChan->DataTime.GetTime(), pMChan->NumberCounts, g_massive_f);
				g_ftime = pMChan->DataTime.GetTime();
				pm2->numberPageCur = 0;
				pm2->qualityAll.AllData = Quality_AllData_True;	// всё обработано
				return pm2->qualityAll;
			}
		}
		pm2->qualityAll.AllData = Quality_AllData_False;	// данных для обработки не достаточно
		return pm2->qualityAll;
	}
	else
	{
		pm1 = (((std::vector<MultiParamSelf>*)ppar->pParamNext)->begin() + ppar->index)._Ptr; 
		firstchannum = pMChan->Channel;
		if (firstchannum == 0)
		{
			int a = 0;
		}
		if (((int)(pm1->numberPage * pMChan->NumberCounts)) > g_size_mass || !g_mass[ppar->index] && !g_mass[ppar->index + 1])		// если количество точек в массиве превышает имеющееся
		{
			if (g_mass[(ppar->index)])
			{
				delete []g_mass[(ppar->index)];
				g_mass[(ppar->index)] = nullptr;
			}
			if (g_mass[(ppar->index + 1)])
			{
				delete []g_mass[(ppar->index + 1)];
				g_mass[(ppar->index + 1)] = nullptr;
			}
			if (g_massive_f)
			{
				delete []g_massive_f;
				g_massive_f = nullptr;
			}
			g_mass[(ppar->index)] = new float[pMChan->NumberCounts * 10];
			g_mass[(ppar->index + 1)] = new float[pMChan->NumberCounts * 10];
			g_massive_f = new float[pMChan->NumberCounts * 10];
			g_size_mass = 10 * pMChan->NumberCounts;
		}
		if (g_mass[(ppar->index)] && g_mass[(ppar->index + 1)])
			ippsCopy_32f(pMChan->pData, g_mass[(ppar->index)] + pm1->numberPageCur * pMChan->NumberCounts, pMChan->NumberCounts);
		if (++pm1->numberPageCur >= pm1->numberPage)
		{
			pm1->time = pMChan->DataTime.GetTime();
			pm1->pCodeQuality[pm1->numberPageCur] = pMChan->RetGetData;
			pm1->numberPageCur = 0;
			pm1->qualityAll.AllData = Quality_AllData_True;	// всё обработано
			return pm1->qualityAll;
		}
		pm1->qualityAll.AllData = Quality_AllData_False;	// данных для обработки не достаточно
		return pm1->qualityAll;
	}
	return 0;
}
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CWattMeterBase, WattMeter_Base)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_MESSAGE(WM_NEW_DATA_SRV, OnSetUpdate)
END_MESSAGE_MAP()
//------------------------------------------------------------------------------
CWattMeterBase::CWattMeterBase(UINT nIDTemplate, CWnd* pParent, CString exeName, 
	CString cfgFilePreamble, ULONG nZPriority)
	: WattMeter_Base(
	nIDTemplate,														// идентификатор программы
	pParent,															// указатель на владельца
	exeName,															// имя exe-файла программы
	cfgFilePreamble,													// имя преамбулы конфигурационного файла				
	nZPriority)															// приоритет программы в Zet-панели (от 0 до 3)
{
	m_bPlacementCFG = true;
	pSaveData = &m_sParam;												// указатель на структуру Save
	pSaveDataNew = &m_sParamNew;
	m_sizeSaveData = sizeof(SaveStruct);
	ZeroMemory(pSaveData, m_sizeSaveData);
	ZeroMemory(pSaveDataNew, m_sizeSaveData);
	m_sParam.SetName(m_sExeName + L".exe");
	m_sParamNew.SetName(m_sExeName + L".exe");
	m_sParam.size = m_sParamNew.size = sizeof(m_sParam);
	m_IDtimerReStart = 1;
	m_bMulti = false;
	ippInit();
}
//------------------------------------------------------------------------------
CWattMeterBase::~CWattMeterBase()
{

}
//------------------------------------------------------------------------------
LRESULT CWattMeterBase::OnSetUpdate(WPARAM wParam, LPARAM lParam)
{
	if (static_cast<int>(wParam) % 2 != 0)
	{
		if (g_IsNaN)
		{
			SetColor(ColorError);
			SetVal1(L"*****");
			SetVal2(L"*****");
			SetVal3(L"*****");
			SetHorscaleVal(0.f);
		}
		else
		{
			CString strTemp(L""), strTemp1(L"");
			CString /*s1(L""),*/ s2(L"");
			int degree = 0;
			int n = 0;
			float max = 0.f;
			SetColor(ColorOk);
			int index = static_cast<int>(wParam);
			float mass1[3] = { *g_res, *(g_res + 1), *(g_res + 2) };
			float mass2[3] = { abs(*g_res), abs(*(g_res + 1)), abs(*(g_res + 2)) };		// инициализируем массив  
			if (g_pThread)																// если не была выполнена функция OnDestroy
			{
				SetHorscaleVal(m_psrv->CurLevel(m_sParam.channel1));					// задаем значение для индикатора уровня
				if (m_sParam.typeview == 0)
				{
					ResetMera();														// убираем приставки
					SetVal1(DigitalNotation(mass1[0]));									// задаем значение активной мощности
					SetVal2(DigitalNotation(mass1[1]));									// задаем значение полной мощности
					SetVal3(DigitalNotation(mass1[2]));									// задаем значение реактивной мощности
				}
				else
				{
					ResetMera();
					ippsMax_32f(mass2, 3, &max);										// находим максмимальное значение
					NumberFormat(max, degree, n, strTemp1);								// форматируем все значение по максимальному
					s2.Format(L"%i", n);
					strTemp.Format(L"%." + s2 + L"f", *g_res / pow(10, degree));		// приводим все строки к одному виду
					AddMera1(strTemp1);													// добавляем приставку к единице измерения
					SetVal1(strTemp);													// выводим значения мощности
					strTemp.Format(L"%." + s2 + L"f", *(g_res + 1) / pow(10, degree));
					AddMera2(strTemp1);
					SetVal2(strTemp);
					strTemp.Format(L"%." + s2 + L"f", *(g_res + 2) / pow(10, degree));
					AddMera3(strTemp1);
					SetVal3(strTemp);
				}
				if (m_bJobUnit)															// результат в UNIT
				{
					m_punitParent->UnitWrite(3, g_res, index);
				}
				if (m_bJobUnit2)														// результат в UNIT-2
				{																		// результат - число
					ValueStruct a;
					a.P = *g_res;
					a.S = *(g_res + 1);
					a.Q = *(g_res + 2);
					a.time = g_ftime;
					a.quality = static_cast<int>(wParam);
					m_punitSrv->WriteResult_str(index * 100 + 1, sizeof(a), (PVOID)&a);
				}
			}
		}
	}
	if (g_pThread)
		g_pThread->SetSynchro();
	return (LRESULT)0;
}
//------------------------------------------------------------------------------
BOOL CWattMeterBase::OnInitDialog()
{
	WattMeter_Base::OnInitDialog();
	m_sParam.numberProgram = m_sParamNew.numberProgram = m_nOrder;						// номер программы
	g_pServer = m_psrv;																	// передаем указатель на сервер
	long Id = 0;
	if ((m_sParamNew.channel1 < 0) || (m_sParamNew.channel1 > (m_psrv->QuanChan() + 1 + 3)))	// три своих виртуальных
		m_sParamNew.channel1 = 0;
	if ((m_sParamNew.channel2 < 0) || (m_sParamNew.channel2 > (m_psrv->QuanChan() + 1 + 3)))
		m_sParamNew.channel2 = 1;
	m_psrv->IdChan(m_sParam.channel1, &Id);
	if ((m_IdVirtChan1 == Id) || (m_IdVirtChan2 == Id) || (m_IdVirtChan3 == Id))
		m_sParamNew.channel1 = 0;
	m_psrv->IdChan(m_sParam.channel2, &Id);
	if ((m_IdVirtChan1 == Id) || (m_IdVirtChan2 == Id) || (m_IdVirtChan3 == Id))
		m_sParamNew.channel2 = 1;
	g_pThread = new CThreadSRVmultiSelf();
	g_pThread->Create(L"", m_hWnd, m_psrv, 1, &GlobalFunctionMultiSelf, &m_multi);
	g_pThread->SetSupport7000(true);
	g_pThread->SetChannelNumber(m_sParamNew.channel1);

	m_pmChanCur1 = g_pThread->GetPointerMultiChannelCur();
	m_pmChanCur1->DataTime.SetAverageCodeMode(0, m_psrv->Frequency(m_sParamNew.channel1));
	m_multi.resize(2);
	m_pmParCur1 = &m_multi[0];
	m_pmParCur2 = &m_multi[1];
	g_pThread->SetIndexCur(1);
	m_pmChanCur2 = g_pThread->GetPointerMultiChannelCur();
	g_pThread->SetIndexCur(0);
	m_pmChanCur1 = g_pThread->GetPointerMultiChannelCur();
	SetFast(m_sParamNew.fast);
	DefaultMultiParam();
	if (!m_bJobUnit && !m_bJobUnit2)													// если программа запущенна не через Юниты
	{
		m_psrv->Disconnect();										
		m_psrv->ConnectVrtCh(3);														// создаем один виртуальный канал
		SetInfoVrtCahnnel(m_sParamNew.channel1);											// настраеваем виртуальный канал
	}
	SetTitleI(m_sParamNew.channel1, m_sParamNew.channel2);									// выводим названия каналов в шапку программы
	g_pThread->AddChannel(m_sParamNew.channel2);										// добавляем канал
	g_pThread->SetIndexCur(1);															// задаем индекс канала и получаем указатель на структуры
	m_pmChanCur2 = g_pThread->GetPointerMultiChannelCur();
	g_pThread->SetIndexCur(0);
	m_pmChanCur1 = g_pThread->GetPointerMultiChannelCur();

	double dt = 1.;
	if (!m_bMulti && (m_sParamNew.fast == 2))
		dt = 10.;
	double t = m_psrv->CurrentTime(m_sParamNew.channel1);
	if (t > dt)
		t -= dt;
	m_pmChanCur1->DataTime.SetStartTime(t);
	g_channels = m_multi.size();
	g_mass.resize(g_channels);
	m_result_f.value.resize(3);
	AfxBeginThread(ThreadShield, &m_hWnd);
	return TRUE;
}
//------------------------------------------------------------------------------
void CWattMeterBase::ReStart(bool bStopThread, bool bNewTime)
{
	if (bStopThread)
		StopAllThread();
	if (bNewTime)
	{
		OnSetTimeSRV(0);
		for(auto it=m_multi.begin(); it!=m_multi.end(); ++it)
			it->numberPageCur = 0;
	}
	m_sParam = m_sParamNew;
	g_pThread->Start();
}
//------------------------------------------------------------------------------
void CWattMeterBase::StopAllThread()
{
	g_pThread->StopAndWait();
}
//------------------------------------------------------------------------------
void CWattMeterBase::OnSetTimeSRV(long GlobalPar)
{
	Sleep(100);
	auto it = g_pThread->GetIteratorBegin();
	for (auto itm = m_multi.begin(), itme = m_multi.end(); itm != itme; ++itm, ++it)
	{
		if (it->bExist && it->bJob && it->bEnabled)
		{
			itm->numberPageCur = 0;
			m_psrv->NumFileUsed(it->Channel);
			double dt = 1.;
			if (!m_bMulti && (m_sParam.fast == 2))
				dt = 10.;
			double t = m_psrv->CurrentTime(it->Channel);
			if (t > dt)
				t -= dt;
			it->DataTime.SetStartTime(t);
		}
	}
}
//------------------------------------------------------------------------------
void CWattMeterBase::OnDestroy()
{
	KillTimer(m_IDtimerReStart);
	m_psrv->DisconnectVrtCh();
	// синхронное завершение работы рабочего потока и удаление экз. класса
	if (g_pThread)
	{
		delete g_pThread;
		g_pThread = nullptr;
	}
	//не исправлять
	m_pmChanCur1 = nullptr;
	m_pmParCur1 = nullptr;
	if (g_mass.size() > 0)
	{
		for (UINT i = 0; i < g_mass.size(); i++)
		{
			delete g_mass[i];
			g_mass[i] = nullptr;
		}
		
	}

	g_mass.clear();

	if (g_massive_f)
	{
		delete[] g_massive_f;
		g_massive_f = nullptr;
	}

	WattMeter_Base::OnDestroy();
}
//------------------------------------------------------------------------------
void CWattMeterBase::OnTimer(UINT_PTR nIDEvent)
{
	if ((nIDEvent == m_IDtimerReStart) && m_bProgramInitialized)
	{
		KillTimer(m_IDtimerReStart);
		OnModifySRV();		// обновление параметров каналов
	}
	WattMeter_Base::OnTimer(nIDEvent);
}
//------------------------------------------------------------------------------
void CWattMeterBase::DefaultMultiParam()
{// Задание параметров обработки по умолчанию в соответствии с OnStructureDefault
	m_pmChanCur1->DataTime.SetAverageCodeMode(0);
	m_pmChanCur1->SetNumberCounts();
//	m_pmChanCur1->NumberCounts = m_pmChanCur1->DataTime.CalcNumberCount(m_psrv->Frequency(m_pmChanCur1->Channel));
	m_pmChanCur2->DataTime.SetAverageCodeMode(0);
	m_pmChanCur2->SetNumberCounts();
//	m_pmChanCur2->NumberCounts = m_pmChanCur2->DataTime.CalcNumberCount(m_psrv->Frequency(m_pmChanCur2->Channel));
	m_pmParCur1->numberPage = 1;
//	m_pmParCur1->ReSize(m_pmChanCur1->NumberCounts);
	m_pmParCur2->numberPage = 1;
//	m_pmParCur2->ReSize(m_pmChanCur1->NumberCounts);
}
//------------------------------------------------------------------------------
void CWattMeterBase::New_fast(bool bTest, bool bReStart)
{//усреднение
	CString strTemp;
	if (!m_pmChanCur1->b7000)
	{
		if (bReStart)
			StopAllThread();
		if (bTest)
		{
			m_sParamNew.fast = m_pmChanCur1->DataTime.TestFast(m_sParamNew.fast);
			m_pmChanCur2->DataTime.TestFast(m_sParamNew.fast);
		}
		bool flag = m_sParam.fast != m_sParamNew.fast;
		if (flag)	//Переходим на усреднение с кодом
		{
			strTemp.Format(TranslateResourse(IDS_STRING124) + L" = %d", m_sParamNew.fast);
			MessageInformation(strTemp);
		}
		m_pmParCur1->fast = m_pmParCur2->fast = m_sParam.fast = m_sParamNew.fast;
		SetFast(m_sParamNew.fast);
		SetFastI(m_sParamNew.fast);
		if (bReStart)
			ReStart(false, flag);
	}
}
//------------------------------------------------------------------------------
void CWattMeterBase::NewViewDisplay(long val)
{
	m_sParam.typeview = m_sParamNew.typeview;
	SetViewI(val);
}
//------------------------------------------------------------------------------
void CWattMeterBase::SetFast(const long val)
{// установка времени усреднения текущему каналу
	m_pmParCur1->fast = m_pmChanCur1->DataTime.TestFast(val);
	m_pmParCur2->fast = m_pmChanCur2->DataTime.TestFast(val);
	switch (m_pmParCur1->fast)
	{
	case 0: m_pmParCur1->numberPage = 1; break;
	case 1: m_pmParCur1->numberPage = 10;
	}
	m_pmParCur1->ReSize(m_pmParCur1->numberPage * m_pmChanCur1->NumberCounts);
	m_pmParCur1->numberPageCur = 0;
	m_pmChanCur2->NumberCounts = m_pmChanCur1->NumberCounts;
	m_pmParCur2->numberPage = m_pmParCur1->numberPage;
	m_pmParCur2->ReSize(m_pmParCur2->numberPage * m_pmChanCur2->NumberCounts);
	m_pmParCur2->numberPageCur = 0;
}
//------------------------------------------------------------------------------
void CWattMeterBase::OnStructureNew()
{
	StopAllThread();
	New_channel(true, false);
	New_fast(true, false);
	NewViewDisplay(m_sParamNew.typeview);
	ReStart(false);
}
//------------------------------------------------------------------------------
void CWattMeterBase::OnStructureDefault()
{
	m_sParam.fast = 0;
	m_sParam.channel1 = 0;
	m_sParam.channel2 = 1;
	m_sParam.typeview = 0;
	m_psrv->IdChan(0, &m_sParam.IdChan1);
	m_psrv->IdChan(1, &m_sParam.IdChan2);

	m_sParam.GetPlacement(m_hWnd);
}
//------------------------------------------------------------------------------
void CWattMeterBase::New_channel(bool bTest, bool bReStart)
{//номер канала
	bool flag1 = false, flag2 = false;
	if (bReStart)
		StopAllThread();
	long err = 0, Id = 0;
	err = m_psrv->IdChan(m_sParamNew.channel1, &Id);
	if ((m_IdVirtChan1 == Id) || (m_IdVirtChan2 == Id) || (m_IdVirtChan3 == Id))
		m_sParamNew.channel1 = m_sParam.channel1;
	if (m_sParam.channel1 != m_sParamNew.channel1)
	{
		if (g_pThread->SetChannelNumber(m_sParamNew.channel1))
		{
			SetFast(m_sParamNew.fast);
			flag1 = m_sParam.channel1 != m_sParamNew.channel1;
			flag2 = m_sParam.channel2 != m_sParamNew.channel2;
			if (flag1 || flag2)
			{	//Переходим на канал -
				SetChannel1(m_sParamNew.channel1);
				SetChannel2(m_sParamNew.channel2);
				SetTitleI(m_sParamNew.channel1, m_sParamNew.channel2);
				// новые данные в диалоговое окно
				ResetOutData_iw();
			}
			m_psrv->IdChan(m_sParamNew.channel1, &m_sParamNew.IdChan1);
			m_psrv->IdChan(m_sParamNew.channel2, &m_sParamNew.IdChan2);
		}
		else
		{
			flag1 = true;
			ResetOutData_iw();
		}
	}
	err = m_psrv->IdChan(m_sParamNew.channel2, &Id);
	if ((m_IdVirtChan1 == Id) || (m_IdVirtChan2 == Id) || (m_IdVirtChan3 == Id))
		m_sParamNew.channel2 = m_sParam.channel2;
	if (m_sParam.channel2 != m_sParamNew.channel2)
	{
		g_pThread->SetIndexCur(1);
		if (g_pThread->SetChannelNumber(m_sParamNew.channel2))
		{
			SetFast(m_sParamNew.fast);
			flag1 = m_sParam.channel1 != m_sParamNew.channel1;
			flag2 = m_sParam.channel2 != m_sParamNew.channel2;
			if (flag1 || flag2)
			{	//Переходим на канал -
				SetChannel1(m_sParamNew.channel1);
				SetChannel2(m_sParamNew.channel2);
				SetTitleI(m_sParamNew.channel1, m_sParamNew.channel2);
				// новые данные в диалоговое окно
				ResetOutData_iw();
			}
			m_psrv->IdChan(m_sParamNew.channel1, &m_sParamNew.IdChan1);
			m_psrv->IdChan(m_sParamNew.channel2, &m_sParamNew.IdChan2);
		}
		else
		{
			flag1 = true;
			ResetOutData_iw();
		}
		g_pThread->SetIndexCur(0);
	}
	if (bReStart)
		ReStart(false, flag1);
}
//------------------------------------------------------------------------------
void CWattMeterBase::OnModifySRV()
{
	//CString strTemp(L""); не используется
	bool flag = true;
	if (g_pThread)
	{	
		if (m_bJobManual || m_bJobPanel)
		{	// Одноканальный режим. Возможна работа в режиме подмены канала
			flag = g_pThread->Modify();	// здесь учитывается возможное изменение FreqADC
			if (flag)
			{	// а он не пропал?
				long err = 0, Id = 0;
				err = m_psrv->IdChan(m_sParam.channel1, &Id);
				if ((m_IdVirtChan1 == Id) || (m_IdVirtChan2 == Id) || (m_IdVirtChan3 == Id))
					m_sParam.channel1 = 0;
				err = m_psrv->IdChan(m_sParam.channel2, &Id);
				if ((m_IdVirtChan1 == Id) || (m_IdVirtChan2 == Id) || (m_IdVirtChan3 == Id))
					m_sParam.channel2 = 1;
				for (long i = 0; i < m_psrv->QuanChan(); i++)
				{
					Id = 0;
					m_psrv->IdChan(i, &Id);
					if (Id == m_sParam.IdChan1) m_sParam.channel1 = i;
					Id = 0;
					m_psrv->IdChan(i, &Id);
					if (Id == m_sParam.IdChan2) m_sParam.channel2 = i;
				}
				g_pThread->SetChannelNumber(m_sParam.channel1);
				g_pThread->SetIndexCur(1);
				g_pThread->SetChannelNumber(m_sParam.channel2);
				g_pThread->SetIndexCur(0);
				SetChannel1(m_pmChanCur1->Channel);
				SetChannel2(m_pmChanCur2->Channel);
				SetTitleI(m_pmChanCur1->Channel, m_pmChanCur1->Channel);
			}
		}
		else
		{	// не режим подмены канала. проверяю наличие канала
			g_pThread->Modify();	// здесь учитывается возможное изменение FreqADC
			if (m_pmChanCur1->bEnabled)
			{	// канал пропал, перехожу на 0-ой
				if (m_pmChanCur1->Channel != -1)
				SetChannel1(m_pmChanCur1->Channel);
				if (m_pmChanCur2->Channel != -1)
				SetChannel2(m_pmChanCur2->Channel);
			}
			if (m_psrv->GetStatus(m_sParamNew.channel2) == -4)
			{
				SetChannel2(0);
				m_sParamNew.channel2 = 0;
			}
		}
		// теперь изменения в m_multi
		auto ic = g_pThread->GetIteratorBegin();
		for (auto it = m_multi.begin(); it != m_multi.end(); ++it, ++ic)
			it->ReSize(it->numberPage * ic->NumberCounts);
		SetTitleI(m_sParam.channel1, m_sParam.channel2);
		if (flag)
			ResetOutData_iw();		// обнуление данных в окне
		g_pThread->GoOnThreadAfterModifySRV();
		ReStart(false, flag);		// StopAllThread в OnFirstModifySRV_0
	}
}
//------------------------------------------------------------------------------
void CWattMeterBase::OnReadyUnitParent(long par)
{
	long param;
	double value;
	m_punitParent->UnitParam(&param, &value);
	long val = (long)value;
	switch (param)
	{
	case 0:	 SetUnitParam_p0(val);	break;
	case 1:	 SetUnitParam_p1(val);	break;
	case 2:	 SetUnitParam_p2(val);	break;
	case 3:	 SetUnitParam_p3(val);	break;
	case -1: SetUnitParam_m1(val);	break;
	case -2: SetUnitParam_m2(val);	break;
	case -3: SetUnitParam_m3(val);	break;
	case -4: SetUnitParam_m4(val);	break;
	case -5: SetUnitParam_m5(val);	
	}
}
//------------------------------------------------------------------------------
void CWattMeterBase::OnReadyUnit2Param()
{
	long val;
	switch (m_paramUnit2.number)
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
	case 2:
		if (m_paramUnit2.type == tdu_32s)
		if (m_paramUnit2.Extract_val(val) == NOERROR)
			SetUnitParam_p2(val);
		break;
	case 3:	// определения вида выводимого значения - не используется
		if (m_paramUnit2.type == tdu_32s)
		if (m_paramUnit2.Extract_val(val) == NOERROR)
			SetUnitParam_p3(val);
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
	}
}
//------------------------------------------------------------------------------
void CWattMeterBase::SetUnitParam_p0(const long val)
{	// установка номера первого канала
	if (m_bMulti)
	{		
		if (m_pmChanCur1->Channel != val)
		{
			StopAllThread();
			g_pThread->SetIndexCur(2 * g_pThread->GetIndexCur());
			g_pThread->SetChannelNumber(val);
			SetFast(m_pmParCur1->fast);
			SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
			g_pThread->SetIndexCur(g_pThread->GetIndexCur() / 2);
		}
	}
	else
	{
		if (val > -1 && val < m_psrv->QuanChan() + 1)
		{
			m_sParamNew.channel1 = val;
			OnStructureNewIfInit();
		}
	}
}
//------------------------------------------------------------------------------
void CWattMeterBase::SetUnitParam_p1(const long val)
{	// установка номера второго канала
	if (m_bMulti)
	{		
		if (m_pmChanCur2->Channel != val)
		{
			StopAllThread();
			g_pThread->SetIndexCur(2*g_pThread->GetIndexCur() + 1);
			g_pThread->SetChannelNumber(val);
			SetFast(m_pmParCur2->fast);
			SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
			g_pThread->SetIndexCur((g_pThread->GetIndexCur() - 1) / 2);
		}//*/
	}
	else
	{
		if (val > -1 && val < m_psrv->QuanChan() + 1)
		{
			m_sParamNew.channel2 = val;
			OnStructureNewIfInit();
		}
	}
}
//------------------------------------------------------------------------------
void CWattMeterBase::SetUnitParam_p2(const long val)
{	// установка времени усреднения
	//CString strTemp(L""), strTemp1(L""); не используется
	if (m_bMulti)
	{
		if (m_pmParCur1->fast != val && val > -1 && val < 2)
		{
			Sleep(100);
			StopAllThread();
			m_pmParCur1->fast = m_pmChanCur1->DataTime.TestFast(val);
			m_pmParCur2->fast = m_pmChanCur2->DataTime.TestFast(val);
			m_sParamNew.fast = val;
			SetFast(m_pmParCur1->fast);
			SetFastI(m_pmParCur1->fast);
			SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
		}
	}
	else
	{
		if (val > -1 && val < 2)
		{
			m_sParamNew.fast = val;
			OnStructureNewIfInit();
		}
	}
}
//------------------------------------------------------------------------------
void CWattMeterBase::SetUnitParam_p3(const long val)
{
	if (!m_bMulti)
	{
		if (val > -1 && val < 2)
		{
			m_sParamNew.typeview = val;
			OnStructureNewIfInit();
		}
	}
}
//------------------------------------------------------------------------------
void CWattMeterBase::NumberFormat(const float val, int &degree, int &flo, CString &type)
{
	int min = 0;
	int max = 0;
	float val1(0.f);
	//CString /*s1(L""),*/ s2(L"");
	val1 = abs(val);
	if (val1 >= 1000)
	{
		max = (int)log10(ceil(val1));
		if (max >= 0 && max < 3)
		{

		}
		else if (max >= 3 && max < 6)
		{
			degree = 3;
			switch (max)
			{
			case 3: flo = 3; break;
			case 4: flo = 2; break;
			case 5: flo = 1; break;
			}
			type = L"к";
		}
		else if (max >= 6 && max < 9)
		{
			degree = 6;
			switch (max)
			{
			case 6: flo = 3; break;
			case 7: flo = 2; break;
			case 8: flo = 1; break;
			}
			type = L"М";
		}
		else if (max >= 9 && max < 12)
		{
			degree = 9;
			switch (max)
			{
			case 9: flo = 3; break;
			case 10: flo = 2; break;
			case 11: flo = 1; break;
			}
			type = L"Г";
		}

	}
	else
	{
		if (val1 < 1000 && val1 > 100)
		{
			min = 1;
		}
		else if (val1 < 1000 && val1 > 10)
		{
			min = 2;
		}
		else if (val1 < 1000 && val1 > 1)
		{
			min = 3;
		}
		else if (val1 > 0 && val1 < 1)
		{
			min = int(abs(log10(val1 - static_cast<int>(val1))) + 1);
			if (min >= 0 && min < 3)
			{
				degree = 0;
			}
			else if (min >= 3 && min < 6)
			{
				degree = -3;
				type = L"m";
			}
			else if (min >= 6 && min < 9)
			{
				degree = -6;
				type = L"mk";
			}
			min = 4;
		}
	flo = min;
	}
}
//------------------------------------------------------------------------------
void CWattMeterBase::SetUnitParam_m1(const long val)
{// Переход в многоканальный режим. Первоначальное задание кол-ва каналов
	MultiParamSelf mlt;
	CString strTemp;
	if (!m_bMulti)
	{
		if (val > 0)
		{
			m_bMulti = true;
			StopAllThread();
			for (long i = 0; i < val - 1; ++i)	// один канал уже есть
			{
				m_multi.push_back(mlt);	// так как используется не один канал а пара
				m_multi.push_back(mlt);	// то добавляем две структуры в вектор
				g_pThread->AddChannel(-1);
				g_pThread->AddChannel(-1);
			}
			g_pThread->SetIndexCur(1);
			g_pThread->SetChannelNumber(-1);
			g_pThread->SetIndexCur(0);
			g_pThread->SetChannelNumber(-1);
			m_pmChanCur1 = g_pThread->GetPointerMultiChannelCur();	// передаем указатели на структуры
			m_pmParCur1 = &m_multi[g_pThread->GetIndexCur()];
			g_pThread->SetIndexCur(1);
			m_pmChanCur2 = g_pThread->GetPointerMultiChannelCur();
			m_pmParCur2 = &m_multi[g_pThread->GetIndexCur()];
			g_pThread->SetIndexCur(0);
			g_mass.resize(2 * val);			// увеличиваем вектор массивово с данными
			//Многоканальный режим работы программы. Кол-во каналов
			strTemp.Format(TranslateResourse(IDS_STRING123) + L" = %d", val);
			MessageInformation(strTemp);
			SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
			g_mass.resize(2 * val);
		}
		ViewNewChanInWindows();
		ResetOutData_iw();
		g_channels = m_multi.size();
	}//*/
}
//------------------------------------------------------------------------------
void CWattMeterBase::SetUnitParam_m2(const long val)
{// задание текущего индекса канала для будущего изменения параметров
	CString strTemp;
	if (m_bMulti)
	{
		long iOld = g_pThread->GetIndexCur();
		if (g_pThread->SetIndexCur(2 * val))
		{
			long num = g_pThread->GetIndexCur();
			if (iOld != num)
			{
				//L"Установка текущего индекса канала"
				strTemp.Format(TranslateResourse(IDS_STRING122) + L" = %d",	num);
				MessageInformation(strTemp);
				m_pmChanCur1 = g_pThread->GetPointerMultiChannelCur();	// передаем указатели на структуры
				m_pmParCur1 = &m_multi[g_pThread->GetIndexCur()];
				g_pThread->SetIndexCur(2* val + 1);
				m_pmChanCur2 = g_pThread->GetPointerMultiChannelCur();
				m_pmParCur2 = &m_multi[g_pThread->GetIndexCur()];
				g_pThread->SetIndexCur(val);
				ViewNewChanInWindows();
			}
		}
		g_channels = m_multi.size();
	}
}
//------------------------------------------------------------------------------
void CWattMeterBase::SetUnitParam_m3(const long val)
{// остановка канала
	CString strTemp;
	if (m_bMulti)
	{
		if ((0 <= val) && (val < (long)m_multi.size()))
		{
			StopAllThread();
			if (g_pThread->StopChannel(2 * val) && g_pThread->StopChannel(2 * val + 1))	// останавливаем пару каналов
			{// L"Остановка канала с индексом"
				strTemp.Format(TranslateResourse(IDS_STRING121) + L" = %d", val);
				MessageInformation(strTemp);
			}
			SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
		}
		g_channels = m_multi.size();
	}//*/
}
//------------------------------------------------------------------------------
void CWattMeterBase::SetUnitParam_m4(const long val)
{// запуск канала
	CString strTemp;
	if (m_bMulti)
	{
		if ((0 <= val) && (val < (long)m_multi.size()))
		{
			StopAllThread();
			if (g_pThread->StartChannel(2 * val) && g_pThread->StartChannel(2 * val + 1))	// запускаем пару каналов
			{// L"Запуск канала с индексом"
				strTemp.Format(TranslateResourse(IDS_STRING119) + L" = %d", val);
				MessageInformation(strTemp);
			}
			SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
		}
		g_channels = m_multi.size();
	}//*/
}
//------------------------------------------------------------------------------
void CWattMeterBase::SetUnitParam_m5(const long val)
{ // добавление канала, он станет текущим
/*	static MultiParamSelf mlt;
	CString strTemp;
	int a = 0;
	if (m_bMulti)
	{// добавление канала № num
		if ((0 <= val) && (val < m_psrv->QuanChan()))
		{
			StopAllThread();
			if (g_pThread->AddChannel(val) != -1 && g_pThread->AddChannel(val) != -1)
			{// добавление удачное (0) или добавлен не существующий канал (-2)
				// функция SetChannelNumber будет вызвана внутри AddChannel				
				m_multi.push_back(mlt);	
				m_multi.push_back(mlt);
				g_pThread->SetIndexCur(2 * g_pThread->GetIndexCur());
				m_pmParCur1 = &m_multi[g_pThread->GetIndexCur()];
				m_pmChanCur1 = g_pThread->GetPointerMultiChannelCur();
				g_pThread->SetIndexCur(g_pThread->GetIndexCur() + 1);
				m_pmParCur2 = &m_multi[g_pThread->GetIndexCur()];
				m_pmChanCur2 = g_pThread->GetPointerMultiChannelCur();
				g_pThread->SetIndexCur((g_pThread->GetIndexCur() - 1) / 2);
				if (m_pmChanCur1->b7000)
				{
					m_pmChanCur1->bJob = false;
				}
				else if (m_pmChanCur1->bExist)
				{
					DefaultMultiParam();
					SetFast(m_pmParCur1->fast);
				}
				strTemp.Format(TranslateResourse(IDS_STRING120) + L"%s",
					m_psrv->Commentary(val));
				MessageInformation(strTemp);
			}
			else
			{
				g_pThread->SetIndexCur(2 * g_pThread->GetIndexCur());
				m_pmParCur1 = &m_multi[g_pThread->GetIndexCur()];
				m_pmChanCur1 = g_pThread->GetPointerMultiChannelCur();
				g_pThread->SetIndexCur(g_pThread->GetIndexCur() + 1);
				m_pmParCur2 = &m_multi[g_pThread->GetIndexCur()];
				m_pmChanCur2 = g_pThread->GetPointerMultiChannelCur();
				g_pThread->SetIndexCur((g_pThread->GetIndexCur() - 1) / 2);
			}
			ViewNewChanInWindows();
			SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
		}
		g_channels = m_multi.size();
	}//*/
}
//------------------------------------------------------------------------------
void CWattMeterBase::ViewNewChanInWindows()
{
	SetChannel1(m_pmChanCur1->Channel);
	SetChannel2(m_pmChanCur2->Channel);
	SetFastI(m_pmParCur1->fast);
	ResetOutData_iw();
}
//------------------------------------------------------------------------------
void CWattMeterBase::OnFileModeSRV(long GlobalPar)
{
	StopAllThread();
	OnModifySRV();		// надо обновить лист каналов
}
//------------------------------------------------------------------------------
void CWattMeterBase::OnStartFileSRV(long GlobalPar)
{// При GlobalPar=0 старт чтения из файла (работа с ридером)
	// При этом время может != 0
	if (GlobalPar == 0)
		OnSetTimeSRV(0);
}
//------------------------------------------------------------------------------
void CWattMeterBase::TestBackupChanMode()
{// режим подмены канала после чтения конфигурационного файла
	long n = m_psrv->detectChanNum(m_sParamNew.IdChan1);
	if (n != -1)
		m_sParamNew.channel1 = n;
	else
	{// канала из конфиг. файла нет
		// работать буду с другим каналом
		long m = m_psrv->QuanChan();
		if (m_sParamNew.channel1 < 0)
			m_sParamNew.channel1 = 0;
		else if (m_sParamNew.channel1 >= m)
			m_sParamNew.channel1 = m - 1;
		m_psrv->IdChan(m_sParamNew.channel1, &m_sParamNew.IdChan1);
		m_psrv->IdChan(m_sParamNew.channel2, &m_sParamNew.IdChan2);
	}
}
//------------------------------------------------------------------------------
void CWattMeterBase::SetInfoVrtCahnnel(const long fchannel)
{
	CString strTemp1, strTemp2;
	BSTR s1, s2;
	float freq = m_psrv->GetFrequency(fchannel);
	float max = m_psrv->MaxLevel(fchannel);
	float min = m_psrv->MinLevel(fchannel);
	float ref = m_psrv->Reference(fchannel);

	strTemp2.Format(L"%i", m_sParam.numberProgram);
	strTemp1 = TranslateResourse(IDS_STRING108);
	strTemp1 += L" " + strTemp2;

	s1 = strTemp1.AllocSysString();
	strTemp1 = TranslateResourse(IDS_STRING125);
	s2 = strTemp1.AllocSysString();
	m_psrv->IdChanVirt(0, &m_IdVirtChan1);
	m_psrv->VrtChnInfo(0, &s1, &s2, freq, max, min, ref);
	SysFreeString(s1);
	SysFreeString(s2);

	strTemp1 = TranslateResourse(IDS_STRING107);
	strTemp1 += L" " + strTemp2;
	s1 = strTemp1.AllocSysString();
	strTemp1 = TranslateResourse(IDS_STRING126);
	s2 = strTemp1.AllocSysString();
	m_psrv->IdChanVirt(1, &m_IdVirtChan2);
	m_psrv->VrtChnInfo(1, &s1, &s2, freq, max, min, ref);
	SysFreeString(s1);
	SysFreeString(s2);

	strTemp1 = TranslateResourse(IDS_STRING109);
	strTemp1 += L" " + strTemp2;
	s1 = strTemp1.AllocSysString();
	strTemp1 = TranslateResourse(IDS_STRING127);
	s2 = strTemp1.AllocSysString();
	m_psrv->IdChanVirt(2, &m_IdVirtChan3);
	m_psrv->VrtChnInfo(2, &s1, &s2, freq, max, min, ref);
	SysFreeString(s1);
	SysFreeString(s2);
}
//------------------------------------------------------------------------------
