//------------------------------------------------------------------------------
// CDialog_ZETi12.cpp : ���� ����������
//
//------------------------------------------------------------------------------
//����������� ����������� ����������
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Custom\Dialog_ZETi12.h"
//------------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//------------------------------------------------------------------------------
// ����������� ����������� ���� CDialog_ZETi12
CDialog_ZETi12::CDialog_ZETi12(
	UINT nIDTemplate,			// ������������� ���������
	CWnd* pParent,				// ��������� �� ���������
	CString exeName,			// ��� exe-����� ���������
	CString cfgFilePreamble,	// ��� ��������� ����������������� �����
	ULONG nZPriority,			// ��������� ��������� � Zet-������ (�� 0 �� 3)
	bool needSRV,				// ����� ������
	long numVirtChan,			// ���-�� ����������� ����������� �������
	bool needDAC,				// ����� ���
	long numDAC)				// ����� ������������� ���
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
	// ������������� Gdiplus
	Gdiplus::GdiplusStartupInput  input;
	GdiplusStartup( &token, &input, NULL );
#endif
	// ������������� QueryPerformanceCounter
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

	// ����������
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
{// �� ������ Esc ��������� ���� ����������
	/*if (m_pAutoSliderParam)
	{
		BOOL val = m_pAutoSliderParam->GetPositionValue();
		if (val)
			val = 0;
		else
			val = 1;
		// ������� �������� ���������� ��������
		m_pAutoSliderParam->SetPositionValue(val);
	}
	if (m_pAutoSliderData)
	{
		BOOL val = m_pAutoSliderData->GetPositionValue();
		if (val)
			val = 0;
		else
			val = 1;
		// ������� �������� ���������� ��������
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
		{// �������� ���� ������
			CRect rect0;
			double CurTime = (m_dCurTime - m_dTimeStartParam) / m_dTimeMoveParam;
			rect0 = m_pParamContainer->m_rect;
			if (CurTime < 1.)
			{// �������� � ��������
				int dx;
				rect0 = m_pParamContainer->m_rect;
				if (!flag_Param)	// ���� ����������, ������� ���� �� ����
					dx = int(rect0.Width() * CurTime);
				else					// ���� ���������, ������� ����� ��� ����
					dx = int(rect0.Width() * (1 - CurTime));
				rect0.right = rect0.right - dx;
				m_pMainContainer->SetParamRect(rect0);
			}
			else
			{// �������� ���������
				m_bMoveParam = false;
				if (flag_Param)	// ���� ��������� ������� ����
				// ��������� ���� � ������������� ���������
					m_pMainContainer->SetParamRect(m_pParamContainer->m_rect);
				else
				{// ���� ��������� ������� ��� ������� ����
					rect0.right = rect0.left;
					m_pMainContainer->SetParamRect(rect0);
				}
			}
			m_pMainContainer->OnTimer();
		}

		if (m_bMoveData && m_pDataContainer)
		{// �������� ���� ������
			CRect rect;
			double CurTime = (m_dCurTime - m_dTimeStartData) / m_dTimeMoveData;
			rect = m_pDataContainer->m_rect;
			if (CurTime < 1.)
			{// �������� � ��������
				int dy;
				rect = m_pDataContainer->m_rect;
				if (!flag_Data)	// ���� ����������, ������� ���� �� ����
					dy = int(rect.Height() * CurTime);
				else					// ���� ���������, ������� ����� ��� ����
					dy = int(rect.Height() * (1 - CurTime));
				rect.bottom = rect.bottom - dy;
				m_pMainContainer->SetDataRect(rect);
			}
			else
			{// �������� ���������
				m_bMoveData = false;
				if (flag_Data)	// ���� ��������� ������� ����
				// ��������� ���� � ������������� ���������
					m_pMainContainer->SetDataRect(m_pDataContainer->m_rect);
				else
				{// ���� ��������� ������� ��� ������� ����
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
{// ������� ��������, ����������� �� ������������ ���� ����������
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
{// ������� ��������, ����������� �� ������������ ���� ������
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
{// ��������� ������� ��������, ����������� �� ������������ ���� ����������
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
{// ��������� ������� ��������, ����������� �� ������������ ���� ������
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
{// ������� ��������, ����������� �� ������������ ���� ����������
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
{// ������� ��������, ����������� �� ������������ ���� ������
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
{// ��������� ������� ������, ����������� �� ������������ ���� ����������
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
{// ��������� ������� ������, ����������� �� ������������ ���� ������
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
