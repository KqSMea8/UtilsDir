//------------------------------------------------------------------------------
// NumIndicator.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Custom\CustomIndicator.h"
#include "C:\ZETTools\Interface2012\Draw\DLevelIndicator.h"
//------------------------------------------------------------------------------
class CLevelIndicator : public CCustomIndicator
{
public:
	CLevelIndicator(CCustomContainer *owner, CRect rect, bool rotate, bool setrotate);
	virtual ~CLevelIndicator();

private:
	CDLevelIndicator *m_pDLevelIndicator;
protected:

public:
	virtual double GetAmplitude() { return m_pDLevelIndicator->GetAmplitude(); }
	virtual void SetAmplitude(double val) { m_pDLevelIndicator->SetAmplitude((float)val); }

	float GetMaxLevel () {return m_pDLevelIndicator->GetMaxLevel(); }
	void SetMaxLevel (float valevel) {m_pDLevelIndicator->SetMaxLevel(valevel); }

	float GetThresholdDanger() { return m_pDLevelIndicator->GetThresholdDanger(); }
	void SetThresholdDanger(float val) { m_pDLevelIndicator->SetThresholdDanger(val); }

	TypePosition GetPositionType() { return m_pDLevelIndicator->GetPositionType(); }

	long GetLinLog() { return m_pDLevelIndicator->GetLinLog(); }
	void SetLinLog(long val) { m_pDLevelIndicator->SetLinLog(val); }

	int GetDistance() { return m_pDLevelIndicator->GetDistance(); }
	void SetDistance(int val) { m_pDLevelIndicator->SetDistance(val); }

	int GetBorderWidth() { return m_pDLevelIndicator->GetBorderWidth(); }
	void SetBorderWidth(int val) { m_pDLevelIndicator->SetBorderWidth(val); }

	COLORREF GetBorderColor() { return m_pDLevelIndicator->GetBorderColor(); }
	void SetBorderColor(COLORREF val) { m_pDLevelIndicator->SetBorderColor(val); }

	COLORREF GetBackColor() { return m_pDLevelIndicator->GetBackColor(); }
	void SetBackColor(COLORREF val) { m_pDLevelIndicator->SetBackColor(val); }

	double GetTimeAnimation() { return m_pDLevelIndicator->GetTimeAnimation(); }
	void SetTimeAnimation(double val) { m_pDLevelIndicator->SetTimeAnimation(val); }

	// виртуальные функции, переопределяемые в классе
	virtual void OnLButtonClk(UINT nFlags, CPoint point);
    virtual void SaveParameters(SimpleXML *pSimpleXML);
	virtual void Clear (void);
};
//------------------------------------------------------------------------------