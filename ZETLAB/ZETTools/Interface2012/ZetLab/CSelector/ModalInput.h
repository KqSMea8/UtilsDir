#pragma once
#include "afxwin.h"


// digout dialog

#define TEXT_OUT_LENGTH 200			
class ModalInput : public CDialogEx
{

public:
	ModalInput(CWnd* pParent = NULL);   // standard constructor
	virtual ~ModalInput();
	CString m_TextOut;
	CPoint m_point;

	enum { IDD = 999 };

protected:

	DECLARE_MESSAGE_MAP()
public:
	CEdit *m_pEdit;
	virtual BOOL OnInitDialog();
//	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
protected:
	virtual void OnOK();
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
