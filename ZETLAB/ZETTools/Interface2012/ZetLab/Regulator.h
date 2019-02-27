//------------------------------------------------------------------------------
// Regulator.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Custom\CustomRegulator.h"
#include "C:\ZETTools\Interface2012\Draw\DRegulator.h"
//------------------------------------------------------------------------------
class CRegulator : public CCustomRegulator
{
public:
	CRegulator(CCustomContainer *owner, CRect rect, bool variant, bool need_scale, float start_angle);
	// variant - способ прокрутки регулятора
	// need_scale  - если true рисует шкалу вокруг регулятора и включается ограничитель поворота 
	// start_angle - начальный угол поворота регулятора ( 0 - регулятор смотрит в право, 90 - вниз, 180 - влево, 270 - вверх)
	virtual ~CRegulator();
	CDRegulator *m_pDRegulator;
private:
	bool m_variant;
protected:

public:
	// функции параметров отображения
	CDFonts* GetFont() { return m_pDRegulator->GetFont(); }
	void SetFont(CDFonts *pfont) { m_pDRegulator->SetFont(pfont); }

	CString GetCaption() { return m_pDRegulator->GetCaption(); }
	void SetCaption(CString val) { m_pDRegulator->SetCaption(val); }

	void DrawCaption (bool var) { m_pDRegulator->DrawCaption (var); }

	double GetTimeAnimation() { return m_pDRegulator->GetTimeAnimation();}
	void SetTimeAnimation(double val) { m_pDRegulator->SetTimeAnimation(val); }

	bool GetRegulatorType() { return m_pDRegulator->GetRegulatorType();}
	void SetRegulatorType(bool val) { m_pDRegulator->SetRegulatorType(val);}

	float GetBorderWidth() { return m_pDRegulator->GetBorderWidth();}
	void SetBorderWidth(float val) { m_pDRegulator->SetBorderWidth(val);}
	void SetRegulatorValue (float angle) { m_pDRegulator->SetRegulatorValue(angle);}
	float GetRegulatorValue (void) { return m_pDRegulator->GetRegulatorValue();}

	void SetTurnCoeff (float turn_coeff) { m_pDRegulator->SetTurnCoeff(turn_coeff);}

	void SetScale (int quan);
	int GetScale (void);

	void SetBlocking (bool val) { m_pDRegulator->SetBlocking (val); }
	bool GetBlocking (void) { return m_pDRegulator->GetBlocking(); }

	void SetCenter (bool center)
	{ 
		m_pDRegulator->SetCenter(center);
	}

	// виртуальные функции, переопределяемые в классе
	virtual void OnMouseLeave();
	virtual void Clear (void);
    virtual void SaveParameters(SimpleXML *pSimpleXML);
	virtual void OnLButtonClk(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
};
//------------------------------------------------------------------------------