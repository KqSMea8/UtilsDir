//------------------------------------------------------------------------------
// VoltMeterDlg.cpp : implementation file
//------------------------------------------------------------------------------
//	Чтение данных каждый канал выполняет за 0,1 сек, это одна порция.
// А кол-во порций для усредненния берётся в соответствии с заданным временем.
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "VoltMeter.h"
#include "VoltMeter_AC.h"
#include <Intel\ipp\ippcore.h>
//------------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//------------------------------------------------------------------------------
CThreadSRVmultiSelf *g_pThread;
float *g_pMasTmp;	// вспомогательный массив
DWORD g_SizeMasTmp;	// его размер
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
// Расчёт измеряемых значений по данным одного канала
void Calc(float *add, DWORD num, float *peak, float *ampl, float *sks)
{// исходный массив изменять НЕЛЬЗЯ
	float x, y, mean;
	if (g_SizeMasTmp < num)
	{
		if (g_pMasTmp)
		{
			delete [] g_pMasTmp;
			g_pMasTmp = nullptr;
		}
		g_pMasTmp = new float [num];
		g_SizeMasTmp = num;
	}
	ippsCopy_32f(add, g_pMasTmp, num);
	ippsMean_32f(g_pMasTmp, num, &mean, ippAlgHintFast);
	ippsSubC_32f_I(mean, g_pMasTmp, num);
	ippsMinMax_32f(g_pMasTmp, num, &x, &y);
	*peak = max(fabsf(x), fabsf(y));

	*ampl = (y - x) * 0.5f;		// амплитуда == половина размаха

	ippsSqr_32f_I(g_pMasTmp, num);
	ippsSum_32f(g_pMasTmp, num, &x, ippAlgHintFast);
	*sks = sqrt( x / (float)num );
}
//------------------------------------------------------------------------------
void Calculation(MultiParamSelf *pm, MultiChannelSelf *pc, float *pResult)
{// Вначале из выборки выделяем целое число периодов
	int i, t1, t2;
	float x, mi, ma;
	float valPeak, valAmpl, valSks;
	bool bPlus, bUp_Doun;
	DWORD sizeUse = pm->numberPage * pc->NumberCounts;
	ippsMinMax_32f(pm->pData, sizeUse, &mi, &ma);
	x = ( ma + mi ) * 0.5f;
	if ( pm->pData[0] > x )
		bPlus = true;
	else
		bPlus = false;

	// ищу первый переход через 0 и определяю направление перехода
	for(i=1; i<(int)sizeUse; ++i)
	{
		if (bPlus)
		{// bPlus = true
			if (pm->pData[i] < x)
			{
				bUp_Doun = true;	// график вниз
				break;
			}
		}
		else
		{// bPlus = false
			if (pm->pData[i] > x)
			{
				bUp_Doun = false;	// график вверх
				break;
			}
		}
	}
	t1 = i;
	// ищу последний переход через 0 с тем же направлением
	for(i=(int)sizeUse-1; i>t1; --i)
	{
		if (bUp_Doun)
		{// bUp_Doun = true, график вниз
			if ( ( pm->pData[i-1] > x ) &&
				( pm->pData[i]   < x ) )
				break;
		}
		else
		{// bUp_Doun = false, график вверх
			if ( ( pm->pData[i-1] < x ) &&
				( pm->pData[i]   > x ) )
				break;
		}
	}
	t2 = i;
	// точку t1 беру, а t2 - нет, тогда будет целое число периодов
	if ( (t2-t1) > (int)sizeUse/3 )
		Calc(pm->pData+t1, t2-t1, &valPeak, &valAmpl, &valSks);
	else
		Calc(pm->pData, sizeUse, &valPeak, &valAmpl, &valSks);
	// задание переменных, из которых основной поток будет вычитывать значения
	pResult[0] = valSks;
	pResult[1] = valAmpl;
	pResult[2] = valPeak;
}
//------------------------------------------------------------------------------
// Функция обработки считанных с сервера данных.
// Данная программа не поддерживает каналы серии 7000!
long GlobalFunctionMulti(PVOID pParam)
{
	float result[3] = {0.f};
	// определяю указатели на параметры канала, у которого прочитаны данные
	SendToSecGlobFunction *ppar = (SendToSecGlobFunction*)pParam;
	MultiChannelSelf *pMChan = ppar->pMChan;
	MultiParamSelf *pm =
		(((std::vector<MultiParamSelf>*)ppar->pParamNext)->begin() + ppar->index)._Ptr;
	CodeQuality curCQ;
	if (pMChan->RetGetData != 0)
		curCQ.Error = 1;
	pm->pCodeQuality[pm->numberPageCur] = curCQ;
	float *add = pm->pData + pm->numberPageCur * pMChan->NumberCounts;
	if ( pMChan->RetGetData == 0 )
		ippsCopy_32f(pMChan->pData, add, pMChan->NumberCounts);
	else
		ippsSet_32f(0.f, add, pMChan->NumberCounts);

	if ( ++pm->numberPageCur >= pm->numberPage )
	{// собраны все данные по каналу
		// проверка качества данных
		pm->time = pMChan->DataTime.GetTime();
		pm->qualityAll = 0;
		for(DWORD i=0; i<pm->numberPage; ++i)
		{
			pm->qualityAll |= pm->pCodeQuality[i];
		}

		if ( pm->qualityAll == 0 )
			Calculation(pm, pMChan, result);		// обработка
		//подготовка к следующей порции
		if (pm->fast != 2)	// усреднение 0,1 или 1 сек	
			pm->numberPageCur = 0;
		else
		{// усреднение 10 сек, реализация скользящего среднего,
			// сдвиг данных на 1 сек, т.е. 10 страниц, т.к.
			// интервал чтения данных 0,1 сек
			UINT sizeMove = 90 * pMChan->NumberCounts * sizeof(float);
			memmove_s(pm->pData, sizeMove, pm->pData + 10 * pMChan->NumberCounts, sizeMove);

			sizeMove = 90 * sizeof(int);
			memmove_s(pm->pCodeQuality, sizeMove, pm->pCodeQuality + 10, sizeMove);

			pm->numberPageCur = 90;
		}
		if ( pm->qualityAll == 0 )
		{	// копирование результатов, именно здесь, в конце
			pm->valSks  = result[0];
			pm->valAmpl = result[1];
			pm->valPeak = result[2];
		}
		else
		{
			pm->valAmpl = pm->valPeak = pm->valSks = NAN;
		}
		pm->qualityAll.AllData = Quality_AllData_True;	// всё обработано
	}
	else
		pm->qualityAll.AllData = Quality_AllData_False;	// данных для обработки не достаточно

	return pm->qualityAll;
}
//------------------------------------------------------------------------------
CVoltMeter_AC::CVoltMeter_AC( UINT nIDTemplate, CWnd* pParent, CString exeName,
	CString cfgFilePreamble, ULONG nZPriority)
	: VoltMeterBase(
	nIDTemplate,		// идентификатор программы
	pParent,			// указатель на владельца
	exeName,			// имя exe-файла программы
	cfgFilePreamble,	// имя преамбулы конфигурационного файла				
	nZPriority)			// приоритет программы в Zet-панели (от 0 до 3)
	, m_ampl(0)
	, m_sks(0)
	, m_peak(0)
	, m_bMulti(false)
	, m_ElapseTimerReStart(200)
	, m_IDtimerReStart(1)
	, m_pmChanCur(nullptr)
	, m_pmParCur(nullptr)

{
	m_bPlacementCFG = true;
	pSaveData = &m_sParam;
	pSaveDataNew = &m_sParamNew;
	m_sizeSaveData = sizeof(SaveStruct);
	ZeroMemory(pSaveData, m_sizeSaveData);
	ZeroMemory(pSaveDataNew, m_sizeSaveData);
	m_sParam.SetName(m_sExeName + L".exe");
	m_sParamNew.SetName(m_sExeName + L".exe");
	m_sParam.size = m_sParamNew.size = sizeof(m_sParam);
	m_sCommentAndOrder = TranslateString(L"Вольтметр переменного тока");

	g_pThread = NULL;
	g_pMasTmp = NULL;	// вспомогательный массив
	g_SizeMasTmp = 0;	// его размер

	ippInit();
}
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CVoltMeter_AC, VoltMeterBase)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_NEW_DATA_SRV, OnSetUpdate)
END_MESSAGE_MAP()
//------------------------------------------------------------------------------
BOOL CVoltMeter_AC::OnInitDialog()
{
	VoltMeterBase::OnInitDialog();

	m_sSec = TranslateString(L"с");
	m_sDb = TranslateString(L"дБ");
	m_sPeak = TranslateString(L"пик");
	m_sFast_0_1 = TranslateString(L"Быстро") + L" 0.1 " + m_sSec;
	m_sFast_1_0 = TranslateString(L"Медленно") + L" 1 " + m_sSec;
	m_sFast_10 = TranslateString(L"Сверхмедленно") + L" 10 " + m_sSec;

	m_srms = TranslateString(L"СКЗ");		
	m_srms_db = m_srms + L", " + m_sDb;		//(L"СКЗ, дБ");
	m_samp = TranslateString(L"Амплитуда");		
	m_samp_db = m_samp + L", " + m_sDb;		// (L"Амплитуда, дБ");


	m_sParam.numberProgram = m_sParamNew.numberProgram = (uint32_t)m_nOrder;

	CreateElementInterface();
	Set_Icon_iw();
	View_NewChan_iw();
	View_MeasureType_iw();
	View_Average_iw();

	Set_Title_iw(m_sParam.channel);
	Set_Channel_iw(m_sParam.channel);
	Set_Fast_iw(m_sParam.fast);
	Set_MeasureType_iw( CalcMeasureType(m_sParam.ampl, m_sParam.decibel) );

	ResetOutData_iw();
	Set_Mera_iw( m_psrv->Conversion( m_sParam.channel ) );

	// По умолчанию задаётся fast=1 => TimeInterval = 1 сек
	// Вначале new (текущий индекс = 0),
	// затем Create (вектор MultiChannel получает один элемент),
	// потом SetTimeInterval и только потом SetChannelNumber!!!
	// Тогда параметры канала заданы полностью и правильно
	g_pThread = new CThreadSRVmultiSelf();
	g_pThread->Create(L"Thread SRV", m_hWnd, m_psrv, 1,
		&GlobalFunctionMulti, &m_multi);
	g_pThread->SetSupport7000(false);
	g_pThread->SetChannelNumber(m_sParam.channel);
	m_pmChanCur = g_pThread->GetPointerMultiChannelCur();
	m_pmChanCur->DataTime.SetAverageCodeMode(0, m_psrv->Frequency(m_sParam.channel));

	MultiParamSelf mulCur;
	m_multi.push_back(mulCur);		// добавил элемент
	m_pmParCur = &m_multi[0];
	SetFast(m_sParam.channel);
	DefaultMultiParam();

	// отображение текстов слева
	if (m_pmChanCur->b7000)
		Show7000();
	else
		ResetOutData_iw();
	//	SetStrFormat(m_sParam.channel, m_sParam.decibel);
	double dt = 1.;
	if ( !m_bMulti && (m_sParam.fast == 2) )
		dt = 10.;
	double t = m_psrv->CurrentTime(m_sParam.channel);
	if (t > dt)
		t -= dt;
	m_pmChanCur->DataTime.SetStartTime(t);
	m_pmParCur->numberPageCur = 0;
	//------------------------------------------------
	AfxBeginThread(ThreadShield, &m_hWnd);
	return TRUE;	// return TRUE  unless you set the focus to a control
}
//------------------------------------------------------------------------------
void CVoltMeter_AC::OnDestroy()
{
	KillTimer(m_IDtimerReStart);
	// синхронное завершение работы рабочего потока и удаление экз. класса
	if (g_pThread)
	{
		delete g_pThread;
		g_pThread = nullptr;
	}
	// временный массив, к нему обращается только рабочий поток
	if (g_pMasTmp)
	{
		delete [] g_pMasTmp;
		g_pMasTmp = nullptr;
	}
	m_multi.clear();	// вектор параметров обработки каналов
	m_pmChanCur = nullptr;
	m_pmParCur = nullptr;

	DeleteElementInterface();

	VoltMeterBase::OnDestroy();
}
//------------------------------------------------------------------------------
void CVoltMeter_AC::OnStructureDefault()
{
	m_sParam.ampl = 0;
	m_sParam.decibel = 0;
	m_sParam.fast = 1;
	m_sParam.channel = 0;
	m_psrv->IdChan(0, &m_sParam.IdChan);
	m_sParam.GetPlacement(m_hWnd);
}
//------------------------------------------------------------------------------
void CVoltMeter_AC::OnStructureNew()
{
	StopAllThread();
	New_channel(true, false);
	New_fast(true, false);
	New_ampl_decibel(true);
	ReStart(false);
}
//------------------------------------------------------------------------------
void CVoltMeter_AC::ViewNewChanInWindows()
{
	Set_Channel_iw(m_pmChanCur->Channel);
	Set_Title_iw(m_pmChanCur->Channel);
	Set_MeasureType_iw( CalcMeasureType(m_pmParCur->ampl, m_pmParCur->decibel) );
	Set_Fast_iw(m_pmParCur->fast);

	ResetOutData_iw();
	if (m_pmParCur->decibel == 0)
		Set_Mera_iw( m_psrv->Conversion(m_pmChanCur->Channel) );
	else
		Set_Mera_iw(m_sDb);
}
//------------------------------------------------------------------------------
//void CVoltMeter_AC::SetStrFormat(const long channel, const long decibel)
//{
//	if (decibel)
//		m_strFormat = L"%.1f";
//	else
//	{
//		int a, b;
//		a = (int)log10(m_psrv->MaxLevel(channel));
//		if (a<0)
//			a=1;
//		b = -(int)log10(m_psrv->MinLevel(channel)) + 1;
//		if (b<0)
//			b=0;
//
//		if (a+b>8)
//		{
//			if (a>8)
//			{
//				a=8;
//				b=0;
//			}
//			else
//			{
//				a=1;
//				b=8;
//			}
//		}
//		b += 2;
//		m_strFormat.Format(L"%%%d.%df", a+b, b);
//	}
//}
//------------------------------------------------------------------------------
void CVoltMeter_AC::SetFast(const long val)
{// установка времени усреднения текущему каналу
	m_pmParCur->fast = m_pmChanCur->DataTime.TestFast(val);
	switch(m_pmParCur->fast)
	{
	case 0: m_pmParCur->numberPage = 1; break;
	case 1: m_pmParCur->numberPage = 10; break;
	case 2: m_pmParCur->numberPage = 100;
	}
	m_pmParCur->ReSize( m_pmParCur->numberPage * m_pmChanCur->NumberCounts );
	m_pmParCur->numberPageCur = 0;
}
//------------------------------------------------------------------------------
//void CVoltMeter_AC::DefaultMultiParam(MultiParamSelf *pmp, bool b7000, int numKadrs)
void CVoltMeter_AC::DefaultMultiParam()
{// Задание параметров обработки по умолчанию в соответствии с OnStructureDefault
	m_pmChanCur->DataTime.SetAverageCodeMode(0);	// всегда чтение порциями по 0,1 сек
	g_pThread->SetChannelNumberCountsAuto();
	SetFast(1);		// по умолчанию усреднение 1 сек
}
//------------------------------------------------------------------------------
void CVoltMeter_AC::Show7000()
{
	Set_Mean_iw(L"not", 0);
	Set_Peak_iw(L"supported", 0);

	Quality_iw(0);
}
//------------------------------------------------------------------------------
long CVoltMeter_AC::CalcMeasureType(const long ampl, const long decibel)
{
	return 2 * ampl + decibel;
}
//------------------------------------------------------------------------------
LRESULT CVoltMeter_AC::OnSetUpdate(WPARAM wParam, LPARAM lParam)
{
	if ( g_pThread )	// если не была выполнена функция OnDestroy
	{
 		float x(0.f), pe(0.f);
		float valSKS(0.f), valAmpl(0.f), valPeak(0.f);		// результаты
		double time(0.);
		CString str;
		str.Format(L"%i", wParam);
		// параметры обработанного канала -----------------------------
		int index = (int)wParam;
		MultiParamSelf *pmp = &m_multi[index];
		MultiChannelSelf *pmc = (g_pThread->GetIteratorBegin() + index)._Ptr;
		// далее к этим полям структуры pmp обращаться нельзя
		valSKS  = pmp->valSks;
		valAmpl = pmp->valAmpl;
		valPeak = pmp->valPeak;
		time = pmp->time;
		long quality = (long)lParam;
		CodeQuality cq = quality;

		// установка события View --------------
		g_pThread->SetSynchro();

		if (cq.AllData == Quality_AllData_False)
			return 0;

		// передача в UNIT всегда ---------------------------
		if (m_bJobUnit || m_bJobUnit2)
		{
			switch (pmp->typeValUnit)
			{
			case 0 : x = valSKS; break;
			case 1 : x = valPeak; break;
			case 2 : x = valAmpl;
			}
			if (pmp->decibel)	// 0 - не дБ, 1 - дБ
				x = 20.f * log10( x / m_psrv->Reference( pmc->Channel ) );
			if (m_bJobUnit)
				m_punitParent->UnitWrite(1, &x, index);
			else if (m_bJobUnit2)// результат в UNIT-2
			{// результат - число
				index *= 100;		// т.к. многоканальная программа
			//	WriteUnitResult_p0(index, x);
				// результат - широкий результат числа
				WideResult<float> wrsF;
				wrsF.quality = quality;
				wrsF.time = time;
				wrsF.value = x;
				WriteUnitResult_p1(index, &wrsF);
			}
		}
		// отображение результатов ----------------------------------		
		// для текущего канала отображение в окне программы
		if ( index == g_pThread->GetIndexCur() )
		{
			Set_HorScale_iw( m_psrv->CurLevel(pmc->Channel) );
			// качество полученных данных ----------------------------------
			if (quality == 0)
			{// надо запомнить
				m_sks  = valSKS;
				m_ampl = valAmpl;
				m_peak = valPeak;

				if (pmp->ampl == 0)	// == 0 - скз
					x = valSKS;
				else				// == 1 - амплитуда
					x = valAmpl;
				pe = valPeak;
				// переходим к дБ (если надо)
				if ( pmp->decibel )	// 0 - не дБ, 1 - дБ
				{
					float r = 1.f / m_psrv->Reference( pmc->Channel );
					x = 20.f * log10( x * r );
					pe = 20.f * log10( pe * r );
					strAmpl.Format(L"%.1f", x);
					strPeak.Format(L"%.1f", pe);
				}
				else
				{
					strAmpl = DigitalNotation(x);
					strPeak = DigitalNotation(pe);
				}
				// отображение в окне программы
				strTemp = m_sPeak + L"  " + strPeak;
				Set_Mean_iw(strAmpl, x);
				Set_Peak_iw(strTemp, pe);
			}
			else
			{
				Set_Mean_iw(L"*****", 0);
				Set_Peak_iw(L"", 0);				
			}
			// перерисовка фона
			Quality_iw(quality);

		}// if ( index == g_pThread->GetIndexCur() )
	} // if ( g_pThread )
	return 0;
}
//------------------------------------------------------------------------------
void CVoltMeter_AC::OnTimer(UINT_PTR nIDEvent)
{// Таймер нужен для того, чтобы при программировании программы через UNIT в
	// многоканальном режиме не передёргивать поток сервера
	if ( (nIDEvent == m_IDtimerReStart) && m_bProgramInitialized )
	{
		KillTimer(m_IDtimerReStart);
		//	ReStart(false);
		OnModifySRV();		// обновление параметров каналов
	}
	VoltMeterBase::OnTimer(nIDEvent);
}
//------------------------------------------------------------------------------
BOOL CVoltMeter_AC::PreTranslateMessage(MSG* pMsg)
{
	if ( (pMsg->message == WM_CHAR) && ( (int)pMsg->wParam == 3) )								
		CopyStringToClipboard(strAmpl);			// код 3 - это "Ctrl + C"
	return VoltMeterBase::PreTranslateMessage(pMsg);
}
//------------------------------------------------------------------------------
void CVoltMeter_AC::StopAllThread()
{	// останавливаю поток сервера -----------------------------
	g_pThread->StopAndWait();
}
//------------------------------------------------------------------------------
void CVoltMeter_AC::New_channel(bool bTest, bool bReStart)
{//номер канала
	bool flag;
	if (bReStart)
		StopAllThread();
	m_sParamNew.channel = m_psrv->detectChanNum(m_sParamNew.IdChan);
	if ( g_pThread->SetChannelNumber(m_sParamNew.channel) )
	{
		SetFast(m_sParamNew.fast);
		flag = m_sParam.channel != m_sParamNew.channel;
		if (flag)
		{	//Переходим на канал -
			MessageInformation(TranslateString(L"Переходим на канал") +
				m_psrv->Commentary(m_sParamNew.channel));
			Set_Channel_iw(m_sParamNew.channel);

			// новые данные в диалоговое окно
			ResetOutData_iw();
		}
		m_psrv->IdChan(m_sParamNew.channel, &m_sParamNew.IdChan);
		if (m_sParamNew.decibel == 1)
			Set_Mera_iw(m_sDb);
		else
			Set_Mera_iw(m_psrv->Conversion(m_sParamNew.channel));
		Set_Title_iw(m_sParamNew.channel);
		//		SetStrFormat(m_sParamNew.channel, m_sParamNew.decibel);
		//if (m_bJobUnit2)
		//	m_punitSrv->SetParamBack_32s(0, m_sParamNew.channel);
		if ( g_pThread->GetPointerMultiChannelCur()->b7000 &&
			!g_pThread->GetSupport7000() )
		{
			Show7000();
		}
	}
	else
	{
		flag = true;
		Set_Title_iw(-1);
		ResetOutData_iw();
	}
	if (bReStart)
		ReStart(false, flag);
}
//------------------------------------------------------------------------------
void CVoltMeter_AC::New_fast(bool bTest, bool bReStart)
{//усреднение
	if (!m_pmChanCur->b7000)
	{
		if (bReStart)
		{
			StopAllThread();
		}
		if (bTest)
			m_sParamNew.fast = m_pmChanCur->DataTime.TestFast(m_sParamNew.fast);

		bool flag = m_sParam.fast != m_sParamNew.fast;
		if (flag)	//Переходим на усреднение с кодом
		{
			strTemp.Format(TranslateString(L"Переходим на усреднение с кодом") + L" = %d",
				m_sParamNew.fast );
			MessageInformation(strTemp);
		}
		m_pmParCur->fast = m_sParam.fast = m_sParamNew.fast;
		SetFast(m_sParamNew.fast);
		Set_Fast_iw(m_sParamNew.fast);

		//if (m_bJobUnit2)
		//	m_punitSrv->SetParamBack_32s(1, m_sParamNew.fast);
		if (bReStart)
		{
			ReStart(false, flag);
		}
	}
}
//------------------------------------------------------------------------------
void CVoltMeter_AC::New_ampl_decibel(bool bTest)
{
	if (bTest)
	{
		if (m_sParamNew.decibel < 0)
			m_sParamNew.decibel = 0;
		else if ( m_sParamNew.decibel > 1 )
			m_sParamNew.decibel = 1;

		if (m_sParamNew.ampl < 0)
			m_sParamNew.ampl = 0;
		else if ( m_sParamNew.ampl > 2 )
			m_sParamNew.ampl = 1;
	}

	if ( (m_sParam.decibel != m_sParamNew.decibel) ||
		(m_sParam.ampl != m_sParamNew.ampl) )
	{
		Set_MeasureType_iw( CalcMeasureType(m_sParamNew.ampl, m_sParamNew.decibel) );

		if (m_sParamNew.decibel)
			Set_Mera_iw(m_sDb);
		else
			Set_Mera_iw( m_psrv->Conversion(m_sParamNew.channel) );
	}
	//if (!m_pmChanCur->b7000)
	//	SetStrFormat(m_pmChanCur->Channel, m_sParamNew.decibel);

	m_pmParCur->ampl = m_sParam.ampl = m_sParamNew.ampl;
	m_pmParCur->decibel = m_sParam.decibel = m_sParamNew.decibel;
	//if (m_bJobUnit2)
	//{
	//	m_punitSrv->SetParamBack_32s(2, m_pmParCur->typeValUnit);
	//	m_punitSrv->SetParamBack_32s(3, m_sParam.decibel);
	//}
}
//------------------------------------------------------------------------------
void CVoltMeter_AC::ReStart(bool bStopThread, bool bNewTime)
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
void CVoltMeter_AC::OnFileModeSRV(long GlobalPar)
{
	StopAllThread();
	OnModifySRV();		// надо обновить лист каналов
}
//------------------------------------------------------------------------------
void CVoltMeter_AC::OnSetTimeSRV(long GlobalPar)
{
// 	Sleep(100);
	auto it = g_pThread->GetIteratorBegin();
	for (auto itm=m_multi.begin(), itme=m_multi.end(); itm!=itme; ++itm, ++it)
	{
		if ( it->bExist && it->bJob && it->bEnabled)
		{
			itm->numberPageCur = 0;
			m_psrv->NumFileUsed(it->Channel);
			CZetTimeSpan zts = m_psrv->CurrentTime(it->Channel);
			zts.RoundSecondsDown();
			zts += CZetTimeSpan(0, 100000000);	// + 0.1 сек
			it->DataTime.SetTimeRead(zts);
		}
	}
}
//------------------------------------------------------------------------------
void CVoltMeter_AC::OnStartFileSRV(long GlobalPar)
{// При GlobalPar=0 старт чтения из файла (работа с ридером)
	// При этом время может != 0
	if (GlobalPar == 0)
		OnSetTimeSRV(0);
}
//------------------------------------------------------------------------------
void CVoltMeter_AC::OnModifySRV()
{// последний Modify или просто надо обновить информацию о каналах
	static CString sAbsent(TranslateString(L"отсутствует"));

	if (g_pThread)
	{	// могли измениться каналы
		if (m_bJobManual || m_bJobPanel)
		{
			g_pThread->Modify();		// здесь учитывается возможное изменение FreqADC
			if (m_sParam.decibel == 0)	// могла измениться размерность
				Set_Mera_iw(m_psrv->Conversion(m_pmChanCur->Channel));
			if (m_pmChanCur->bExist)
				Set_Title_iw(m_pmChanCur->Channel);		// могло измениться имя канала
			else
			{
				CString title;
				GetWindowTextW(title);
				title += L" " + sAbsent;
				SetWindowTextW(title);
				ResetOutData_iw();		// обнуление данных в окне
			}
		}
		else
		{// UNIT - без подмены канала
			g_pThread->Modify();	// здесь учитывается возможное изменение FreqADC
			if (m_pmChanCur->bEnabled)
			{
				if (m_pmChanCur->Channel != -1)
					Set_Channel_iw(m_pmChanCur->Channel);
				//				SetStrFormat(m_pmChanCur->Channel, m_pmParCur->decibel);	// мог измениться коэффициент усиления
				Set_Mera_iw(
					m_psrv->Conversion(m_pmChanCur->Channel));	//могла измениться размерность
			}
		}

		// теперь изменения в m_multi
		auto ic = g_pThread->GetIteratorBegin();
		for (auto it=m_multi.begin(); it!=m_multi.end(); ++it, ++ic)
			it->ReSize( it->numberPage * ic->NumberCounts);

		ReStart(false, false);
	}
}
//------------------------------------------------------------------------------
void CVoltMeter_AC::OnFirstModifySRV_0()
{// Первый Modify c GlobalPar == 0, могли измениться каналы
// ничего делать не буду
}
//------------------------------------------------------------------------------
void CVoltMeter_AC::OnReadyUnitParent(long par)
{
	long param;
	double value;
	m_punitParent->UnitParam(&param, &value);
	long val = (long)value;
	switch(param)
	{
	case 0:	 SetUnitParam_p0(val);	break;
	case 1:	 SetUnitParam_p1(val);	break;
	case 2:	 SetUnitParam_p2(val);	break;
	case 3:	 SetUnitParam_p3(val);	break;
	case -1:	 SetUnitParam_m1(val);	break;
	case -2: SetUnitParam_m2(val);	break;
	case -3: SetUnitParam_m3(val);	break;
	case -4: SetUnitParam_m4(val);	break;
	case -5: SetUnitParam_m5(val);	//break;
	}
}
//------------------------------------------------------------------------------
void CVoltMeter_AC::OnReadyUnit2Param()
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
	case 2:
		if (m_paramUnit2.type == tdu_32s)
			if (m_paramUnit2.Extract_val(val) == NOERROR)
				SetUnitParam_p2(val);
		break;
	case 3:
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
		//break;
	}
}
//------------------------------------------------------------------------------
void CVoltMeter_AC::SetUnitParam_p0(const long val)
{//установка номера канала
	if (m_bMulti)
	{
		if (m_pmChanCur->Channel != val)
		{
			StopAllThread();
			g_pThread->SetChannelNumber(val);
			ViewNewChanInWindows();
			if (m_pmChanCur->b7000)
			{
				Show7000();
				m_pmChanCur->bJob = false;
			}
			else
			{
				SetFast(m_pmParCur->fast);
				//				SetStrFormat(NULL, true);
			}
			SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
		}
		//	m_punitSrv->SetParamBack_32s(0, m_pmChanCur->Channel);
	}
	else
	{
		long id;
		m_psrv->IdChan(val, &id);
		m_sParamNew.IdChan = id;
		m_sParamNew.channel = val;
		OnStructureNewIfInit();
	}
}
//------------------------------------------------------------------------------
void CVoltMeter_AC::SetUnitParam_p1(const long val)
{//установка быстро, медленно, сверхмедленно
	if (m_bMulti)
	{
		if (m_pmParCur->fast != val)
		{
			StopAllThread();
			m_pmParCur->fast = m_pmChanCur->DataTime.TestFast(val);
			SetFast(m_pmParCur->fast);
			Set_Fast_iw(m_pmParCur->fast);
			SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
		}
		//	m_punitSrv->SetParamBack_32s(1, m_pmParCur->fast);
	}
	else
	{
		m_sParamNew.fast = val;
		OnStructureNewIfInit();
	}
}
//------------------------------------------------------------------------------
void CVoltMeter_AC::SetUnitParam_p2(const long val)
{// тип передаваемого значения в UNIT:
	// на работу рабочего потока этот параметр не влияет	
	// 0 - СКЗ, 1 - пиковое или 2 - амплитудное
	long num;
	if (val < 0)
		num = 0;
	else if (val > 2)
		num = 2;
	else
		num = val;
	m_pmParCur->typeValUnit = num;

	if (m_bMulti)
	{
		m_pmParCur->ampl = num / 2;
		Set_MeasureType_iw( CalcMeasureType(m_pmParCur->ampl, m_pmParCur->decibel) );
		Set_Mera_iw( m_psrv->Conversion(m_pmChanCur->Channel) );
		//		SetStrFormat(NULL, true);
		//	m_punitSrv->SetParamBack_32s(2, num);
	}
	else
	{
		m_sParamNew.ampl = num / 2;
		New_ampl_decibel(false);
	}
}
//------------------------------------------------------------------------------
void CVoltMeter_AC::SetUnitParam_p3(const long val)
{	//линейное (0) или децибельное (1)
	// на работу рабочего потока этот параметр не влияет
	long num = 0;
	if (m_bMulti)
	{
		if (val)
			num = 1;
		m_pmParCur->decibel = num;
		Set_MeasureType_iw(num);
		Set_Mera_iw(m_sDb);
		//		SetStrFormat(NULL, true);
		//	m_punitSrv->SetParamBack_32s(3, num);
	}
	else
	{
		m_sParamNew.decibel = 2 * m_pmParCur->ampl + m_pmParCur->decibel;
		New_ampl_decibel(true);
	}
}
//------------------------------------------------------------------------------
void CVoltMeter_AC::SetUnitParam_m1(const long val)
{// Переход в многоканальный режим. Первоначальное задание кол-ва каналов
	MultiParamSelf mlt;
	if (!m_bMulti)
	{
		if (val > 0)
		{
			m_bMulti = true;
			StopAllThread();
			for(long i=0; i<val - 1; ++i)	// один канал уже есть
			{
				m_multi.push_back(mlt);
				g_pThread->AddChannel(-1);
			}
			g_pThread->SetIndexCur(0);
			g_pThread->SetChannelNumber(-1);
			m_pmChanCur = g_pThread->GetPointerMultiChannelCur();
			m_pmParCur = &m_multi[ g_pThread->GetIndexCur() ];
			//Многоканальный режим работы программы. Кол-во каналов
			strTemp.Format(TranslateString(L"Многоканальный режим работы программы. Кол-во каналов") + L" = %d", val);
			MessageInformation(strTemp);
			SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
		}
		ViewNewChanInWindows();
		ResetOutData_iw();
		//	m_punitSrv->SetParamBack_32s( -1, (long)m_multi.size() );
	}
}
//------------------------------------------------------------------------------
void CVoltMeter_AC::SetUnitParam_m2(const long val)
{// задание текущего индекса канала для будущего изменения параметров
	if (m_bMulti)
	{
		long iOld = g_pThread->GetIndexCur();
		if ( g_pThread->SetIndexCur(val) )
		{
			long num = g_pThread->GetIndexCur();
			if ( iOld != num )
			{
				//L"Установка текущего индекса канала"
				strTemp.Format(TranslateString(L"Установка текущего индекса канала") + L" = %d",
					num);
				MessageInformation(strTemp);
				m_pmChanCur = g_pThread->GetPointerMultiChannelCur();
				m_pmParCur = &m_multi[ g_pThread->GetIndexCur() ];
				ViewNewChanInWindows();
				if (m_pmChanCur->b7000)
					Show7000();
				else
					ResetOutData_iw();
			}
		}
		//	m_punitSrv->SetParamBack_32s( -2, g_pThread->GetIndexCur() );
	}
}
//------------------------------------------------------------------------------
void CVoltMeter_AC::SetUnitParam_m3(const long val)
{// остановка канала
	if (m_bMulti)
	{
		if ( (0 <= val) && (val < (long)m_multi.size() ) )
		{
			StopAllThread();
			if ( g_pThread->StopChannel(val) )
			{// L"Остановка канала с индексом"
				strTemp.Format(TranslateString(L"Остановка канала с индексом") + L" = %d", val);
				MessageInformation(strTemp);
			}
			SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
			//	m_punitSrv->SetParamBack_32s(-3, val);
		}
		//	else
		//		m_punitSrv->SetParamBack_32s(-3, -1);
	}
}
//------------------------------------------------------------------------------
void CVoltMeter_AC::SetUnitParam_m4(const long val)
{// запуск канала
	if (m_bMulti)
	{
		if ( (0 <= val) && (val < (long)m_multi.size() ) )
		{
			StopAllThread();
			if ( g_pThread->StartChannel(val) )
			{// L"Запуск канала с индексом"
				strTemp.Format(TranslateString(L"Запуск канала с индексом") + L" = %d", val);
				MessageInformation(strTemp);
			}
			SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
			//	m_punitSrv->SetParamBack_32s(-3, val);
		}
		//	else
		//		m_punitSrv->SetParamBack_32s(-3, -1);
	}
}
//------------------------------------------------------------------------------
void CVoltMeter_AC::SetUnitParam_m5(const long val)
{ // добавление канала, он станет текущим
	static MultiParamSelf mlt;
	if (m_bMulti)
	{// добавление канала № num
		if ( (0 <= val) && (val < m_psrv->QuanChan()) )
		{
			StopAllThread();
			if ( g_pThread->AddChannel(val) != -1)
			{// добавление удачное (0) или добавлен не существующий канал (-2)
				// функция SetChannelNumber будет вызвана внутри AddChannel
				m_multi.push_back(mlt);
				m_pmParCur = &m_multi[ g_pThread->GetIndexCur() ];
				m_pmChanCur = g_pThread->GetPointerMultiChannelCur();

				if (m_pmChanCur->b7000)
				{
					Show7000();
					m_pmChanCur->bJob = false;
				}
				else if (m_pmChanCur->bExist)
				{
					DefaultMultiParam();
					SetFast(m_pmParCur->fast);
					//					SetStrFormat(NULL, true);
				}
				strTemp.Format(TranslateString(L"Добавлен канал") + L"%s",
					m_psrv->Commentary(val) );
				MessageInformation(strTemp);
				//	m_punitSrv->SetParamBack_32s(-5, m_pmChanCur->Channel);
			}
			else
			{
				//	m_punitSrv->SetParamBack_32s(-5, -1);
				m_pmChanCur = g_pThread->GetPointerMultiChannelCur();
				m_pmParCur = &m_multi[ g_pThread->GetIndexCur() ];
			}
			ViewNewChanInWindows();
			SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
		}
		//else
		//	m_punitSrv->SetParamBack_32s(-5, -1);
	}
}
//------------------------------------------------------------------------------
//void CVoltMeter_AC::WriteUnitResult_p0(
//	const long shift, const float val)
//{
//	m_punitSrv->WriteResult_32f(shift, val);
//}
//------------------------------------------------------------------------------
void CVoltMeter_AC::WriteUnitResult_p1(
	const long shift, WideResult<float> *pval)
{
	m_punitSrv->WriteResult_wrs_32f(shift + 1, pval);
}
//------------------------------------------------------------------------------