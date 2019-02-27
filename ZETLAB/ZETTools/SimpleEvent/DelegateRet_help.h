//------------------------------------------------------------------------------
//		Пример использования класса CDelegateRet
//------------------------------------------------------------------------------
//		Данный класс удобен в случаях, когда требуется вызывать разные
// функции из некоторого перечня. И/или когда вызываемые функции
// принадлежат разным классам из перечня классов.
//		Перечни могут состоять и из одного элемента (как в приведённом примере).
// Тогда получаем вызов функции какого-то класса из произвольного места.
//------------------------------------------------------------------------------
//		В приведённой части кода некоторого класса диалогового окна класс
// CDelegateRet используется для демонстрации вызова различных по типу функций
// класса CSimple.
//		В том числе и из рабочего потока, который реализован как ждущий поток с
// корректным завершением работы потока.
//------------------------------------------------------------------------------
// Event_234Dlg.h : файл заголовка
/*
#pragma once
#include "SimpleEvent\DelegateRet.h"
struct par
{
	long freq;
	long time;
};

class CSimple
{
public:
	CSimple() {}
	void Sound_void_void(PVOID)
	{	// Функция будет вызываться, как Sound_void_void(NULL).
		// Без PVOID, к сожалению,  обойтись не удалось
		Beep(1000, 100);
	}
	void Sound_void_par(par *p)
	{
		Beep(p->freq, p->time);
	}
	int Sound_int_void(PVOID)
	{
		Beep(1000, 100);
		return 0;
	}
	int Sound_int_par(par *p)
	{
		Beep(p->freq, p->time);
		return 0;
	}
};

struct globParam
{
	HANDLE hEvent[2];
	СDelegateRet<void, par> *pde_void__par;
};
//--------------------------------------
// диалоговое окно CEvent_234Dlg
class CEvent_234Dlg : public CDialogEx
{
private:
	globParam m_gp;
	CSimple m_smp;
	CWinThread *m_pwt;
	СDelegateRet<void, void> m_de_void_void;
	СDelegateRet<void, par>  m_de_void_par;
	СDelegateRet<int, void>  m_de_int_void;
	СDelegateRet<int, par>   m_de_int_par;
public:
	CEvent_234Dlg(CWnd* pParent = NULL);	// стандартный конструктор
	virtual ~CEvent_234Dlg();
protected:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
};
//------------------------------------------------------------------------------
// Event_234Dlg.cpp : файл реализации
//
#include "stdafx.h"
#include "Event_234.h"
#include "Event_234Dlg.h"
#include "afxdialogex.h"
//------------------------------------------------------------------------------
UINT GlobalFunctionThread(LPVOID pParam)
{
	par s = {1750, 100};
	globParam *p = (globParam*)pParam;
	bool flag = true;
	while(flag)
	{
		switch ( WaitForMultipleObjects(2, p->hEvent, FALSE, INFINITE) )
		{
		case WAIT_OBJECT_0:
			(*p->pde_void__par)(&s);
			break;
		case WAIT_OBJECT_0 + 1:
			flag = false;
		}
	}
	return 0;
}
//------------------------------------------------------------------------------
// диалоговое окно CEvent_234Dlg
CEvent_234Dlg::CEvent_234Dlg(CWnd* pParent)
	: CDialogEx(CEvent_234Dlg::IDD, pParent)
	, m_de_void_void(&m_smp, &CSimple::Sound_void_void)
	, m_de_void_par(&m_smp, &CSimple::Sound_void_par)
	, m_de_int_void(&m_smp, &CSimple::Sound_int_void)
	, m_de_int_par(&m_smp, &CSimple::Sound_int_par)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CEvent_234Dlg::~CEvent_234Dlg()
{
	// команда на завершение потока
	SetEvent(m_gp.hEvent[1]);
	// жду, когда поток завершится
	WaitForSingleObject(m_pwt->m_hThread, 500);
	// очистка памяти
	CloseHandle(m_gp.hEvent[0]);
	CloseHandle(m_gp.hEvent[1]);
	delete m_gp.pde_void__par;
}

BEGIN_MESSAGE_MAP(CEvent_234Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CEvent_234Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CEvent_234Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CEvent_234Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CEvent_234Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CEvent_234Dlg::OnBnClickedButton5)
END_MESSAGE_MAP()

BOOL CEvent_234Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// событие команды потоку вызвать функцию
	m_gp.hEvent[0] = CreateEventW(NULL, FALSE, FALSE, NULL);
	// событие команды потоку завершить работу
	m_gp.hEvent[1] = CreateEventW(NULL, TRUE, FALSE, NULL);

	// делегат
	m_gp.pde_void__par = new СDelegateRet<void, par> (&m_smp, &CSimple::Sound_void_par);

	m_pwt = AfxBeginThread( GlobalFunctionThread, &m_gp );

	return TRUE;
}
//---------------------------------------
void CEvent_234Dlg::OnBnClickedButton1()
{
	m_de_void_void();
}

void CEvent_234Dlg::OnBnClickedButton2()
{
	par s = {1500, 100};
	m_de_void_par(&s);
}

void CEvent_234Dlg::OnBnClickedButton3()
{
	int r = m_de_int_void();
}

void CEvent_234Dlg::OnBnClickedButton4()
{
	par s = {2000, 100};
	int r = m_de_int_par(&s);
}

void CEvent_234Dlg::OnBnClickedButton5()
{// команда рабочему потоку
	SetEvent(m_gp.hEvent[0]);
}
*/
//------------------------------------------------------------------------------