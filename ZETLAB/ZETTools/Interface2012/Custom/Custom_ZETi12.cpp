//------------------------------------------------------------------------------
// Custom_ZETi12.cpp : файл реализации
//
//------------------------------------------------------------------------------
//подключение стандартных заголовков
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Custom\Custom_ZETi12.h"
#include "C:\ZETTools\Interface2012\Custom\CustomItem.h"                 
//------------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//------------------------------------------------------------------------------
// конструктор диалогового окна CDialog_ZETi12
CCustom_ZETi12::CCustom_ZETi12(
	UINT nIDTemplate,			// идентификатор программы
	CWnd* pParent)				// указатель на владельца
	: CDialogEx(nIDTemplate, pParent)
{
	m_CurNumberTabFocus = 0;
	m_pCustomMain = NULL;
	m_pOwnerMouse = NULL;
}
//------------------------------------------------------------------------------
CCustom_ZETi12::~CCustom_ZETi12()
{
}
//------------------------------------------------------------------------------
void CCustom_ZETi12::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CCustom_ZETi12, CDialogEx)
	ON_COMMAND_RANGE(WM_USER+0x5000, WM_USER+0x7FFF, OnNewMenuItems)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_MESSAGE(IDM_PICK_ITEM, OnNewMenuItems2) //zettest
END_MESSAGE_MAP()
//------------------------------------------------------------------------------
BOOL CCustom_ZETi12::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// Удаление у окна заголовка. И добавление системных кнопок,
	// которые будут использоваться
	long NewLong = GetWindowLongW(m_hWnd, GWL_STYLE)
		| WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	if (NewLong & WS_CAPTION)
		NewLong ^= WS_CAPTION;
	SetWindowLongW(m_hWnd, GWL_STYLE, NewLong);

	return TRUE;
}
//------------------------------------------------------------------------------
void CCustom_ZETi12::OnDestroy()
{
	m_VectorFocus.clear();
	m_VectorMenu.clear();

	CDialogEx::OnDestroy();
}
//------------------------------------------------------------------------------
void CCustom_ZETi12::OnPaint()
{
	if (m_pCustomMain)
    {
		m_pCustomMain->OnDraw();
    }
}
//------------------------------------------------------------------------------
BOOL CCustom_ZETi12::OnEraseBkgnd(CDC* pDC)
{// Данная функция должна быть пустой
	return 1;
}
//------------------------------------------------------------------------------
void CCustom_ZETi12::RegistrationOwnerMouse(PVOID pClass)
{
	m_pOwnerMouse = (CCustomAll*)pClass;
}
//------------------------------------------------------------------------------
void CCustom_ZETi12::RegistrationFocus(PVOID pClass)
{
	CCustomAll *pElement = (CCustomAll*)pClass;
	long newVal = pElement->m_IndexTabFocus;
	if (m_CurNumberTabFocus != newVal)
	{
		m_VectorFocus[m_CurNumberTabFocus]->KillFocus();
		m_CurNumberTabFocus = newVal;
		m_VectorFocus[m_CurNumberTabFocus]->GetFocus();
	}
}
//------------------------------------------------------------------------------
void CCustom_ZETi12::RegistrationMenu(PVOID pClass)
{// Регистрация элемента, использующего меню, с выдачей этому элементу своего
// начального ID
	int n = m_VectorMenu.size();
	((CCustomItem*)pClass)->SetStartItemID(WM_USER + 0x7F00 - n * 0x100);
	m_VectorMenu.push_back((CCustomAll*)pClass);
}
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
void CCustom_ZETi12::OnNewMenuItems(UINT nID)
{// Выбран n-ый Item в i-ом меню
	long i = ( WM_USER + 0x7FFF - nID ) / 0x100;
	long n = nID - (WM_USER + 0x7F00 - i * 0x100);
	if ( i < (long)m_VectorMenu.size() )	// проверка от дурака
		((CCustomItem*)m_VectorMenu[i])->OnPickItem(n);
}
#else
void CCustom_ZETi12::OnNewMenuItems(UINT nID)
{// Выбран n-ый Item в i-ом меню
	long i = ( WM_USER + 0x7FFF - nID ) / 0x100;
	//long n = nID - (WM_USER + 0x7F00 - i * 0x100);
	if ( i < (long)m_VectorMenu.size() )	// проверка от дурака
		((CCustomItem*)m_VectorMenu[i])->OnPickItem(nID);
}
#endif

//------------------------------------------------------------------------------
BOOL CCustom_ZETi12::PreTranslateMessage(MSG* pMsg)
{
	static bool bShiftDoun = false, bTabDoun = false;
	bool bTab, bTabShift;
	UINT nFlags;
	CPoint point;
	short delta;
	BOOL ret = TRUE;
	TRACKMOUSEEVENT csTME = {0};	// структура нужна
	csTME.cbSize = sizeof(csTME);	//	чтобы
	csTME.dwFlags = TME_LEAVE;		//	приходил
	csTME.hwndTrack = m_hWnd;		//	WM_MOUSELEAVE

	if (m_pCustomMain)
	{
		CCustomAll *pElement;		
		if ( m_VectorFocus.size() > 0 )
			pElement = m_VectorFocus[m_CurNumberTabFocus];
		else
			pElement = NULL;
		if ( (pMsg->message == WM_MOUSEMOVE) ||
			 (pMsg->message == WM_LBUTTONDOWN)   || (pMsg->message == WM_LBUTTONUP) ||
			 (pMsg->message == WM_LBUTTONDBLCLK) ||
			 (pMsg->message == WM_RBUTTONDOWN)   || (pMsg->message == WM_RBUTTONUP) ||
			 (pMsg->message == WM_MOUSEWHEEL) ||
			 (pMsg->message == WM_MOUSELEAVE) )
		{
			if (pMsg->message != WM_MOUSEWHEEL)
				nFlags = pMsg->wParam;
			point.x = GET_X_LPARAM(pMsg->lParam);
			point.y = GET_Y_LPARAM(pMsg->lParam);
			switch(pMsg->message)
			{
			case WM_MOUSEMOVE :
				m_pCustomMain->OnMouseMove(nFlags, point);
				_TrackMouseEvent( &csTME );	// чтобы приходил WM_MOUSELEAVE
				break;
			case WM_MOUSELEAVE :
				m_pCustomMain->OnMouseLeave();
				break;
			case WM_LBUTTONDOWN :
				
				if (m_pOwnerMouse)
					m_pOwnerMouse->OnLButtonDown(nFlags, point);
				break;
			case WM_LBUTTONUP :
				if (m_pOwnerMouse)
					m_pOwnerMouse->OnLButtonUp(nFlags, point);
				break;
			case WM_LBUTTONDBLCLK :
				if (m_pOwnerMouse)
					m_pOwnerMouse->OnLButtonDblClk(nFlags, point);
				break;
			case WM_RBUTTONDOWN :
				if (m_pOwnerMouse)
					m_pOwnerMouse->OnRButtonDown(nFlags, point);
				break;
			case WM_RBUTTONUP :
				if (m_pOwnerMouse)
					m_pOwnerMouse->OnRButtonUp(nFlags, point);
				break;
			case WM_MOUSEWHEEL :
				if (m_pOwnerMouse)
				{
					nFlags = GET_KEYSTATE_WPARAM(pMsg->wParam);
					delta  = GET_WHEEL_DELTA_WPARAM(pMsg->wParam);
					ScreenToClient(&point);
					m_pOwnerMouse->OnMouseWheel(nFlags, point, delta);
				}	//	break;
			}
			ret = FALSE;
		}

		else 
			if (pMsg->message == WM_SYSCOMMAND)
			{
				if (pMsg->wParam == SC_RESTORE) 
					m_pCustomMain->m_bNoMinimize = true;
				if (pMsg->wParam == SC_MINIMIZE)
					m_pCustomMain->m_bNoMinimize = false;
				if (pMsg->wParam == SC_SIZE)
					m_pCustomMain->OnSize();
			}

		else if (pMsg->message == WM_KEYDOWN)
		{
			if (pElement)
			{// если есть элемент с фокусом
				switch(pMsg->wParam)
				{
				case VK_TAB :		// Tab
					bTabDoun = true;
					ret = FALSE;	// именно здесь и далее
					break;
				case VK_SHIFT :		// Shift
					bShiftDoun = true;
					ret = FALSE;
					break;
				case VK_RETURN :	// Enter
				case VK_SPACE :		// пробел
					if (pElement)
						pElement->OnLButtonClk( 0, pElement->GetRect().CenterPoint() );
					ret = FALSE;
				}
			}
		}
		else if (pMsg->message == WM_KEYUP)
		{
			if (pElement)
			{// если есть элемент с фокусом
				switch(pMsg->wParam)
				{
				case VK_TAB :		// Tab
					if (bShiftDoun)	{
						bTab = false;	bTabShift = true;	}
					else	{
						bTab = true;	bTabShift = false;	}
					bTabDoun = false;
					ret = FALSE;
					break;
				case VK_SHIFT :		// Shift
					if (bTabDoun)	{
						bTab = false;	bTabShift = true;	}
					else	{
						bTab = false;	bTabShift = false;	}
					bShiftDoun = false;
					ret = FALSE;
					break;
				default :
					bTab = false;		bTabShift = false;
				}
				if (bTab || bTabShift)
				{
					m_VectorFocus[m_CurNumberTabFocus]->KillFocus();
					if (bTab)
					{
						m_CurNumberTabFocus++;
						if (m_VectorFocus.size() == m_CurNumberTabFocus)
							m_CurNumberTabFocus = 0;
						bTab = false;
					}
					else
					{
						m_CurNumberTabFocus--;
						if (m_CurNumberTabFocus == -1)
							m_CurNumberTabFocus = m_VectorFocus.size() - 1;
						bTabShift = false;
					}
					m_VectorFocus[m_CurNumberTabFocus]->GetFocus();
				}
			}
		}
		else if (pMsg->message == WM_CHAR)
		{// Клавиша на клаве
			if (pElement)	// элемент с фокусом
				pElement->OnChar(pMsg->wParam, pMsg->lParam);
			ret = TRUE;
		}
		else if (pMsg->message == WM_CLOSE)
		{
			TRACE (L"OnClose - ACHTUNG!");
		}
		//else if (pMsg->message == WM_TIMER)
		//{
		//	int nnn = 0;
		//}
	}
	if (ret == TRUE)
		ret = CDialogEx::PreTranslateMessage(pMsg);
	else
		ret = TRUE;
	return ret;
}
//------------------------------------------------------------------------------
void CCustom_ZETi12::AddTabFocus(PVOID pClass)
{
	static int CurNumberTabFocus = 0;
	CCustomAll *pElement = (CCustomAll*)pClass;
	m_VectorFocus.push_back(pElement);
	pElement->m_IndexTabFocus = CurNumberTabFocus++;
	if (CurNumberTabFocus == 1)
		pElement->GetFocus();
}

LRESULT CCustom_ZETi12::OnNewMenuItems2( WPARAM wparam, LPARAM lparam )
{
	UINT_PTR nID;
	nID = (UINT_PTR) wparam;
	long i = ( WM_USER + 0x7FFF - nID ) / 0x100;
	long n = nID - (WM_USER + 0x7F00 - i * 0x100);
	if ( i < (long)m_VectorMenu.size() )	// проверка от дурака
		((CCustomItem*)m_VectorMenu[i])->OnPickItem(n);
	return 0;
}

//------------------------------------------------------------------------------