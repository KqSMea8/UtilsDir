//------------------------------------------------------------------------------
// CustomDraw.h : ���� ���������
//------------------------------------------------------------------------------
//	������� ����� ��� ������� ����������� ��������� ���������� 2012 �.
//------------------------------------------------------------------------------
#pragma once
#include <Interface2012\Custom\FunctionForAll.h>
#include <SimpleXML\SimpleXML.h>
//------------------------------------------------------------------------------
//		���� ���������������� ��������
enum TypePosition
{
	tpHorizontal,	// ��������������
	tpVertical		// ������������
};
//------------------------------------------------------------------------------
struct ParamCustomDraw
{// ������������ ��������� ���������� �����������, ���������� ��������
// ����� ����������. � �������-����������� ����� ����������� ����
// ���������, ����������� �� ����.
	DWORD Size;			// ������ ��������� � ������
	bool bVisible;		// ������� �������
	bool bEnable;		// ������� ���������
	bool bFocus;		// ����� � ��������
	bool bMouseCome;	// ���� ��� ���������
	bool bMouseChange;	// ������� � ����� ������
	bool bAnimation;	// ������� ��������� ��������
	int  nElement;		// ����� �������� ������� ������ � ���������
	CString Name;		// ��� ����������
	ParamCustomDraw()
		: bVisible(true)
		, bEnable(true)
		, bFocus(false)
		, bMouseCome(false)
		, bMouseChange(false)
		, bAnimation(false)
		, nElement(1)
		, Name(L"")
		, Size(0)
	{
	}
};
//------------------------------------------------------------------------------
class CCustomDraw
{
public:
#ifdef _GDIPLUS
	CCustomDraw(CRect rect, PVOID ppGraphics,
		bool *pFlagIsChange, double *pTimeAnimation = NULL);
#else
	CCustomDraw(CRect rect,
		bool *pFlagIsChange, double *pTimeAnimation = NULL);
#endif
	virtual ~CCustomDraw();
	eButtonType m_ButtonType;
private:
	bool *m_pbIsChange;
	double *m_pTimeAnimation;

protected:
	CRect m_rect;

#ifdef _GDIPLUS
	PVOID m_ppGraphics;		//	Graphics **m_ppGraphics;
#endif
	ParamCustomDraw *m_pParamDraw;

	// ��������� �������
	void IsChange();
	double GetTime();
	CPoint GetClientPoint(CPoint point);
	void SetNameWithNumber(CString str);


public:

#ifdef _GDIPLUS
#else

#endif

	// ������� ����� ���������� �����������
	virtual bool SetStruct(PVOID add);	// ����������� ���������� �� add � ���������
	virtual bool GetStruct(PVOID add);	// ����������� ���������� �� ��������� � add

	CRect GetRect() { return m_rect; }
	void SetRect(CRect val) { m_rect = val; }

	int GetNumberElement() { return m_pParamDraw->nElement; }

	bool GetVisible() { return m_pParamDraw->bVisible;}
	void SetVisible(bool val);

	bool GetEnable() { return m_pParamDraw->bEnable;}
	void SetEnable(bool val);

	bool GetAnimation() { return m_pParamDraw->bAnimation;}
	void SetAnimation(bool val) { m_pParamDraw->bAnimation = val; }

	bool GetFocusStatus() { return m_pParamDraw->bFocus;}
	virtual void GetFocus();
	virtual void KillFocus();

	bool GetMouseStatus() { return m_pParamDraw->bMouseCome;}
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();

	bool GetMouseChange() { return m_pParamDraw->bMouseChange;}
	void SetMouseChange(bool val) { m_pParamDraw->bMouseChange = val; }

	CString GetName() { return m_pParamDraw->Name; }
	void SetName(CString str) { m_pParamDraw->Name = str; }

	eButtonType GetButtonType() {return m_ButtonType;}
	void SetButtonType(eButtonType val) {m_ButtonType = val;}
 
	// ����������� �������, ���������������� � �������-�����������
	virtual void OnDraw(){}
	virtual void Clear(){}
    virtual void SaveParameters(SimpleXML *pSimpleXML){}
    virtual void LoadParameters(SimpleXML *pSimpleXML){}
	virtual void DoProcess(){}
	virtual void OnChar(UINT nChar, LPARAM nFlags){}
	virtual void OnMouseMove(UINT nFlags, CPoint point){}
	virtual void OnLButtonClk(UINT nFlags, CPoint point){}
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point){}
	virtual void OnLButtonUp(UINT nFlags, CPoint point){}
	virtual void OnLButtonDown(UINT nFlags, CPoint point){}
	virtual void OnRButtonUp(UINT nFlags, CPoint point){}
	virtual void OnRButtonDown(UINT nFlags, CPoint point){}
	virtual void OnMouseWheel(UINT nflags, CPoint point, short delta){}

    virtual int GetQntPlants(){return 0;};
//	virtual void SetRectForDraw(CRect val); // ����������� �������� ����� ������� ��� ��������� //�������� � �����

};
//------------------------------------------------------------------------------