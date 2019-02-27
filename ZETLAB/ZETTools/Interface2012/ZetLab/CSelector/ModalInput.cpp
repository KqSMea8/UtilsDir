// digout.cpp : implementation file
//

#include "stdafx.h"
#include "ModalInput.h"


// digout dialog
ModalInput::ModalInput(CWnd* pParent /*=NULL*/)
	: CDialogEx(ModalInput::IDD, pParent)
{
	m_pEdit = NULL;
}

ModalInput::~ModalInput()
{
	if(m_pEdit != NULL)
		delete m_pEdit;
}


BEGIN_MESSAGE_MAP(ModalInput, CDialogEx)
ON_WM_RBUTTONUP()
ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// digout message handlers

BOOL ModalInput::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_pEdit = new CEdit();
	m_pEdit->Create(ES_CENTER | ES_AUTOHSCROLL, CRect(3,2,52,12), this, 1000);
	m_pEdit->SetWindowText(m_TextOut);

	RECT MyRect;
	GetWindowRect(&MyRect);
	MoveWindow(m_point.x,m_point.y,MyRect.right-MyRect.left,MyRect.bottom-MyRect.top,1);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void ModalInput::OnOK()
{
	m_pEdit->GetWindowText(m_TextOut);
	CDialogEx::OnOK();
}

void ModalInput::OnRButtonUp(UINT nFlags, CPoint point)
{
	OnCancel();
	CDialogEx::OnRButtonUp(nFlags, point);
}

void ModalInput::OnLButtonUp(UINT nFlags, CPoint point)
{
	OnCancel();
	CDialogEx::OnLButtonUp(nFlags, point);
}
