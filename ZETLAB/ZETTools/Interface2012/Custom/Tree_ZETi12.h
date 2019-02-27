//------------------------------------------------------------------------------
// Tree_ZETi12.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Custom\Custom_ZETi12.h"
//#include "C:\ZETTools\Interface2012\ZetLab\ChanPanel.h"
//------------------------------------------------------------------------------
class CTree_ZETi12 : public CCustom_ZETi12
{
public:
	CTree_ZETi12(UINT nIDTemplate, CWnd* pMainWnd);
	virtual ~CTree_ZETi12();

private:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	CCustom_ZETi12 *m_pMainWnd;			// главное окно программы
	//CChanPanel *m_pMainContainer;

	DECLARE_MESSAGE_MAP()

public:
	void OnDestroy();
	void Init(CString sFullFolderPNG, double *pTimeDraw);
	void OnLChannel(CString str);
};
//------------------------------------------------------------------------------
