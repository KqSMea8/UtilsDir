#include "stdafx.h"
#include "TimeGridBasic.h"
#include "TimeGridDrow.h"
#include <windowsx.h>
#include <Resource.h>
#include <Grafic1/GraficTools/GridLayoutX/GridLayoutX.h>

BEGIN_MESSAGE_MAP(CTimeGridBasic, COleControl)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_ACTIVATE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_ENABLE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_MESSAGE(WM_USER_DINAMICSCROLL, OnDynamicScroll)
END_MESSAGE_MAP()

CTimeGridBasic::CTimeGridBasic()
	: MouseWheelPosY(0)
	, PrevMousePos(0)
	, CurMousePos(0)
	, LBDownMousePos(0)
	, m_focus(0)
	, cur_move(NULL)
	, cur_up(NULL)
	, cur_down(NULL)
	, cur_vrsdv(NULL)
	, cur_vrrsd(NULL)
	, cur_left(NULL)
	, cur_right(NULL)
	, cur_hrsdv(NULL)
	, cur_hrrsd(NULL)
	, cur_norma(NULL)
	, cur_hand(NULL)
	, cur_arrow(NULL)
	, m_bEnable(true)
	, mwl(false)
	, statusThread(NULL)
	, m_DrowDistinguish(false)
{
	copy_grid = mygr;
	wcscpy_s(mygr.font_name, _T("MS Shell Dlg"));
}

CTimeGridBasic::~CTimeGridBasic()
{
}

int CTimeGridBasic::MouseCursor(CPoint point)
{
	int zona;
	zona = -1;
	SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_arrow);
	if (mygr.bVerticalGraphs)
	{
		if (point.y <= mygr.left_x)
		{
			// сверху
			if (point.x <= mygr.bottom_y)
			{
				// верхний левый угол - автомасштаб
				//SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_arrow);
				zona = 9;
			}
			else if (point.x > mygr.bottom_y + mygr.height_y)
			{
				// правый верхний угол - ничего не делаем
				//SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_arrow);
				zona = 10;
			}
			else
			{
				zona = 11;				
			}
		}
		else if (point.x <= mygr.bottom_y)
		{
			// слева
			if (point.y <= mygr.left_x + mygr.width_x)
			{
				// рабочая область слева
				switch (((point.y - mygr.left_x) * 4) / (mygr.width_x))
				{
				case 0:
					SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_up);
					zona = 5;
					break;
				case 1:
					SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_vrsdv);
					zona = 6;
					break;
				case 2:
					SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_vrrsd);
					zona = 7;
					break;
				case 3:
					SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_down);
					zona = 8;
					break;
				}
			}
			else
			{
				// нижняя левая часть графика
				SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_norma);
				zona = 9;
			}
		}
		else if (point.x > mygr.bottom_y + mygr.height_y)
		{
			// справа
			//SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_arrow);
			zona = 10;
		}
		else if (point.y > mygr.left_x + mygr.width_x)
		{
			// снизу
			// рабочая область


			// по умолчанию ставим курсор стрелочкой
			SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_arrow);

			if (mygr.Display_Mode == DisplayMode::Default)
			{
				switch (((point.x - mygr.bottom_y) * 4) / (mygr.height_y))
				{
				case 0:
					SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_left);
					zona = 4;
					break;
				case 1:
					SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_hrsdv);
					zona = 3;
					break;
				case 2:
					SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_hrrsd);
					zona = 2;
					break;
				case 3:
					SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_right);
					zona = 1;
					break;
				}
			}
			else if ((mygr.Display_Mode == DisplayMode::SeismicGrath) || 
					 (mygr.Display_Mode == DisplayMode::ImpositionGraths))
			{
				switch (((point.x - mygr.bottom_y) * 4) / (mygr.height_y))
				{
				case 0:
				case 1:
					SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_hrrsd);
					zona = 2;
					break;
				case 2:
				case 3:
					SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_hrsdv);
					zona = 3;
					break;
				}
			}
		}
		else
		{
			zona = 0;
		}
	}
	else
	{
		if ((point.x <= mygr.left_x) && (point.y >= mygr.top_y))
		{
			// left

			if (((point.y - mygr.top_y) * 4) / mygr.height_y == 4)
			{
				SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_norma);
				zona = 9;
			}
			else
			{
				// по умолчанию ставим курсор стрелочкой
				SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_arrow);

				if (mygr.Display_Mode == DisplayMode::Default)
				{
					switch (((point.y - mygr.top_y) * 4) / (mygr.height_y))
					{
					case 0:
						SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_up);
						zona = 1;
						break;
					case 1:
						SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_vrrsd);
						zona = 2;
						break;
					case 2:
						SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_vrsdv);
						zona = 3;
						break;
					case 3:
						SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_down);
						zona = 4;
						break;
					}
				}
				else if ((mygr.Display_Mode == DisplayMode::SeismicGrath) ||
						 (mygr.Display_Mode == DisplayMode::ImpositionGraths))
				{
					switch (((point.y - mygr.top_y) * 4) / (mygr.height_y))
					{
					case 0:
					case 1:
						SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_vrrsd);
						zona = 2;
						break;
					case 2:
					case 3:
						SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_vrsdv);
						zona = 3;
						break;
					}
				}
			}
		}
		else
		{
			if (point.y >= mygr.top_y + mygr.height_y && point.x < mygr.left_x + mygr.width_x - 1.01 && mygr.width_x > 0)
			{
				// внизу
				switch (((point.x - mygr.left_x) * 4) / (mygr.width_x))
				{
				case 0:
					SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_left);
					zona = 5;
					break;
				case 1:
					SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_hrsdv);
					zona = 6;
					break;
				case 2:
					SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_hrrsd);
					zona = 7;
					break;
				case 3:
					SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_right);
					zona = 8;
					break;
				}
			}
			else
			{
				if (point.y >= mygr.top_y && point.x <= mygr.left_x + mygr.width_x - 1.01)
				{
					// 				if (LeftBPressed)
					// 				{
					// 					SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_move);					
					// 					TRACE("TRUE");
					// 				}
					// 				else
					// 				{
					//					SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_arrow);
					// 					TRACE("FASLE");
					// 				}
					zona = 0;
				}
				else if (point.y <= mygr.top_y && point.x <= mygr.left_x + mygr.width_x + mygr.right_x - 1.01)
				{
					// верхняя часть графика
					SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_arrow);
					zona = 10;
					if (point.x > mygr.left_x + mygr.width_x - 1.01)
					{
						// правый верхний угол
						double x1 = point.x - mygr.left_x - mygr.width_x + 1.01;
						if (x1 + point.y > mygr.top_y)
							zona = 11;
					}

				}
				else if (point.x >= mygr.left_x + mygr.width_x - 1.01)
				{
					// справа
					SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_arrow);
					zona = 11;
				}
				SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)cur_arrow);
			}
		}
	}
	return zona;	
}

void CheckAvailabilityOfTheExtension(TrendGrid *grd, double Delta, double Increament)
{
	CZetTime last = grd->math_min_x + Increament + grd->math_width_x * Delta;		// значение после приближения
	CZetTime first = grd->math_min_x + Increament;									// значение после приближения
	CZetTimeSpan dt = last - first;
	double kolnanoseconds = dt;
	kolnanoseconds *= PRECISION_NANSEC;												// 3 знака после запятой
	int numvert = grd->width_x / WIDTH_COLUMN_IN_PIX;
	double shvert = round((double)kolnanoseconds / numvert);
	
	if (shvert > 0)																	// если не уперлись в минимальное приближение
	{
		grd->math_width_x = grd->math_width_x * Delta;
		grd->math_min_x += Increament;
	}
}

void CTimeGridBasic::ChangeXScale(short zDelta)
{	// расширяем и сужаем к позиции курсора
	// процент экрана на котором стоит мышка
	double percent = (mygr.mouse_x - mygr.left_x - 1) / (double)mygr.width_x;		// положение мыши на экране
	if (zDelta < 0)
	{	// сужение
		mygr.math_width_x = mygr.math_width_x / 0.98;		
		mygr.math_min_x -=  mygr.math_width_x * percent * (1.0 - 0.98);
	}
	else
	{	// расширение
		double increament = mygr.math_width_x * percent * (1.0 - 0.98);			// инкримент math_min_x
		CheckAvailabilityOfTheExtension(&mygr, 0.98, increament);
	}

	CheckScaleX();
}

int CTimeGridBasic::ForCreate()
{
	SetNewFont();

	HINSTANCE hInstance;
	hInstance = AfxGetInstanceHandle();
	cur_move  = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CUR_MOVE));
	cur_up    = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CUR_UP));
	cur_down  = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CUR_DOWN));
	cur_vrsdv = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CUR_V_NARROW));
	cur_vrrsd = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CUR_V_WIDE));
	cur_left  = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CUR_LEFT));
	cur_right = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CUR_RIGHT));
	cur_hrrsd = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CUR_H_WIDE));
	cur_hrsdv = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CUR_H_NARROW));
	cur_norma = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CUR_NORMA));
	cur_hand  = LoadCursor(NULL, IDC_HAND);
	cur_arrow = LoadCursor(NULL, IDC_ARROW);

	HMODULE ntlib;
	ntlib = LoadLibrary(_T("ntdll.dll"));

	if (!ntlib)
	{
		MessageBox(_T("Can't load ntdll.dll"));
		return -1;
	}

	FARPROC proc = GetProcAddress(ntlib, ("NtQuerySystemInformation"));

	if (!proc)
	{
		MessageBox(_T("Can't find NtQuerySystemInformation procedure"));
		FreeLibrary(ntlib);
		return -1;
	}

	return 0;
}

void CTimeGridBasic::OnSize(UINT nType, int cx, int cy)
{
	_critSect.Lock();
	if (cx < mygr.left_x + 80)
	{
		cx = mygr.left_x + 80;
		mygr.width_x = 80;
	}
	if (cy < mygr.bottom_y + mygr.top_y + 80)
	{
		cy = mygr.bottom_y + mygr.top_y + 80;
		mygr.height_y = 80;
	}
	mygr.update = UPDATE_DRAW_ALL;
	InvalidateControl();
	_critSect.Unlock();

	COleControl::OnSize(nType, cx, cy);
}

void CTimeGridBasic::OnSizing(UINT fwSide, LPRECT pRect)
{
	COleControl::OnSizing(fwSide, pRect);

	mygr.update |= UPDATE_DRAW_ALL;
}

void CTimeGridBasic::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	COleControl::OnActivate(nState, pWndOther, bMinimized);
	m_focus = 1;
}

void CTimeGridBasic::OnSetFocus(CWnd* pOldWnd)
{
	COleControl::OnSetFocus(pOldWnd);
	m_focus = 1;
}

void CTimeGridBasic::OnKillFocus(CWnd* pNewWnd)
{
	COleControl::OnKillFocus(pNewWnd);
	m_focus = 1;
}

void CTimeGridBasic::OnEnable(BOOL bEnable)
{
	COleControl::OnEnable(bEnable);

	m_bEnable = (bEnable != FALSE);
	InvalidateControl();
}

void GetParents(CWnd* pWnd, CWnd** ptr)
{
	const int numParents = 3;

	for (int i = 0; i < numParents; ++i)
		ptr[i] = NULL;
	if (NULL == pWnd)
		return;
	ptr[0] = pWnd->GetTopLevelFrame();
	ptr[1] = pWnd->GetTopLevelOwner();
	ptr[2] = pWnd->GetTopLevelParent();
}

void CTimeGridBasic::OnMouseMove(UINT nFlags, CPoint point)
{
	const int numParents = 3;

	if (GetFocus() != this)// && IsTopParentActive()) //если не в фокусе GetActiveWindow 
	{
		CWnd* parents[numParents] = { NULL };
		CWnd* active[3][numParents] = { NULL };
		GetParents(this, parents);
		GetParents(GetActiveWindow(), active[0]);
		GetParents(GetFocus(), active[1]);
		GetParents(GetForegroundWindow(), active[2]);
		bool bCommon = false;
		int i = 0, j = 0;
		for (j = 0; j < 3 && !bCommon; ++j)
		{
			for (i = 0; i < numParents && !bCommon; ++i)
				for (int k = 0; k < numParents && !bCommon; ++k)
					if (parents[i] && parents[i] == active[j][k])
						bCommon = true;
		}
		if (bCommon)
		{
			SetFocus(); //захватить фокус. Чтобы колесо отрабатывало на этот грид
		}
	}
	int zona;
	_critSect.Lock();
	zona = MouseCursor(point);

	double CurY, PrevY;
	double CurDate, PrevDate;
	long left_x, right_x;
	long top_y, bottom_y;
	
	if ((zona == 0) && (nFlags == MK_LBUTTON) && !VertCursMoves && !GorCursMoves)
	{
		// отслеживание перемещения курсора при зажатой левой мыши
		if (PrevMousePos != point)
		{
			if (mygr.Display_Mode == DisplayMode::Default)
			{
				if (mygr.bVerticalGraphs)
				{
					CurY = GetYFromCoord(&mygr, point.x);
					PrevY = GetYFromCoord(&mygr, PrevMousePos.x);
				}
				else
				{
					CurY = GetYFromCoord(&mygr, point.y);
					PrevY = GetYFromCoord(&mygr, PrevMousePos.y);
				}

				// перемещаем на получившуюся разницу
				mygr.math_min_y += PrevY - CurY;
				CheckScaleY(&mygr);
			}

			if (mygr.bVerticalGraphs)
			{
				CurDate = GetXFromCoord(&mygr, point.y);						// дата соответствущая текущему положению курсора
				PrevDate = GetXFromCoord(&mygr, PrevMousePos.y);				// дата соответствущая предыдущему положению курсора
			}
			else
			{
				CurDate = GetXFromCoord(&mygr, point.x);						// дата соответствущая текущему положению курсора
				PrevDate = GetXFromCoord(&mygr, PrevMousePos.x);				// дата соответствущая предыдущему положению курсора
			}

			// перемещаем на получившуюся разницу
			mygr.math_min_x += PrevDate - CurDate;
			CheckScaleX();
			
			PrevMousePos = point;
			InvalidateControl();
		}
	}

	// если зажат и контрол и левая кнопка мыши
	if ((zona == 0) && ((nFlags & (MK_LBUTTON | MK_CONTROL)) == (MK_LBUTTON | MK_CONTROL)))
	{
		// рамка для масштабирования
		m_DrowDistinguish = true;
		CurMousePos = point;
		InvalidateControl();
	}
	else
	{
		if (m_DrowDistinguish)
		{
			// масштабирование при отмене рамки
			m_DrowDistinguish = false;

			if ((PrevMousePos.x > CurMousePos.x) && (PrevMousePos.y > CurMousePos.y) && (!PrevGraphPos.IsEmpty()))
			{
				// отмена предыдуего масштабирования рамкой
				mygr.math_min_x = PrevGraphPos.math_min_x;
				mygr.math_width_x = PrevGraphPos.math_width_x;
				mygr.math_min_y = PrevGraphPos.math_min_y;
				mygr.math_width_y = PrevGraphPos.math_width_y;
			}
			else
			{
				if (PrevMousePos.x < CurMousePos.x)
				{
					left_x = PrevMousePos.x;
					right_x = CurMousePos.x;
				}
				else
				{
					left_x = CurMousePos.x;
					right_x = PrevMousePos.x;
				}

				if (PrevMousePos.y < CurMousePos.y)
				{
					top_y = PrevMousePos.y;
					bottom_y = CurMousePos.y;
				}
				else
				{
					top_y = CurMousePos.y;
					bottom_y = PrevMousePos.y;
				}

				if (mygr.bVerticalGraphs)
				{
					PrevDate = GetXFromCoord(&mygr, top_y);					// дата соответствущая предыдущему положению курсора
					CurDate = GetXFromCoord(&mygr, bottom_y);				// дата соответствущая текущему положению курсора				
					CurY = GetYFromCoord(&mygr, right_x);
					PrevY = GetYFromCoord(&mygr, left_x);
				}
				else
				{
					PrevDate = GetXFromCoord(&mygr, left_x);				// дата соответствущая предыдущему положению курсора
					CurDate = GetXFromCoord(&mygr, right_x);				// дата соответствущая текущему положению курсора
					CurY = GetYFromCoord(&mygr, top_y);
					PrevY = GetYFromCoord(&mygr, bottom_y);
				}

				PrevGraphPos.math_min_x = mygr.math_min_x;
				PrevGraphPos.math_width_x = mygr.math_width_x;
				PrevGraphPos.math_min_y = mygr.math_min_y;
				PrevGraphPos.math_width_y = mygr.math_width_y;
				
				mygr.math_min_y = PrevY;
				mygr.math_width_y = CurY - PrevY;

				mygr.math_min_x = PrevDate;
				mygr.math_width_x = CurDate - PrevDate;
			}
			CheckScaleX();
			CheckScaleY(&mygr);
			InvalidateControl();
		}
	}
	CPoint CurrentPoint(point);
	if (mygr.bVerticalGraphs)
	{
		CurrentPoint.x = point.y;
		CurrentPoint.y = point.x;
	}

	if (VertCursMoves)
		MoveCursor(CurrentPoint);

	if (GorCursMoves)
	{
		// защита от увода курсора за пределы графика
		if (CurrentPoint.y <= mygr.top_y)
			m_CoordGorCurs[CurSelectedCursY].CurrentPos = mygr.math_min_y + mygr.math_width_y;
		else if (CurrentPoint.y >= mygr.top_y + mygr.height_y)
			m_CoordGorCurs[CurSelectedCursY].CurrentPos = mygr.math_min_y;
		else
			m_CoordGorCurs[CurSelectedCursY].CurrentPos = GetYFromCoord(&mygr, CurrentPoint.y);

		if (CurSelectedCursY <= 1)	// если перемещаются первые 2 курсора
			SendCursDistance();
		CursMoving(false, CurSelectedCursY);
		InvalidateControl();
	}

	if ((zona == 6) || (zona == 7) || (zona == 0))
	{
		if (mygr.bVerticalGraphs)
		{
			mygr.mouse_y = point.x;
			mygr.mouse_x = point.y;
		}
		else
		{
			mygr.mouse_y = point.y;
			mygr.mouse_x = point.x;
		}
	}
	if ((nFlags & MK_LBUTTON) == MK_LBUTTON)
	{
		if (zona == 0)
		{
			if (mygr.timer != 0)
			{
				if (mygr.bVerticalGraphs)
				{
					mygr.mouse_y = point.x;
					mygr.mouse_x = point.y;
				}
				else
				{
					mygr.mouse_y = point.y;
					mygr.mouse_x = point.x;
				}
			}
		}
		else
		{
			KillTimer(1);
		}
	}
	_critSect.Unlock();

	COleControl::OnMouseMove(nFlags, point);
}

void CTimeGridBasic::CheckScaleX()
{
	CTimeGridDrow::CheckScaleX(&mygr);
}

void CTimeGridBasic::MoveCursor(CPoint point)
{
	double CurDate, PrevDate;

	CurDate = GetXFromCoord(&mygr, point.x);						// дата соответствущая текущему положению курсора
	PrevDate = GetXFromCoord(&mygr, PrevMousePosCurs.x);			// дата соответствущая предыдущему положению курсора
// 	CZetTimeSpan dtX = CurDate - PrevDate;				// на сколько переместить
// 	CZetTime DisplacementX = PrevDate;
// 	DisplacementX += dtX;
//	DisplacementX = CurDate;

	if (point.x <= mygr.left_x)
		m_CoordVertCurs[CurSelectedCursX].CurrentPos = mygr.math_min_x;
	else if (point.x >= mygr.left_x + mygr.width_x)
		m_CoordVertCurs[CurSelectedCursX].CurrentPos = mygr.math_min_x + mygr.math_width_x;
	else
		m_CoordVertCurs[CurSelectedCursX].CurrentPos = CurDate;


// 	CZetTime last = mygr.math_min_x + mygr.math_width_x;
// 	CZetTime first = mygr.math_min_x;
	double prevcolumn = mygr.math_min_x;
	double nextcolumn = mygr.math_min_x + mygr.math_width_x;
	// находим между каким колонками будет находиться курсор
	for (int i = 0; i < (int)CursorCoords.VectorLayout.size(); i++)
	{
		if ((m_CoordVertCurs[CurSelectedCursX].CurrentPos >= prevcolumn) && (m_CoordVertCurs[CurSelectedCursX].CurrentPos <= (double)CursorCoords.VectorLayout[i]))
		{
			// нашли между какими колонками будет курсор
			nextcolumn = CursorCoords.VectorLayout[i];
			break;
		}
		else if (i == CursorCoords.VectorLayout.size() - 1)
		{
			// после последней колонки
			prevcolumn = CursorCoords.VectorLayout[i];
			nextcolumn = mygr.math_min_x + mygr.math_width_x;
		}
		else
			prevcolumn = CursorCoords.VectorLayout[i];
	}

	if ((m_CoordVertCurs[CurSelectedCursX].CurrentPos - prevcolumn) < (nextcolumn - m_CoordVertCurs[CurSelectedCursX].CurrentPos))
		m_CoordVertCurs[CurSelectedCursX].CurrentPos = prevcolumn;
	else
		m_CoordVertCurs[CurSelectedCursX].CurrentPos = nextcolumn;

	if (CurSelectedCursY <= 1)	// если перемещаются первые 2 курсора
		SendCursDistance();
	CursMoving(true, CurSelectedCursX);
	InvalidateControl();
}

void CTimeGridBasic::OnTimer(UINT_PTR nIDEvent)
{
	_critSect.Lock();
	mygr.timer--;
	if (mygr.timer <= 0)
	{
		mygr.update |= UPDATE_CURSOR_MOVE;
		InvalidateControl();
		mygr.timer = -1;
	}
	_critSect.Unlock();
	COleControl::OnTimer(nIDEvent);
}

void CTimeGridBasic::OnLButtonDown(UINT nFlags, CPoint point)
{
	int zona;

	_critSect.Lock();

	if (m_focus == 0)
	{
		mygr.update |= UPDATE_CURSOR_NEW;
		m_focus = 1;
	}

	zona = MouseCursor(point);

	if (zona == 0)
	{
		// зажатие и перетаскивание графика
		PrevMousePos = point;
		LBDownMousePos = point;

		if (GetCursor() == cur_hand) // если кликнули тултип
		{
			AddOrRemoveToolTipToVisible(point);
		}
		else
		{
			if (mygr.bVerticalGraphs)
			{
				mygr.mouse_y = point.x;
				mygr.mouse_x = point.y;
			}
			else
			{
				mygr.mouse_y = point.y;
				mygr.mouse_x = point.x;
			}

			mygr.timer = 6;
			SetTimer(1, 50, NULL);
			mygr.update |= UPDATE_CURSOR_MOVE;
		}
	}

	CPoint CurrentPoint(point);


	if (zona == 10)
	{
		if (mygr.bVerticalGraphs)
		{
			CurrentPoint.x = point.y;
			CurrentPoint.y = point.x;
		}

		VertCursMoves = false;
		for (int i = 0; i < (int)m_CoordVertCurs.size(); i++)
		{
			if ((CurrentPoint.x >= GetPixX(&mygr, m_CoordVertCurs[i].CurrentPos) - CURSOR_SIZE) && (CurrentPoint.x <= GetPixX(&mygr, m_CoordVertCurs[i].CurrentPos) + CURSOR_SIZE))
			{
				if (VertCursMoves)
				{
					// если был уже найден курсор проверяем не подходит ли нам другой
					if (fabs((double)GetPixX(&mygr, m_CoordVertCurs[i].CurrentPos) - (double)CurrentPoint.x) <= fabs((double)GetPixX(&mygr, m_CoordVertCurs[CurSelectedCursX].CurrentPos) - (double)CurrentPoint.x))
						CurSelectedCursX = i;
					continue;
				}
				// если перетаскиваем курсор
				VertCursMoves = true;
				PrevMousePosCurs = CurrentPoint;
				CurSelectedCursX = i;
			}
			else if ((m_CoordVertCurs[i].CurrentPos < mygr.math_min_x) && (CurrentPoint.x >= GetPixX(&mygr, mygr.math_min_x) - CURSOR_SIZE) && (CurrentPoint.x <= GetPixX(&mygr, mygr.math_min_x) + CURSOR_SIZE))
			{
				CurSelectedCursX = i;
				// если схватили находящийся слева видимого диапозона курсор
				for (int j = i + 1; j < (int)m_CoordVertCurs.size(); j++)
				{
					// находим курсор который правее
					if ((m_CoordVertCurs[j].CurrentPos < mygr.math_min_x) && (CurrentPoint.x >= GetPixX(&mygr, mygr.math_min_x) - CURSOR_SIZE) && (CurrentPoint.x <= GetPixX(&mygr, mygr.math_min_x) + CURSOR_SIZE))
					{
						if (m_CoordVertCurs[j].CurrentPos >= m_CoordVertCurs[i].CurrentPos)
						{
							// находится правее
							CurSelectedCursX = j;
						}
					}
				}
				m_CoordVertCurs[CurSelectedCursX].CurrentPos = mygr.math_min_x;
				VertCursMoves = true;
				PrevMousePosCurs = CurrentPoint;
				break;
			}
			else if ((m_CoordVertCurs[i].CurrentPos > mygr.math_min_x + mygr.math_width_x) && (CurrentPoint.x >= GetPixX(&mygr, mygr.math_min_x + mygr.math_width_x) - CURSOR_SIZE)
					 && (CurrentPoint.x <= GetPixX(&mygr, mygr.math_min_x + mygr.math_width_x) + CURSOR_SIZE))
			{
				CurSelectedCursX = i;
				// если схватили находящийся справа видимого диапозона курсор
				for (int j = i + 1; j < (int)m_CoordVertCurs.size(); j++)
				{
					// находим курсор который правее
					if ((m_CoordVertCurs[j].CurrentPos > mygr.math_min_x + mygr.math_width_x) && (CurrentPoint.x >= GetPixX(&mygr, mygr.math_min_x + mygr.math_width_x) - CURSOR_SIZE)
						&& (CurrentPoint.x <= GetPixX(&mygr, mygr.math_min_x + mygr.math_width_x) + CURSOR_SIZE))
					{
						if (m_CoordVertCurs[j].CurrentPos <= m_CoordVertCurs[i].CurrentPos)
						{
							// находится левее
							CurSelectedCursX = j;
						}
					}
				}
				m_CoordVertCurs[CurSelectedCursX].CurrentPos = mygr.math_min_x + mygr.math_width_x;
				VertCursMoves = true;
				PrevMousePosCurs = CurrentPoint;
				break;
			}
		}
		if (VertCursMoves)
			InvalidateControl();
	}

	if (zona == 11)
	{
		GorCursMoves = false;

		if (mygr.bVerticalGraphs)
		{			
			CRect CtrlRect;
			GetClientRect(CtrlRect);
			CurrentPoint.y = CtrlRect.Width() - CurrentPoint.x;
		}

		for (int i = 0; i < (int)m_CoordGorCurs.size(); i++)
		{
			if ((CurrentPoint.y >= GetPixY(&mygr, m_CoordGorCurs[i].CurrentPos) - CURSOR_SIZE) && (CurrentPoint.y <= GetPixY(&mygr, m_CoordGorCurs[i].CurrentPos) + CURSOR_SIZE))
			{
				if (GorCursMoves)
				{
					// если был уже найден курсор проверяем не подходит ли нам другой
					if (fabs((double)GetPixY(&mygr, m_CoordGorCurs[i].CurrentPos) - (double)CurrentPoint.y) <= fabs((double)GetPixY(&mygr, m_CoordGorCurs[CurSelectedCursY].CurrentPos) - (double)CurrentPoint.y))
						CurSelectedCursY = i;
					continue;
				}
				// если перетаскиваем курсор
				GorCursMoves = true;
				PrevMousePosCurs = CurrentPoint;
				CurSelectedCursY = i;
			}
			else if ((m_CoordGorCurs[i].CurrentPos < mygr.math_min_y) && (CurrentPoint.y >= GetPixY(&mygr, mygr.math_min_y) - CURSOR_SIZE) && (CurrentPoint.y <= GetPixY(&mygr, mygr.math_min_y) + CURSOR_SIZE))
			{
				CurSelectedCursY = i;
				// если схватили находящийся слева видимого диапозона курсор
				for (int j = i + 1; j < (int)m_CoordGorCurs.size(); j++)
				{
					// находим курсор который правее
					if ((m_CoordGorCurs[j].CurrentPos < mygr.math_min_y) && (CurrentPoint.y >= GetPixY(&mygr, mygr.math_min_y) - CURSOR_SIZE) && (CurrentPoint.y <= GetPixY(&mygr, mygr.math_min_y) + CURSOR_SIZE))
					{
						if (m_CoordGorCurs[j].CurrentPos >= m_CoordGorCurs[i].CurrentPos)
						{
							// находится правее
							CurSelectedCursY = j;
						}
					}
				}
				m_CoordGorCurs[CurSelectedCursY].CurrentPos = mygr.math_min_y;
				GorCursMoves = true;
				PrevMousePosCurs = CurrentPoint;
				break;
			}
			else if ((m_CoordGorCurs[i].CurrentPos > mygr.math_min_y + mygr.math_width_y) && (CurrentPoint.y >= GetPixY(&mygr, mygr.math_min_y + mygr.math_width_y) - CURSOR_SIZE)
					 && (CurrentPoint.y <= GetPixY(&mygr, mygr.math_min_y + mygr.math_width_y) + CURSOR_SIZE))
			{
				CurSelectedCursY = i;
				// если схватили находящийся справа видимого диапозона курсор
				for (int j = i + 1; j < (int)m_CoordGorCurs.size(); j++)
				{
					// находим курсор который правее
					if ((m_CoordGorCurs[j].CurrentPos > mygr.math_min_y + mygr.math_width_y) && (CurrentPoint.y >= GetPixY(&mygr, mygr.math_min_y + mygr.math_width_y) - CURSOR_SIZE)
						&& (CurrentPoint.y <= GetPixY(&mygr, mygr.math_min_y + mygr.math_width_y) + CURSOR_SIZE))
					{
						if (m_CoordGorCurs[j].CurrentPos <= m_CoordGorCurs[i].CurrentPos)
						{
							// находится левее
							CurSelectedCursY = j;
						}
					}
				}
				m_CoordGorCurs[CurSelectedCursY].CurrentPos = mygr.math_min_y + mygr.math_width_y;
				GorCursMoves = true;
				PrevMousePosCurs = CurrentPoint;
				break;
			}
		}
		if (GorCursMoves)
			InvalidateControl();
	}

	_critSect.Unlock();
	//InvalidateControl();

	COleControl::OnLButtonDown(nFlags, point);
}

void CTimeGridBasic::OnLButtonUp(UINT nFlags, CPoint point)
{
	long zona;
	int flu = 0;
	double dt(0);

	VertCursMoves = false;
	GorCursMoves = false;

	_critSect.Lock();

	zona = MouseCursor(point);

	if (mygr.timer != 0)
	{
		KillTimer(1);
		mygr.timer = 0;
	}
	switch (zona)
	{
		case 0:
		{
			if (LBDownMousePos == point)
				ChoosePoint(point);				
			break;
		}
		case 1:
			if ((mygr.Display_Mode == DisplayMode::Default) || 
				(mygr.Display_Mode == DisplayMode::SeismicGrath) ||
				(mygr.Display_Mode == DisplayMode::ImpositionGraths))
			{
				mygr.math_min_y = mygr.math_min_y + mygr.math_width_y / 6.;

				CheckScaleY(&mygr);
				flu = 1;
			}
			break;
		case 2:
			switch (mygr.Display_Mode)
			{
				case DisplayMode::Default:
				{
					if (mygr.make_upor == 0)
					{
						mygr.math_min_y = mygr.math_min_y + mygr.math_width_y*0.1;
						mygr.math_width_y = mygr.math_width_y*0.8;
					}
					if (mygr.make_upor != 0)
					{
						if (stop == FALSE)
						{
							mygr.math_min_y = mygr.math_min_y + mygr.math_width_y*0.1;
							mygr.math_width_y = mygr.math_width_y*0.8;
						}
					}

					CheckScaleY(&mygr);
					flu = 1;
					break;
				}
				case DisplayMode::SeismicGrath:
				case DisplayMode::ImpositionGraths:
				{
					float PrevVal;
					float NextVal;
					CalculateStunnerValues((float)mygr.math_width_y, &PrevVal, &NextVal);

					mygr.math_min_y += (mygr.math_width_y - PrevVal) / 2 * 50;
					mygr.math_width_y = PrevVal;

					CheckScaleY(&mygr);
					flu = 1;
					break;
				}
			}
			break;
		case 3:
			switch (mygr.Display_Mode)
			{
				case DisplayMode::Default:
				{
					mygr.math_width_y = mygr.math_width_y / 0.8;
					mygr.math_min_y = mygr.math_min_y - mygr.math_width_y*0.1;
					CheckScaleY(&mygr);
					flu = 1;
					break;
				}
				case DisplayMode::SeismicGrath:
				case DisplayMode::ImpositionGraths:
				{
					float PrevVal;
					float NextVal;
					CalculateStunnerValues((float)mygr.math_width_y, &PrevVal, &NextVal);

					mygr.math_min_y -= (NextVal - mygr.math_width_y) / 2 * 50;
					mygr.math_width_y = NextVal;

					flu = 1;
					break;
				}
			}
			break;
		case 4:
			if ((mygr.Display_Mode == DisplayMode::Default) || 
				(mygr.Display_Mode == DisplayMode::SeismicGrath) ||
				(mygr.Display_Mode == DisplayMode::ImpositionGraths))
			{
				mygr.math_min_y = mygr.math_min_y - mygr.math_width_y / 6.;
				CheckScaleY(&mygr);
				flu = 1;
			}
			break;
		case 5://влево
			mygr.math_min_x = mygr.math_min_x - mygr.math_width_x / 6.;
			CheckScaleX();
			flu = 1;
			break;
		case 6://сужение																
			mygr.math_width_x = mygr.math_width_x / 0.8;
			mygr.math_min_x -= mygr.math_width_x / (float)mygr.width_x * (mygr.mouse_x - mygr.left_x) * (1.0 - 0.8);
			CheckScaleX();
			flu = 1;
			break;
		case 7://расширение		
		{
			double increament  = mygr.math_width_x / (float)mygr.width_x*(mygr.mouse_x - mygr.left_x) * (1.0 - 0.8);	// инкримент math_min_x
			CheckAvailabilityOfTheExtension(&mygr, 0.8, increament);
			
			CheckScaleX();
			flu = 1;
			break;
		}
		case 8://вправо
			mygr.math_min_x = mygr.math_min_x + mygr.math_width_x / 6.;
			CheckScaleX();
			flu = 1;
			break;
		case 9:
			if ((nFlags & MK_CONTROL) == MK_CONTROL)
				NeedLoadDataForAutoScale();
			else
			{
				if (((nFlags & MK_CONTROL) != MK_CONTROL) && ((nFlags & MK_SHIFT) != MK_SHIFT))
					AutoScaleVisibleData(&mygr);

				switch (mygr.Display_Mode)
				{
					case DisplayMode::Events:
					case DisplayMode::Default:
					case DisplayMode::SplittingFields:
						if (((nFlags & MK_SHIFT) == MK_SHIFT) && (mygr.Display_Mode == DisplayMode::Default))
							AutoScaleYData(&mygr);

						if (((m_VisibleLeftXCoord == NUMBER_SEC_IN_3000) && (m_VisibleRightXCoord == 0)) ||
							((m_VisibleBottomYCoord == DBL_MAX) && (m_VisibleTopYCoord   == -DBL_MAX) &&
							(mygr.Display_Mode == DisplayMode::Default)))
							NeedLoadDataForAutoScale();
						
						break;
					case DisplayMode::SeismicGrath:
					case DisplayMode::ImpositionGraths:
					{
						if (((m_VisibleLeftXCoord == NUMBER_SEC_IN_3000) && (m_VisibleRightXCoord == 0)) ||
							((m_VisibleBottomYCoord == DBL_MAX) && (m_VisibleTopYCoord   == -DBL_MAX)))
							NeedLoadDataForAutoScale();
						else
							AutoScaleYData(&mygr);

						break;
					}
					default:
						break;
				}
			}			
			break;
	}

	if (zona != 0)
		mygr.update |= UPDATE_DRAW_ALL;
	_critSect.Unlock();

	InvalidateControl();
	COleControl::OnLButtonUp(nFlags, point);
}

short ThreadDelta;
LRESULT	CTimeGridBasic::OnDynamicScroll(WPARAM wParam, LPARAM lParam)
{
	double percent;
	if (mygr.bVerticalGraphs)
		percent = (MouseWheelPosY - mygr.bottom_y) / (float)mygr.height_y;		// положение мыши на экране
	else
		percent = 1.0 - (MouseWheelPosY - mygr.top_y) / (float)mygr.height_y;	// положение мыши на экране
	
	_critSect.Lock();
	switch ((int)lParam)
	{
		case 1:
		case 4:
			if ((mygr.Display_Mode == DisplayMode::Default) || 
				(mygr.Display_Mode == DisplayMode::SeismicGrath) ||
				(mygr.Display_Mode == DisplayMode::ImpositionGraths))
			{
				if (ThreadDelta > 0)
				{
					mygr.math_min_y = mygr.math_min_y + mygr.math_width_y*0.01;
				}
				else
				{
					mygr.math_min_y = mygr.math_min_y - mygr.math_width_y*0.01;
				}
				CheckScaleY(&mygr);
			}
			break;
		case 2:
		case 3:
			switch (mygr.Display_Mode)
			{
				case DisplayMode::Default:
					if (ThreadDelta > 0)
					{
						if (stop == FALSE)
						{
							mygr.math_min_y += mygr.math_width_y * percent * (1.0 - 0.98);
							mygr.math_width_y = mygr.math_width_y * 0.98;
						}
					}
					else
					{
						mygr.math_width_y = mygr.math_width_y / 0.98;
						mygr.math_min_y -= mygr.math_width_y * percent * (1.0 - 0.98);
					}
					CheckScaleY(&mygr);
					break;
				case DisplayMode::SeismicGrath:
				case DisplayMode::ImpositionGraths:
					float PrevVal;
					float NextVal;
					CalculateStunnerValues((float)mygr.math_width_y, &PrevVal, &NextVal);

					if (ThreadDelta > 0)
					{
						if (stop == FALSE)
						{
							mygr.math_min_y += (mygr.math_width_y - PrevVal) / 2 * percent;
							mygr.math_width_y = PrevVal;
						}
					}
					else
					{
						mygr.math_min_y -= (NextVal - mygr.math_width_y) / 2 * percent;
						mygr.math_width_y = NextVal;
					}
					CheckScaleY(&mygr);

					break;
				default:
					break;
			}
			break;
		case 5:
		case 8:
			if (ThreadDelta < 0)
			{
				mygr.math_min_x = mygr.math_min_x - mygr.math_width_x*0.01;
			}
			else
			{
				mygr.math_min_x = mygr.math_min_x + mygr.math_width_x*0.01;
			}
			CheckScaleX();
			break;
		case 0:
		case 6:
		case 7:
			//полоса, располагаемая в нижней части экрана
			//изменение масштаба по горизонтали
			ChangeXScale(ThreadDelta);
			break;
		case -1:
			switch (mygr.Display_Mode)
			{
				case DisplayMode::Default:
					if (ThreadDelta > 0)
					{
						if (stop == FALSE)
						{
							mygr.math_min_y += mygr.math_width_y * percent * (1.0 - 0.98);
							mygr.math_width_y = mygr.math_width_y * 0.98;
						}
					}
					else
					{
						mygr.math_width_y = mygr.math_width_y / 0.98;
						mygr.math_min_y -= mygr.math_width_y * percent * (1.0 - 0.98);
					}
					CheckScaleY(&mygr);
					break;
				case DisplayMode::SeismicGrath:
				case DisplayMode::ImpositionGraths:
					float PrevVal;
					float NextVal;
					CalculateStunnerValues((float)mygr.math_width_y, &PrevVal, &NextVal);
					
					if (ThreadDelta > 0)
					{
						if (stop == FALSE)
						{
							mygr.math_min_y += (mygr.math_width_y - PrevVal) / 2 * percent;
							mygr.math_width_y = PrevVal;
						}
					}
					else
					{
						mygr.math_min_y -= (NextVal - mygr.math_width_y) / 2 * percent;
						mygr.math_width_y = NextVal;
					}
					CheckScaleY(&mygr);

					break;
				default:
					break;
			}

			break;
	}

	_critSect.Unlock();
	InvalidateControl();
	return 0;
}

int ScrollZona(0);
HWND handleMain;
int ThreadMultiply;
UINT StatusCheck(LPVOID lParam)
{
	for (int i = 0; i < ThreadMultiply; i++)
	{
		::PostMessage(handleMain, WM_USER_DINAMICSCROLL, 0, ScrollZona);
		Sleep(20);
	}
	return 0;
}

BOOL CTimeGridBasic::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int zona;
	CPoint point;
	CRect user;
	if ((nFlags == MK_CONTROL) || (nFlags == MK_SHIFT))
	{
		GetWindowRect(&user);
		if (user.PtInRect(pt) != 0)
		{
			mwl = true;
			point.x = pt.x - user.left;
			point.y = pt.y - user.top;
			zona = MouseCursor(point);
			handleMain = this->m_hWnd;
			ThreadDelta = zDelta;
			if ((nFlags == MK_CONTROL && !mygr.bVerticalGraphs) || 
				(nFlags == MK_SHIFT   &&  mygr.bVerticalGraphs))
				ScrollZona = zona;
			else
				ScrollZona = -1;
			if (mygr.bVerticalGraphs)
			{
				mygr.mouse_y = point.x;
				mygr.mouse_x = point.y;
				MouseWheelPosY = point.x;
			}
			else
			{
				mygr.mouse_y = point.y;
				mygr.mouse_x = point.x;
				MouseWheelPosY = point.y;
			}

			if (((mygr.Display_Mode == DisplayMode::SeismicGrath) || (mygr.Display_Mode == DisplayMode::ImpositionGraths)) && 
				((ScrollZona == -1) || (ScrollZona == 2) || (ScrollZona == 3)))
				ThreadMultiply = 1;
			else
				ThreadMultiply = 20;
			statusThread = AfxBeginThread(StatusCheck, 0);
		}
	}
	else
	{
		_critSect.Lock();

		if (m_focus == 0)
		{
			m_focus = 1;
			mygr.update |= UPDATE_CURSOR_NEW;
			Invalidate(false);
			_critSect.Unlock();
			return TRUE;
		}

		GetWindowRect(&user);
		if (user.PtInRect(pt) != 0)
		{
			mwl = true;
			point.x = pt.x - user.left;
			point.y = pt.y - user.top;
			zona = MouseCursor(point);

			switch (zona)
			{
				case 1:
				case 4:
					switch (mygr.Display_Mode)
					{
						case DisplayMode::Default:
						case DisplayMode::SeismicGrath:
						case DisplayMode::ImpositionGraths:
							if (zDelta > 0)
								mygr.math_min_y += mygr.math_width_y*0.01;
							else
								mygr.math_min_y -= mygr.math_width_y*0.01;
							
							CheckScaleY(&mygr);
							break;
						default:
							break;
					}					
					break;
				case 2:
				case 3:
					switch (mygr.Display_Mode)
					{
						case DisplayMode::Default:
							if (zDelta > 0)
							{
								if (stop == FALSE)
								{
									mygr.math_min_y += mygr.math_width_y*0.01;
									mygr.math_width_y = mygr.math_width_y*0.98;
								}
							}
							else
							{
								mygr.math_width_y = mygr.math_width_y / 0.98;
								mygr.math_min_y -= mygr.math_width_y*0.01;
							}
							CheckScaleY(&mygr);
							break;
						case DisplayMode::SeismicGrath:
						case DisplayMode::ImpositionGraths:
							float PrevVal;
							float NextVal;
							CalculateStunnerValues((float)mygr.math_width_y, &PrevVal, &NextVal);
							if (zDelta > 0)
							{
								if (stop == FALSE)
								{
									mygr.math_min_y += (mygr.math_width_y - PrevVal) / 2;
									mygr.math_width_y = PrevVal;
								}
							}
							else
							{
								mygr.math_min_y -= (NextVal - mygr.math_width_y) / 2;
								mygr.math_width_y = NextVal;
							}
							CheckScaleY(&mygr);
							break;
						default:
							break;
					}
					break;
				case 5:
				case 8:
					if (zDelta < 0)
					{
						mygr.math_min_x = mygr.math_min_x - mygr.math_width_x*0.01;
					}
					else
					{
						mygr.math_min_x = mygr.math_min_x + mygr.math_width_x*0.01;
					}
					CheckScaleX();
					break;
				case 0:
				case 6:
				case 7:
					//полоса, располагаемая в нижней части экрана
					//изменение масштаба по горизонтали
					ChangeXScale(zDelta);
					break;
			}//switch (zona)
			mygr.update |= UPDATE_DRAW_ALL;

		}// user.PtInRect(pt) != 0
		_critSect.Unlock();
		InvalidateControl();
	}

	return COleControl::OnMouseWheel(nFlags, zDelta, pt);
}

void CTimeGridBasic::OnMButtonDown(UINT nFlags, CPoint point)
{
	MouseCursor(point);
	SetFocus();
	COleControl::OnMButtonDown(nFlags, point);
}

void CTimeGridBasic::OnRButtonDown(UINT nFlags, CPoint point)
{
	GetCursorPos(&point);
	RightClickPoint(point);

	MouseCursor(point);
	COleControl::OnRButtonDown(nFlags, point);
}

void CTimeGridBasic::OnMButtonUp(UINT nFlags, CPoint point)
{
	MouseCursor(point);
	COleControl::OnMButtonUp(nFlags, point);
}

void CTimeGridBasic::SetNewFont(_In_opt_ bool bBuildFont /*= false*/)
{
	if (m_FontNew != nullptr)
	{
		DeleteFont(m_FontNew);
		m_FontNew = nullptr;
	}

	m_FontNew = CreateFont(-mygr.font_size,	// height_y Of Font
						   long((double)(mygr.font_size) / 2.3),	// Width Of Font
						   0,				       				// Angle Of Escapement
						   0,				       				// Orientation Angle
						   FW_NORMAL,							// Font Weight
						   0,			       					// Italic
						   0,			       					// Underline
						   0,			       					// Strikeout
						   RUSSIAN_CHARSET,					// Character Set Identifier
						   OUT_TT_PRECIS,						// Output Precision
						   CLIP_DEFAULT_PRECIS,				// Clipping Precision
						   ANTIALIASED_QUALITY,	       		// Output Quality
						   FF_DONTCARE | DEFAULT_PITCH,     	// Family And Pitch
						   mygr.font_name);

	if (bBuildFont)
		BuildFontOGL(m_hDC, m_FontNew);

	m_bNeedLoadNewFont = true;
}
