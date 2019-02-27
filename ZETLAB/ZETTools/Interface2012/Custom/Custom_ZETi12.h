//------------------------------------------------------------------------------
// Custom_ZETi12.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include <afxdisp.h>
#include "afxwin.h"
#include "afxdialogex.h"
#include <vector>
#include "C:\ZETTools\Interface2012\Custom\CustomMain.h"
//------------------------------------------------------------------------------
class CCustom_ZETi12 : public CDialogEx
{
public:
	CCustom_ZETi12(){}
	CCustom_ZETi12(UINT nIDTemplate, CWnd* pParent);
	virtual ~CCustom_ZETi12();
	void OnDestroy();

private:

protected:
	CCustomAll *m_pOwnerMouse;
	CCustomMain *m_pCustomMain;
	DWORD m_CurNumberTabFocus;
	std::vector<CCustomAll*> m_VectorMenu;
	std::vector<CCustomAll*> m_VectorFocus;
	double *m_pTimeDraw;

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	
	void AddTabFocus(PVOID pClass);
	void RegistrationMain(CCustomMain *pClass) { m_pCustomMain = pClass; }

	DECLARE_MESSAGE_MAP()

public:
	CString m_sFullFolderPNG;
	void RegistrationOwnerMouse(PVOID pClass);
	void RegistrationFocus(PVOID pClass);
	void RegistrationMenu(PVOID pClass);
	virtual double* GetPointerToTime() { return m_pTimeDraw; }
	virtual double GetTime() { return *m_pTimeDraw; }
	virtual void Resize(){};
	
	afx_msg void OnNewMenuItems(UINT nID);
	afx_msg LRESULT OnNewMenuItems2(WPARAM wparam, LPARAM lparam);
	// Функции-обработчики событий
	virtual void OnNotifyEvent_0(){}
	virtual void OnNotifyEvent_1(){}
	virtual void OnNotifyEvent_2(){}
	virtual void OnNotifyEvent_3(){}
	virtual void OnNotifyEvent_4(){}
	virtual void OnNotifyEvent_5(){}
	virtual void OnNotifyEvent_6(){}
	virtual void OnNotifyEvent_7(){}
	virtual void OnNotifyEvent_8(){}
	virtual void OnNotifyEvent_9(){}
	virtual void OnNotifyEvent_10(){}
	virtual void OnNotifyEvent_11(){}
	virtual void OnNotifyEvent_12(){}
	virtual void OnNotifyEvent_13(){}
	virtual void OnNotifyEvent_14(){}
	virtual void OnNotifyEvent_15(){}
	virtual void OnNotifyEvent_16(){}
	virtual void OnNotifyEvent_17(){}
	virtual void OnNotifyEvent_18(){}
	virtual void OnNotifyEvent_19(){}
	virtual void OnNotifyEvent_20(){}
	virtual void OnNotifyEvent_21(){}
	virtual void OnNotifyEvent_22(){}
	virtual void OnNotifyEvent_23(){}
	virtual void OnNotifyEvent_24(){}
	virtual void OnNotifyEvent_25(){}
	virtual void OnNotifyEvent_26(){}
	virtual void OnNotifyEvent_27(){}
	virtual void OnNotifyEvent_28(){}
	virtual void OnNotifyEvent_29(){}
	virtual void OnNotifyEvent_30(){}
	virtual void OnNewLong_0(long val){}
	virtual void OnNewLong_1(long val){}
	virtual void OnNewLong_2(long val){}
	virtual void OnNewLong_3(long val){}
	virtual void OnNewLong_4(long val){}
	virtual void OnNewLong_5(long val){}
	virtual void OnNewLong_6(long val){}
	virtual void OnNewLong_7(long val){}
	virtual void OnNewLong_8(long val){}
	virtual void OnNewLong_9(long val){}
	virtual void OnNewLong_10(long val){}
	virtual void OnNewLong_11(long val){}
	virtual void OnNewLong_12(long val){}
	virtual void OnNewLong_13(long val){}
	virtual void OnNewDouble_0(double val){}
	virtual void OnNewDouble_1(double val){}
	virtual void OnNewDouble_2(double val){}
	virtual void OnNewDouble_3(double val){}
	virtual void OnNewDouble_4(double val){}
	virtual void OnNewDouble_5(double val){}
	virtual void OnNewDouble_6(double val){}
	virtual void OnNewDouble_7(double val){}
	virtual void OnNewLongDouble_0(long lVal, double dVal){}
	virtual void OnNewLongDouble_1(long lVal, double dVal){}
	virtual void OnNewLongDouble_2(long lVal, double dVal){}
	virtual void OnNewLongDouble_3(long lVal, double dVal){}
	virtual void OnNewLongDouble_4(long lVal, double dVal){}
	virtual void OnNewLongDouble_5(long lVal, double dVal){}
	virtual void OnNewLongDouble_6(long lVal, double dVal){}
	virtual void OnNewLongDouble_7(long lVal, double dVal){}

	// Функции-обработчики стандартных событий
	virtual void OnNewDouble_SliderParam(double val){}
	virtual void OnNewDouble_SliderData(double val){}
	virtual void OnNewDouble_ButtonParam(void){}
	virtual void OnNewDouble_ButtonData(void){}
};
//------------------------------------------------------------------------------