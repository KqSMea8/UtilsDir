//------------------------------------------------------------------------------
//		������ ������������� ������ CDelegateRet
//------------------------------------------------------------------------------
//		������ ����� ������ � �������, ����� ��������� �������� ������
// ������� �� ���������� �������. �/��� ����� ���������� �������
// ����������� ������ ������� �� ������� �������.
//		������� ����� �������� � �� ������ �������� (��� � ���������� �������).
// ����� �������� ����� ������� ������-�� ������ �� ������������� �����.
//------------------------------------------------------------------------------
//		� ���������� ����� ���� ���������� ������ ����������� ���� �����
// CDelegateRet ������������ ��� ������������ ������ ��������� �� ���� �������
// ������ CSimple.
//		� ��� ����� � �� �������� ������, ������� ���������� ��� ������ ����� �
// ���������� ����������� ������ ������.
//------------------------------------------------------------------------------
// Event_234Dlg.h : ���� ���������
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
	{	// ������� ����� ����������, ��� Sound_void_void(NULL).
		// ��� PVOID, � ���������,  �������� �� �������
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
	�DelegateRet<void, par> *pde_void__par;
};
//--------------------------------------
// ���������� ���� CEvent_234Dlg
class CEvent_234Dlg : public CDialogEx
{
private:
	globParam m_gp;
	CSimple m_smp;
	CWinThread *m_pwt;
	�DelegateRet<void, void> m_de_void_void;
	�DelegateRet<void, par>  m_de_void_par;
	�DelegateRet<int, void>  m_de_int_void;
	�DelegateRet<int, par>   m_de_int_par;
public:
	CEvent_234Dlg(CWnd* pParent = NULL);	// ����������� �����������
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
// Event_234Dlg.cpp : ���� ����������
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
// ���������� ���� CEvent_234Dlg
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
	// ������� �� ���������� ������
	SetEvent(m_gp.hEvent[1]);
	// ���, ����� ����� ����������
	WaitForSingleObject(m_pwt->m_hThread, 500);
	// ������� ������
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

	// ������� ������� ������ ������� �������
	m_gp.hEvent[0] = CreateEventW(NULL, FALSE, FALSE, NULL);
	// ������� ������� ������ ��������� ������
	m_gp.hEvent[1] = CreateEventW(NULL, TRUE, FALSE, NULL);

	// �������
	m_gp.pde_void__par = new �DelegateRet<void, par> (&m_smp, &CSimple::Sound_void_par);

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
{// ������� �������� ������
	SetEvent(m_gp.hEvent[0]);
}
*/
//------------------------------------------------------------------------------