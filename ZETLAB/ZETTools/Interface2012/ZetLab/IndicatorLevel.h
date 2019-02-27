//------------------------------------------------------------------------------
// IndicatorLevel.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Custom\CustomIndicator.h"
#include "C:\ZETTools\Interface2012\Draw\DIndicatorLevel.h"
//------------------------------------------------------------------------------
class CIndicatorLevel : public CCustomIndicator
{
public:
	CIndicatorLevel(CCustomContainer *owner, CRect rect);
	virtual ~CIndicatorLevel();

private:
	CDIndicatorLevel *m_pDIndicator;
protected:

public:
	virtual double GetAmplitude() { return m_pDIndicator->GetAmplitude(); }
	virtual void SetAmplitude(double val) { m_pDIndicator->SetAmplitude((float)val); }
	
	float GetThresholdDanger() { return m_pDIndicator->GetThresholdDanger(); }
	void SetThresholdDanger(float val) { m_pDIndicator->SetThresholdDanger(val); }

	TypePosition GetPositionType() { return m_pDIndicator->GetPositionType(); }
	
	long GetLinLog() { return m_pDIndicator->GetLinLog(); }
	void SetLinLog(long val) { m_pDIndicator->SetLinLog(val); }

	int GetDistance() { return m_pDIndicator->GetDistance(); }
	void SetDistance(int val) { m_pDIndicator->SetDistance(val); }
	
	int GetBorderWidth() { return m_pDIndicator->GetBorderWidth(); }
	void SetBorderWidth(int val) { m_pDIndicator->SetBorderWidth(val); }
	
	COLORREF GetBorderColor() { return m_pDIndicator->GetBorderColor(); }
	void SetBorderColor(COLORREF val) { m_pDIndicator->SetBorderColor(val); }

	COLORREF GetBackColor() { return m_pDIndicator->GetBackColor(); }
	void SetBackColor(COLORREF val) { m_pDIndicator->SetBackColor(val); }

	CString GetFileNameTriangle() { return m_pDIndicator->GetFileNameTriangle(); }
	void SetFileNameTriangle(CString str) { m_pDIndicator->SetFileNameTriangle(str); }

	double GetTimeAnimation() { return m_pDIndicator->GetTimeAnimation(); }
	void SetTimeAnimation(double val) { m_pDIndicator->SetTimeAnimation(val); }

	// виртуальные функции, переопределяемые в классе
	virtual void OnLButtonClk(UINT nFlags, CPoint point);
};
//------------------------------------------------------------------------------