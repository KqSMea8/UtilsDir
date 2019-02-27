//------------------------------------------------------------------------------
// NumSelector.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Custom\CustomRegulator.h"
#include "C:\ZETTools\Interface2012\Draw\DNumSelector.h"
//------------------------------------------------------------------------------
class CNumSelector : public CCustomRegulator
{
public:
	CNumSelector(CCustomContainer *owner, CRect rect);
	virtual ~CNumSelector();
private:
	CDNumSelector *m_pDNumSelector;
	CRect m_rect;
	double CurValue;
	int BolderWidth;
protected:
public:
	// функции параметров отображения
	CRect rect_dot;
	int width;
	int* GetMask() { return m_pDNumSelector->GetMask();}
	void SetMask(int integer, int fraction) {m_pDNumSelector->SetMask(integer, fraction);}

	double GetTimeAnimation() { return m_pDNumSelector->GetTimeAnimation();}
	void SetTimeAnimation(double val) { m_pDNumSelector->SetTimeAnimation(val); }

	double GetCurValue() {return m_pDNumSelector->GetCurValue();}
	void SetCurValue(double val) {m_pDNumSelector->SetCurValue(val);}

	double GetMaxLimit() {return m_pDNumSelector->GetMaxLimit();}
	void SetMaxLimit(double val) {m_pDNumSelector->SetMaxLimit(val);}

	double GetMinLimit() {return m_pDNumSelector-> GetMinLimit();}
	void SetMinLimit(double val) {m_pDNumSelector->SetMinLimit(val);}

	int GetBolderWidth() {return m_pDNumSelector->GetBolderWidth();}
	void SetBolderWidth(int Width) {m_pDNumSelector->SetBolderWidth(Width);}

	// виртуальные функции, переопределяемые в классе
	virtual void OnMouseLeave();
	//virtual void OnMouseEnter();
	//virtual void OnDraw();
	//virtual void OnLButtonClk(UINT nFlags, CPoint point);
	//virtual void OnLButtonUp(UINT nFlags, CPoint point);
};
//------------------------------------------------------------------------------