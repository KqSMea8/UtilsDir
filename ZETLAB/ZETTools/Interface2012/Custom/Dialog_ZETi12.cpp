//------------------------------------------------------------------------------
// CDialog_ZETi12.cpp : файл реализации
//
//------------------------------------------------------------------------------
//подключение стандартных заголовков
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Custom\Dialog_ZETi12.h"
//------------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//------------------------------------------------------------------------------
// конструктор диалогового окна CDialog_ZETi12
CDialog_ZETi12::CDialog_ZETi12(
	UINT nIDTemplate,			// идентификатор программы
	CWnd* pParent,				// указатель на владельца
	CString exeName,			// имя exe-файла программы
	CString cfgFilePreamble,	// имя преамбулы конфигурационного файла
	ULONG nZPriority,			// приоритет программы в Zet-панели (от 0 до 3)
	bool needSRV,				// нужен сервер
	long numVirtChan,			// кол-во создаваемых виртуальных каналов
	bool needDAC,				// нужен ЦАП
	long numDAC)				// номер используемого ЦАП
	: CDialog_ZET(nIDTemplate, pParent, exeName, cfgFilePreamble,
			nZPriority, needSRV, numVirtChan, needDAC, numDAC)
{
	m_pMainContainer = NULL;
	m_pAutoSliderParam = NULL;
	m_pAutoSliderData = NULL;

 	m_pDataContainer = nullptr;
	m_pParamContainer = nullptr;
	m_pMainContainer = nullptr;

	nIDEventTimerGDI = 101;
	m_ElapseTimerGDI = 16;
	
	m_bMoveParam = false;
	m_bMoveData = false;
	
	m_dTimeMoveParam = 0.5;
	m_dTimeMoveData = 0.5;
	
	flag_Param = false;
	flag_Data = false;
}
//------------------------------------------------------------------------------
CDialog_ZETi12::~CDialog_ZETi12()
{

}
//------------------------------------------------------------------------------
void CDialog_ZETi12::DoDataExchange(CDataExchange* pDX)
{
	CDialog_ZET::DoDataExchange(pDX);
}
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CDialog_ZETi12, CDialog_ZET)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_TIMER()
	ON_WM_MOVE()
END_MESSAGE_MAP()
//------------------------------------------------------------------------------
BOOL CDialog_ZETi12::OnInitDialog()
{
	CDialog_ZET::OnInitDialog();
	m_sFullFolderPNG = m_sFullFolderInstLoc + L"template\\png\\";
#ifdef _GDIPLUS
	// Инициализация Gdiplus
	Gdiplus::GdiplusStartupInput  input;
	GdiplusStartup( &token, &input, NULL );
#endif
	// Инициализация QueryPerformanceCounter
	m_dCurTime = 0.;
	m_pTimeDraw = &m_dCurTime;
	LARGE_INTEGER li;
    if( QueryPerformanceFrequency(&li) )
	{
		m_dTicksPerSec = double(li.QuadPart);
		QueryPerformanceCounter(&li);
		m_llStartTime = li.QuadPart;
	}
	else
		MessageError(
		L"Hardware implementation of the computer does not support QueryPerformanceCounter",
				MSG_CONFIG_ERROR);

	SetTimer(nIDEventTimerGDI, m_ElapseTimerGDI, NULL);

	// переменные
	return TRUE;
}
//------------------------------------------------------------------------------
void CDialog_ZETi12::OnDestroy()
{
	StopServerExchange();

	if (m_pDataContainer)
 		delete m_pDataContainer;
	if (m_pParamContainer)
		delete m_pParamContainer;
	if (m_pMainContainer)
		delete m_pMainContainer;

#ifdef _GDIPLUS
	Gdiplus::GdiplusShutdown(token);
#endif
	CDialog_ZET::OnDestroy();
}
//------------------------------------------------------------------------------
void CDialog_ZETi12::OnCancel()
{// по кнопке Esc отображаю окно параметров
	/*if (m_pAutoSliderParam)
	{
		BOOL val = m_pAutoSliderParam->GetPositionValue();
		if (val)
			val = 0;
		else
			val = 1;
		// посылка слайдеру инверсного значения
		m_pAutoSliderParam->SetPositionValue(val);
	}
	if (m_pAutoSliderData)
	{
		BOOL val = m_pAutoSliderData->GetPositionValue();
		if (val)
			val = 0;
		else
			val = 1;
		// посылка слайдеру инверсного значения
		m_pAutoSliderData->SetPositionValue(val);
	} */
	CDialog_ZET::OnCancel();
}
//------------------------------------------------------------------------------
void CDialog_ZETi12::OnTimer(UINT_PTR nIDEvent)
{
//	RECT rectP;
	if ( (nIDEvent == nIDEventTimerGDI) && m_pMainContainer )
	{
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		m_dCurTime = double(li.QuadPart - m_llStartTime) / m_dTicksPerSec;

		if (m_bMoveParam && m_pParamContainer)
		{// анимация окна данных
			CRect rect0;
			double CurTime = (m_dCurTime - m_dTimeStartParam) / m_dTimeMoveParam;
			rect0 = m_pParamContainer->m_rect;
			if (CurTime < 1.)
			{// анимация в процессе
				int dx;
				rect0 = m_pParamContainer->m_rect;
				if (!flag_Param)	// окно появляется, выезжая вниз из окна
					dx = int(rect0.Width() * CurTime);
				else					// окно убирается, въезжая вверх под окно
					dx = int(rect0.Width() * (1 - CurTime));
				rect0.right = rect0.right - dx;
				m_pMainContainer->SetParamRect(rect0);
			}
			else
			{// анимация завершена
				m_bMoveParam = false;
				if (flag_Param)	// окно полностью выехало вниз
				// отображаю окно в окончательном положении
					m_pMainContainer->SetParamRect(m_pParamContainer->m_rect);
				else
				{// окно полностью заехало под главное окно
					rect0.right = rect0.left;
					m_pMainContainer->SetParamRect(rect0);
				}
			}
			m_pMainContainer->OnTimer();
		}

		if (m_bMoveData && m_pDataContainer)
		{// анимация окна данных
			CRect rect;
			double CurTime = (m_dCurTime - m_dTimeStartData) / m_dTimeMoveData;
			rect = m_pDataContainer->m_rect;
			if (CurTime < 1.)
			{// анимация в процессе
				int dy;
				rect = m_pDataContainer->m_rect;
				if (!flag_Data)	// окно появляется, выезжая вниз из окна
					dy = int(rect.Height() * CurTime);
				else					// окно убирается, въезжая вверх под окно
					dy = int(rect.Height() * (1 - CurTime));
				rect.bottom = rect.bottom - dy;
				m_pMainContainer->SetDataRect(rect);
			}
			else
			{// анимация завершена
				m_bMoveData = false;
				if (flag_Data)	// окно полностью выехало вниз
				// отображаю окно в окончательном положении
					m_pMainContainer->SetDataRect(m_pDataContainer->m_rect);
				else
				{// окно полностью заехало под главное окно
					rect.bottom = rect.top;
					m_pMainContainer->SetDataRect(rect);
				}
			}
			m_pMainContainer->OnTimer();
		}
#ifdef _GDIPLUS
		m_pMainContainer->OnTimer();
#else
		//wglMakeCurrent(pDC->GetSafeHdc(), hrc);
		//m_pMainContainer->OnTimer();
		//m_pMainContainer->OnDraw();
#endif
	}
	else
		CDialog_ZET::OnTimer(nIDEvent);
}
//------------------------------------------------------------------------------
bool CDialog_ZETi12::SetAutoSliderParam(CSimpleSlider *pSlider)
{// задание слайдера, отвечающего за визуализацию окна параметров
	bool ret;
	if (pSlider)
	{
		m_pAutoSliderParam = pSlider;
		m_pAutoSliderParam->m_FunctionEvent =
			&CCustom_ZETi12::OnNewDouble_SliderParam;
		ret = true;
	}
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
bool CDialog_ZETi12::SetAutoSliderData(CSimpleSlider *pSlider)
{// задание слайдера, отвечающего за визуализацию окна данных
	bool ret;
	if (pSlider)
	{
		m_pAutoSliderData = pSlider;
		m_pAutoSliderData->m_FunctionEvent =
			&CCustom_ZETi12::OnNewDouble_SliderData;
		ret = true;
	}
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
void CDialog_ZETi12::OnNewDouble_SliderParam(double val)
{// обработка события слайдера, отвечающего за визуализацию окна параметров
	if (m_pParamContainer)
	{
		m_bMoveParam = true;
		m_dTimeStartParam = m_dCurTime;
		if (val == 1)
			flag_Param = true;
		else
			flag_Param = false;
	}
}
//------------------------------------------------------------------------------
void CDialog_ZETi12::OnNewDouble_SliderData(double val)
{// обработка события слайдера, отвечающего за визуализацию окна данных
	if (m_pDataContainer)
	{
		m_bMoveData = true;
		m_dTimeStartData = m_dCurTime;
		if (val == 1)
			flag_Data = true;
		else
			flag_Data = false;
	}
}
//------------------------------------------------------------------------------
bool CDialog_ZETi12::SetAutoButtonParam(CZButton *pButton)
{// задание слайдера, отвечающего за визуализацию окна параметров
	bool ret;
	if (pButton)
	{
		m_pAutoButtonData = pButton;
		m_pAutoButtonData->m_FunctionEvent = &CCustom_ZETi12::OnNewDouble_ButtonParam;
		ret = true;
	}
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
bool CDialog_ZETi12::SetAutoButtonData(CZButton *pButton)
{// задание слайдера, отвечающего за визуализацию окна данных
	bool ret;
	if (pButton)
	{
		m_pAutoButtonData = pButton;
		m_pAutoButtonData->m_FunctionEvent = &CCustom_ZETi12::OnNewDouble_ButtonData;
		ret = true;
	}
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
void CDialog_ZETi12::OnNewDouble_ButtonParam(void)
{// обработка события кнопки, отвечающего за визуализацию окна параметров
	if (m_pParamContainer)
	{
		m_bMoveParam = true;
		m_dTimeStartParam = m_dCurTime;
		if (flag_Param)
			flag_Param = false;
		else
			flag_Param = true;

	}
}
//------------------------------------------------------------------------------
void CDialog_ZETi12::OnNewDouble_ButtonData(void)
{// обработка события кнопки, отвечающего за визуализацию окна данных
	if (m_pDataContainer)
	{
		m_bMoveData = true;
		m_dTimeStartData = m_dCurTime;
		if (flag_Data)
			flag_Data = false;
		else
			flag_Data = true;

	}
}
//------------------------------------------------------------------------------
bool CDialog_ZETi12::GetParamFlag(void)
{
	return flag_Param;
}
//------------------------------------------------------------------------------
bool CDialog_ZETi12::GetDataFlag(void)
{
	return flag_Data;
}
//------------------------------------------------------------------------------

/// Server Exchange //////////////////////////////////////////////////////

bool CDialog_ZETi12::InitializeServerExchange()
{
	CString commandLine = GetCommandLine();
	int thinFind = commandLine.Find(L"-thin");
	if(thinFind >= 0)
	{
		CString guidString = commandLine.Mid(thinFind + 5, 36);
		if(_serverExchanger.Initialize(m_hWnd, m_pMainContainer, guidString))
		{
			_serverExchanger.Exchange();
			return true;
		}
	}
	return false;
}
void CDialog_ZETi12::StopServerExchange()
{
	_serverExchanger.Disconnect();
}

//////////////////////////////////////////////////////////////////////////
