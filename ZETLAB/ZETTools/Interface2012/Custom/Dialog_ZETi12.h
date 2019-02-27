//------------------------------------------------------------------------------
// CDialog_ZETi12.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Dialog_ZET\Dialog_ZET.h"
#include "C:\ZETTools\Interface2012\ZetLab\Main.h"
#include "C:\ZETTools\Interface2012\ZetLab\MainData.h"
#include "C:\ZETTools\Interface2012\ZetLab\MainParam.h"
#include "C:\ZETTools\Interface2012\ZetLab\SimpleSlider.h"
#include "C:\ZETTools\Interface2012\ZetLab\Button.h"
#include "C:\ZETTools\Interface2012\Custom\ServerExchange\ServerExchanger.h"
//------------------------------------------------------------------------------
// диалоговое окно CDialog_ZETi12
class CDialog_ZETi12 : public CDialog_ZET
{
public:
	CDialog_ZETi12(UINT nIDTemplate, CWnd* pParent,
				CString exeName,
				CString cfgFilePreamble, ULONG nZPriority,
				bool needSRV = true,  long numVirtChan = 0,
				bool needDAC = false, long numDAC = 0);
	virtual ~CDialog_ZETi12();
	void OnDestroy();
#ifdef _GDIPLUS
#else
	HGLRC hrc;
	CDC* pDC;
#endif
private: 
	int m_WidthParam, m_HightParam, m_dyParam, m_HightData, m_WidthData,m_dxData, m_WidthTree, m_HightTree;
	double m_dCurTime;
	double m_dTicksPerSec;
	LONGLONG m_llStartTime;

	bool m_bMoveParam;
	bool flag_Param;
	double m_dTimeStartParam, m_dTimeMoveParam;

	bool m_bMoveData;
	bool flag_Data;
	double m_dTimeStartData, m_dTimeMoveData;
	
protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnCancel();

	void GetFocus();
	CMain *m_pMainContainer;
	CMainData *m_pDataContainer;	// главный контейнер окна данных
	CMainParam *m_pParamContainer;	// главный контейнер окна данных
	bool SetAutoSliderParam(CSimpleSlider *pSlider);
	bool SetAutoSliderData(CSimpleSlider *pSlider);
	
	bool InitializeServerExchange();
	void StopServerExchange();

	ServerExchanger _serverExchanger;

	ULONG_PTR token;
	UINT_PTR nIDEventTimerGDI;
	UINT m_ElapseTimerGDI;

	DECLARE_MESSAGE_MAP()

public:
	bool m_bParamIsVisible;
	HICON m_hIcon;

	virtual double* GetPointerToTime() { return &m_dCurTime; }
	virtual double* GetPositionType() { return &m_dCurTime; }
	virtual double GetTime() { return m_dCurTime; }
	CSimpleSlider *m_pAutoSliderParam;
	CSimpleSlider *m_pAutoSliderData;
	CZButton *m_pAutoButtonParam;
	CZButton *m_pAutoButtonData;

	virtual void OnNewDouble_SliderParam(double val);
	virtual void OnNewDouble_SliderData(double val);
	bool SetAutoButtonParam(CZButton *pButton);
	bool SetAutoButtonData(CZButton *pButton);
	void OnNewDouble_ButtonParam(void);
	void OnNewDouble_ButtonData(void);
	bool GetParamFlag(void);
	bool GetDataFlag(void);
};
//------------------------------------------------------------------------------