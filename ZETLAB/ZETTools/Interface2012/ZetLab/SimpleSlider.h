//------------------------------------------------------------------------------
// SimpleSlider.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Custom\CustomSlider.h"
#include "C:\ZETTools\Interface2012\Draw\DSimpleSlider.h"
//------------------------------------------------------------------------------
class CSimpleSlider : public CCustomSlider
{
public:
	CSimpleSlider(CCustomContainer *owner, CRect rect);
	virtual ~CSimpleSlider();
	CDSimpleSlider *m_pDSlider;
private:
	
protected:

public:
	// функции параметров отображения
	TypePosition GetPositionType() { return m_pDSlider->GetPositionType();}

	BOOL GetPositionValue() { return m_pDSlider->GetPositionValue();}
	void SetPositionValue(BOOL val);

	int GetDeltaX() { return m_pDSlider->GetDeltaX();}
	void SetDeltaX(int val) { m_pDSlider->SetDeltaX(val); }
	
	int GetDeltaY() { return m_pDSlider->GetDeltaY();}
	void SetDeltaY(int val) { m_pDSlider->SetDeltaY(val); }

	CString GetFileNameField() { return m_pDSlider->GetFileNameField();}
	void SetFileNameField(CString str, double angle) { m_pDSlider->SetFileNameField(str, angle); }

	CString GetFileNameCursor() { return m_pDSlider->GetFileNameCursor();}
	void SetFileNameCursor(CString str, double angle) { m_pDSlider->SetFileNameCursor(str, angle); }
	
	double GetTimeAnimation() { return m_pDSlider->GetTimeAnimation();}
	void SetTimeAnimation(double val) { m_pDSlider->SetTimeAnimation(val); }

	// виртуальные функции, переопределяемые в классе
	virtual void OnMouseLeave();
	virtual void Clear();
    virtual void SaveParameters(SimpleXML *pSimpleXML);
    virtual void LoadParameters(SimpleXML *pSimpleXML);
	virtual void OnLButtonClk(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
};
//------------------------------------------------------------------------------