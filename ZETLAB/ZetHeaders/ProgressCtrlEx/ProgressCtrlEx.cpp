#include "stdafx.h"
#include "ProgressCtrlEx.h"

IMPLEMENT_DYNAMIC( CProgressCtrlEx, CProgressCtrl )

CProgressCtrlEx::CProgressCtrlEx()
	: m_colorText(GetSysColor(COLOR_BTNTEXT))
{
}

CProgressCtrlEx::~CProgressCtrlEx()
{
}

BEGIN_MESSAGE_MAP( CProgressCtrlEx, CProgressCtrl )
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CProgressCtrlEx::OnPaint()
{
	CProgressCtrl::OnPaint();

	CClientDC	dc( this );
	CRect		rc;
	CFont*		pOldFont ;

	// Устанавливаем НОРМАЛЬНЫЙ шрифт
	pOldFont = dc.SelectObject( GetParent()->GetFont() );
	GetClientRect( &rc );

	// Рисуем текст
	dc.SetBkMode( TRANSPARENT );

	CString CtrlText;
	CProgressCtrl::GetWindowTextW(CtrlText);
	if (!CtrlText.IsEmpty())
	{
		dc.SetTextColor(m_colorText);
		dc.DrawText(CtrlText, rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}

	// Восстанавливаем предыдущий шрифт
	dc.SelectObject( pOldFont );
}

void CProgressCtrlEx::SetShowText( UINT nID )
{
	CString csRes;

	if (csRes.LoadString(nID))
		CProgressCtrlEx::SetShowText(csRes);
}

void CProgressCtrlEx::SetShowText(CString c_str)
{
	CProgressCtrl::SetWindowTextW(c_str);
}

CString CProgressCtrlEx::GetShowText()
{
	CString csRes;
	CProgressCtrl::GetWindowTextW(csRes);
	return csRes;
}

void CProgressCtrlEx::SetZeroRange( int range )
{
	SetRange( 0, range );
	SetPos( 0 );

	CWnd* pParent = GetParent();

	// Таскбар Win7
	pTaskbarList = afxGlobalData.GetITaskbarList3();
	if (NULL == pTaskbarList) return;
	pTaskbarList->SetProgressValue( pParent->GetSafeHwnd(), 0, range );
	pTaskbarList->SetProgressState( pParent->GetSafeHwnd(), TBPF_NOPROGRESS );
}

void CProgressCtrlEx::SetPosition( int pos )
{
	SetPos( pos );

	CWnd* pParent = GetParent();

	// Таскбар Win7
	pTaskbarList = afxGlobalData.GetITaskbarList3();
	if (NULL == pTaskbarList) return;
	int _min, _max; GetRange( _min, _max );
	pTaskbarList->SetProgressValue( pParent->GetSafeHwnd(), pos, _max );
	pTaskbarList->SetProgressState( pParent->GetSafeHwnd(), pos ? TBPF_NORMAL : TBPF_NOPROGRESS );
}

void CProgressCtrlEx::Indeterminate( BOOL bInf )
{
	CWnd* pParent = GetParent();

	// Таскбар Win7
	pTaskbarList = afxGlobalData.GetITaskbarList3();
	if (NULL == pTaskbarList) return;
	pTaskbarList->SetProgressState( pParent->GetSafeHwnd(), bInf ? TBPF_INDETERMINATE : TBPF_NOPROGRESS );
}

void CProgressCtrlEx::Pause()
{
	CWnd* pParent = GetParent();

	// Таскбар Win7
	pTaskbarList = afxGlobalData.GetITaskbarList3();
	if (NULL == pTaskbarList) return;
	pTaskbarList->SetProgressState( pParent->GetSafeHwnd(), TBPF_PAUSED );
}

void CProgressCtrlEx::Error()
{
	CWnd* pParent = GetParent();

	// Таскбар Win7
	pTaskbarList = afxGlobalData.GetITaskbarList3();
	if (NULL == pTaskbarList) return;
	pTaskbarList->SetProgressState( pParent->GetSafeHwnd(), TBPF_ERROR );
}

void CProgressCtrlEx::SetTextColor(const COLORREF color)
{
	m_colorText = color;
}
