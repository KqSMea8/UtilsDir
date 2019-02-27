//------------------------------------------------------------------------------
// ZScrollBar.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Custom\CustomSlider.h"
#include "C:\ZETTools\Interface2012\Draw\DScrollBar.h"
static bool flagScroll; //флаг разрешения на прокрутку элементов
//------------------------------------------------------------------------------
class CZScrollBar : public CCustomSlider
{
public:
	CZScrollBar(CCustomContainer *owner, CRect rect);
	virtual ~CZScrollBar();

private:
	CDScrollBar *m_pDScrollBar;

protected:

public:
	// функции параметров отображения
	TypePosition GetPositionType() { return m_pDScrollBar->GetPositionType();}

	float GetPositionValue() { return m_pDScrollBar->GetPositionValue();}
	void SetPositionValue(float val);

	int GetDeltaX() { return m_pDScrollBar->GetDeltaX();}
	void SetDeltaX(int val) { m_pDScrollBar->SetDeltaX(val); }

	int GetDeltaY() { return m_pDScrollBar->GetDeltaY();}
	void SetDeltaY(int val) { m_pDScrollBar->SetDeltaY(val); }

	CString GetFileNameField() { return m_pDScrollBar->GetFileNameField();}
	void SetFileNameField(CString str, double angle) { m_pDScrollBar->SetFileNameField(str, angle); }

	CString GetFileNameCursor() { return m_pDScrollBar->GetFileNameCursor();}
	void SetFileNameCursor(CString str, double angle) { m_pDScrollBar->SetFileNameCursor(str, angle); }

	double GetTimeAnimation() { return m_pDScrollBar->GetTimeAnimation();}
	void SetTimeAnimation(double val) { m_pDScrollBar->SetTimeAnimation(val); }

	// виртуальные функции, переопределяемые в классе
	virtual void OnMouseLeave();
	virtual void Clear (void);
	virtual void SaveParameters(SimpleXML *pSimpleXML);
	virtual void LoadParameters(SimpleXML *pSimpleXML);
	virtual void OnLButtonClk(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);

	void LiftScrollBar(float val) { m_pDScrollBar->LiftScrollBar(val);}
	float GetValueCur() { return m_pDScrollBar->GetValueCur();}
	void SetValueCur(float val) { m_pDScrollBar->SetValueCur(val);}
	void PointerForScroll(void (*val)()) { m_pDScrollBar->PionterForScroll(val);}
	
	bool GetNowUsing() { return m_pDScrollBar->GetNowUsing(); }
	void SetNowUsing(bool val) { m_pDScrollBar->SetNowUsing(val); }

};
//------------------------------------------------------------------------------