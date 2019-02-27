//------------------------------------------------------------------------------
// CustomGDI_ZET.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include <Interface2012\Draw\CustomDraw.h>
#define IDM_PICK_ITEM WM_USER+1000
//------------------------------------------------------------------------------
class CCustomAll : public CObject
{
public:
	CCustomAll();
	virtual ~CCustomAll();
private:
	bool m_bFlagClick;
	CPoint m_PointClick;

protected:
	CCustomDraw *m_pDClass;		// указатель на базвый класс отображени€
	CCustomAll *m_pCOwner;		// указатель на класс будущего родител€
	HCURSOR m_hCursor;			// собственный курсор
	bool m_bToolTipEnabled;		// есть ToolTip

public:
	long m_IndexTabFocus;		// индекс фокуса (0, 1, 2, ...) или -1
	bool m_butFixation;		// флаг нажати€ кнопки
	eButtonType m_butType;  //тип кнопки 
	bool BelongsMenu; //принадлежность к раскрывающимс€ спискам
	bool OpenMenu; //флаг того, что открыт выпадающий список
	bool Removal; //пометка на удаление
	bool m_Press; //флаг "зажати€" раскрывающихс€ элементов
	void SetCursor(LPCWSTR pNameCursor);
	void SetCursorFromFile(LPCWSTR pNameCursor);
	HCURSOR GetHCursor() { return m_hCursor; }

#ifdef _GDIPLUS
#else
	virtual void SaveParameters(SimpleXML *pSimpleXML){ m_pDClass->SaveParameters(pSimpleXML); };
	virtual void LoadParameters(SimpleXML *pSimpleXML){ m_pDClass->LoadParameters(pSimpleXML); };
#endif

	// ToolTip -----------------------------------
	BOOL AddToolTip(CString str);
	BOOL AddToolTip(UINT nIDS_STRING);
	void UpdateToolTipText(CString str);
	void UpdateToolTipText(UINT nIDS_STRING);

	// реализаци€ механизма владени€ -------------
	virtual CString GetFolderPNG();
#ifdef _GDIPLUS
	virtual Gdiplus::Graphics** GetPointerToGraphics();
#endif

	virtual void Clear(){}					//	очистить всю выделенную пам€ть дл€ объектов
	virtual double* GetPointerToTime();
	virtual bool* GetPointerToIsChange();
	virtual CWnd* GetParent();
	virtual double GetTime();
	
	// функции общих параметров отображени€
	int GetNumberElement() { return m_pDClass->GetNumberElement(); }

	CRect GetRect() { return m_pDClass->GetRect(); }
	void SetRect(CRect val) { m_pDClass->SetRect(val); }

	CString GetName() { return m_pDClass->GetName(); }
	void SetName(CString str) { m_pDClass->SetName(str); }

	bool GetVisible() { return m_pDClass->GetVisible(); }
	void SetVisible(bool val) { m_pDClass->SetVisible(val); }

	bool GetEnable() { return m_pDClass->GetEnable(); }
	void SetEnable(bool val) { m_pDClass->SetEnable(val); }

	bool GetAnimation() { return m_pDClass->GetAnimation(); }
	void SetAnimation(bool val) { m_pDClass->SetAnimation(val); }

	bool GetFocusStatus() { return m_pDClass->GetFocusStatus();}
	void GetFocus() { m_pDClass->GetFocus(); }
	void KillFocus() { m_pDClass->KillFocus(); }

	bool GetMouseStatus() { return m_pDClass->GetMouseStatus(); }
	virtual void OnMouseEnter() { m_pDClass->OnMouseEnter(); }
	virtual void OnMouseLeave();

	bool GetMouseChange() { return m_pDClass->GetMouseChange(); }
	void SetMouseChange(bool val) { m_pDClass->SetMouseChange(val); }

	eButtonType GetButtonType() {return m_pDClass->GetButtonType(); }
	virtual void SetButtonType(eButtonType val) { m_butType = val; }

	bool GetBelongsMenu() {return BelongsMenu;}
	void SetBelongsMenu(bool val) { BelongsMenu = val;}

	bool GetElementState (void) {return m_butFixation;}
	virtual void SetElementState(bool val) { m_butFixation = val; }

	// функции, измен€ющие значени€ параметров отображени€
	virtual void DoProcess() { m_pDClass->DoProcess(); }
	virtual void OnDraw() { m_pDClass->OnDraw(); }
	virtual void Resize(CRect newRect){};

	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	
	virtual void OnChar(UINT nChar, LPARAM nFlags)
	{ m_pDClass->OnChar(nChar, nFlags); }
	virtual void OnMouseEnter(UINT nFlags, CPoint point)
	{ m_pDClass->OnMouseEnter(); }
	virtual void OnLButtonClk(UINT nFlags, CPoint point)
	{ m_pDClass->OnLButtonClk(nFlags, point);}
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point)
	{ m_pDClass->OnLButtonDblClk(nFlags, point); }
	virtual void OnRButtonUp(UINT nFlags, CPoint point);
	//{ m_pDClass->OnRButtonUp(nFlags, point); }
	virtual void OnRButtonDown(UINT nFlags, CPoint point);
	//{ m_pDClass->OnRButtonDown(nFlags, point); }
	virtual void OnMouseWheel(UINT nFlags, CPoint point, short delta)
	{ m_pDClass->OnMouseWheel(nFlags, point, delta); }

	//функции дл€ элементов раскрывающегос€ меню (Combobox, ChanSelector)
	virtual void CloseMenu() {}
	virtual void SetParamAddRect(CRect rect){}
	virtual void SetPress(bool val) {} 
};
//------------------------------------------------------------------------------