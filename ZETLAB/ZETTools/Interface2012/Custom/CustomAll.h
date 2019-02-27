//------------------------------------------------------------------------------
// CustomGDI_ZET.h : ���� ���������
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
	CCustomDraw *m_pDClass;		// ��������� �� ������ ����� �����������
	CCustomAll *m_pCOwner;		// ��������� �� ����� �������� ��������
	HCURSOR m_hCursor;			// ����������� ������
	bool m_bToolTipEnabled;		// ���� ToolTip

public:
	long m_IndexTabFocus;		// ������ ������ (0, 1, 2, ...) ��� -1
	bool m_butFixation;		// ���� ������� ������
	eButtonType m_butType;  //��� ������ 
	bool BelongsMenu; //�������������� � �������������� �������
	bool OpenMenu; //���� ����, ��� ������ ���������� ������
	bool Removal; //������� �� ��������
	bool m_Press; //���� "�������" �������������� ���������
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

	// ���������� ��������� �������� -------------
	virtual CString GetFolderPNG();
#ifdef _GDIPLUS
	virtual Gdiplus::Graphics** GetPointerToGraphics();
#endif

	virtual void Clear(){}					//	�������� ��� ���������� ������ ��� ��������
	virtual double* GetPointerToTime();
	virtual bool* GetPointerToIsChange();
	virtual CWnd* GetParent();
	virtual double GetTime();
	
	// ������� ����� ���������� �����������
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

	// �������, ���������� �������� ���������� �����������
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

	//������� ��� ��������� ��������������� ���� (Combobox, ChanSelector)
	virtual void CloseMenu() {}
	virtual void SetParamAddRect(CRect rect){}
	virtual void SetPress(bool val) {} 
};
//------------------------------------------------------------------------------