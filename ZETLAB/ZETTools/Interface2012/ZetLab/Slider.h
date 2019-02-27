//------------------------------------------------------------------------------
// Slider.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Custom\CustomSlider.h"
#include "C:\ZETTools\Interface2012\Draw\DSlider.h" //путь!!!!
//------------------------------------------------------------------------------
class CSlider : public CCustomSlider
{
public:
	CSlider(CCustomContainer *owner, CRect rect);
	virtual ~CSlider();

private:
	CDSlider *m_pDSlider;
protected:

public:
	// функции параметров отображения
	TypePosition GetPositionType() { return m_pDSlider->GetPositionType();}

	float GetPositionValue() { return m_pDSlider->GetPositionValue();}
	void SetPositionValue(float val);

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
    virtual void SaveParameters(SimpleXML *pSimpleXML);
    virtual void LoadParameters(SimpleXML *pSimpleXML);
	virtual void OnLButtonClk(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
};
//------------------------------------------------------------------------------