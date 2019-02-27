#pragma once

class CProgressCtrlEx : public CProgressCtrl
{
	DECLARE_DYNAMIC( CProgressCtrlEx )

public:
	CProgressCtrlEx();
	virtual ~CProgressCtrlEx();

protected:
	DECLARE_MESSAGE_MAP()

public:
	void	SetShowText( CString c_str );
	void	SetShowText( UINT nID );
	CString	GetShowText();
	void	SetZeroRange( int range );
	void	SetPosition( int pos );
	void	Indeterminate( BOOL bInf = TRUE );
	void	Pause();
	void	Error();
	void	SetTextColor(const COLORREF color);	// Цвет текста

protected:
	afx_msg void OnPaint();
	COLORREF		m_colorText;
	ITaskbarList3*	pTaskbarList = nullptr;
};
