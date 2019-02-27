#pragma once
#include <afxmt.h>
#include "Struct_trend.h"
#include <Grafic1\GraficTools\ColorLine\ColorLine.h>
#include "Grafic1\GraficTools\TimeGridBasic\TimeGridDrow.h"
//*************************************************************************************************
#define UPDATE_FONT						0x1
#define UPDATE_CURSOR_MOVE				0x2
#define UPDATE_DRAW_ALL					0x4
#define UPDATE_CLIPBOARD				0x8
#define UPDATE_CURSOR_NEW				0x10
#define UPDATE_NEW_SIZE					0x20
#define UPDATE_CLIPBOARD_TEXT			0x80
#define UPDATE_CLIPBOARD_DIGIT			0x100
#define UPDATE_CLIPBOARD_DIGIT_ONLY		0x200
#define UPDATE_SELECT_GRAPH				0x800			// перерисовываем только для выячисления какой график был выделен
#define UPDATE_SAVE_BITMAP				0x1000			// сохраняем текущее изображение сетки
#define WM_USER_DINAMICSCROLL			(WM_USER + 4)	// прокрутка колесом мыши
//*************************************************************************************************
// размеры графика
struct GraphPos
{
	double math_min_y;
	double math_width_y;
	double math_min_x;
	double math_width_x;
	GraphPos()
		: math_min_y(0),
		math_width_y(0),
		math_min_x(0),
		math_width_x(0)
	{
	}
	bool IsEmpty()
	{
		if ((math_min_y == 0) && (math_width_y == 0) && (math_min_x == 0) && (math_width_x == 0))
			return true;
		else
			return false;
	}
};
//*************************************************************************************************
class CTimeGridBasic : public COleControl, public CTimeGridDrow
{
public:
	CTimeGridBasic();
	~CTimeGridBasic();
protected:
	CCriticalSection _critSect;
	bool m_bEnable;
	int m_focus;
	//*********************************************************************************************
	bool mwl;
	CWinThread *statusThread;
	//*********************************************************************************************
	HCURSOR cur_move,
		cur_up,
		cur_down,
		cur_vrsdv,
		cur_vrrsd,
		cur_left,
		cur_right,
		cur_hrsdv,
		cur_hrrsd,
		cur_norma,
		cur_hand,
		cur_arrow;
	//*********************************************************************************************
	bool m_DrowDistinguish;							// флаг рисование рамки	
	//*********************************************************************************************
	CPoint PrevMousePosCurs;						// координаты мышки при выборе курсора
	CPoint CurMousePos;								// конечные координаты мышки при завершении выделения рамки
	CPoint PrevMousePos;							// координаты мышки при начале выделения рамки 
	CPoint LBDownMousePos;							// координаты мышки при зажатии ЛКМ
	int MouseWheelPosY;								// координаты мышки при кручении колесика мыши в зоне 0
	GraphPos PrevGraphPos;							// размеры графика при начале выделения рамки
	//*********************************************************************************************
	TrendGrid mygr, copy_grid;						// структуры сетки
	//*********************************************************************************************
protected://***************************************************************************************
	int ForCreate(void);
	//*********************************************************************************************
	// установка нового шрифта для записей, bBuildFont - установка шрифта для сетки
	void SetNewFont(_In_opt_ bool bBuildFont = false);	
	//*********************************************************************************************
	virtual void CheckScaleX();						// сообщение что нужно проверить размеры сетки по оси Х
	virtual void CursMoving(bool bVert, int Index) {};	// событие возникающее при перемещении курсора
	virtual void SendCursDistance() = 0;			// нужно отправить расстоиние между курсорами
	virtual void NeedLoadDataForAutoScale() = 0;	// нужно загрузить данные для масштабирования
	virtual void ChoosePoint(CPoint point) = 0;		// нажатие в рабочую область графика
	virtual void RightClickPoint(CPoint point) = 0;	// нажатие правой кнопкой мыши
private:
	void MoveCursor(CPoint point);			// событие возникающие при перетаскивании вертикального курсора
	int MouseCursor(CPoint point);
	void ChangeXScale(short zDelta);
	//*********************************************************************************************
	afx_msg LRESULT	OnDynamicScroll(WPARAM wParam, LPARAM lParam);
	//*********************************************************************************************
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
};

