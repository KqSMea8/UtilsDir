//------------------------------------------------------------------------------
// Data_ZETi12.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Custom\Custom_ZETi12.h"
#include "C:\ZETTools\Interface2012\ZetLab\MainData.h"
//------------------------------------------------------------------------------
class CData_ZETi12 : public CCustom_ZETi12
{
public:
	CData_ZETi12(UINT nIDTemplate, CWnd* pMainWnd);
	virtual ~CData_ZETi12();

private:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	CCustom_ZETi12 *m_pMainWnd;			// главное окно программы
	CMainData *m_pMainContainer;	// главный контейнер окна данных

#ifdef _GDIPLUS
#else
	HGLRC hrc_data;
	CDC* pDC_data;
#endif
	DECLARE_MESSAGE_MAP()

public:
	void OnDestroy();
	void Init(CString sFullFolderPNG, double *pTimeDraw);
	void OnTimerDraw();
};
//------------------------------------------------------------------------------