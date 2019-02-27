//------------------------------------------------------------------------------
// NumIndicator.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Custom\CustomIndicator.h"
#include "C:\ZETTools\Interface2012\Draw\DNumIndicator.h"
//------------------------------------------------------------------------------
class CNumIndicator : public CCustomIndicator
{
public:
	CNumIndicator(CCustomContainer *owner, CRect rect);
	virtual ~CNumIndicator();

private:
	CDNumIndicator *m_pDNumIndicator;
protected:

public:
	virtual double GetAmplitude() { return m_pDNumIndicator->GetAmplitude(); }
	virtual void SetAmplitude(double val) { m_pDNumIndicator->SetAmplitude((float)val); }

	float GetThresholdDanger() { return m_pDNumIndicator->GetThresholdDanger(); }
	void SetThresholdDanger(float val) { m_pDNumIndicator->SetThresholdDanger(val); }

	TypePosition GetPositionType() { return m_pDNumIndicator->GetPositionType(); }

	long GetLinLog() { return m_pDNumIndicator->GetLinLog(); }
	void SetLinLog(long val) { m_pDNumIndicator->SetLinLog(val); }

	long GetSizeString() { return m_pDNumIndicator->GetSizeString(); }
	void SetSizeString(long postcomma) { m_pDNumIndicator->SetSizeString(postcomma); }

	int GetDistance() { return m_pDNumIndicator->GetDistance(); }
	void SetDistance(int val) { m_pDNumIndicator->SetDistance(val); }

	int GetBorderWidth() { return m_pDNumIndicator->GetBorderWidth(); }
	void SetBorderWidth(int val) { m_pDNumIndicator->SetBorderWidth(val); }

	COLORREF GetBorderColor() { return m_pDNumIndicator->GetBorderColor(); }
	void SetBorderColor(COLORREF val) { m_pDNumIndicator->SetBorderColor(val); }

	COLORREF GetBackColor() { return m_pDNumIndicator->GetBackColor(); }
	void SetBackColor(COLORREF val) { m_pDNumIndicator->SetBackColor(val); }

	double GetTimeAnimation() { return m_pDNumIndicator->GetTimeAnimation(); }
	void SetTimeAnimation(double val) { m_pDNumIndicator->SetTimeAnimation(val); }

	int GetPrecision() { return m_pDNumIndicator->GetPrecision(); }
	void SetPrecision(int val) { m_pDNumIndicator->SetPrecision(val); }

	void SetIntegerPart (int val) {m_pDNumIndicator->SetIntegerPart(val);}
	int GetIntegerPart (void) { return m_pDNumIndicator->GetIntegerPart();}

	void SetTextColor (COLORREF colortext) { m_pDNumIndicator->SetTextColor(colortext); }
	COLORREF GetTextColor (void) { return m_pDNumIndicator->GetTextColor(); }

	void SetTextSize (float val) {m_pDNumIndicator->SetTextSize(val);}
	float GetTextSize (void) {return m_pDNumIndicator->GetTextSize(); }

	void SetTextAmplitude (CString val) { m_pDNumIndicator->SetTextValue(val); }
	CString GetTextAmplitude (void) { return m_pDNumIndicator->GetTextVal(); }

	void SetTypeValue (bool val) { m_pDNumIndicator->SetTypeView(val); }
	bool GetTypeValue (void) { return m_pDNumIndicator->GetTypeView(); }

	void SetTextCeils (bool val) { m_pDNumIndicator->SetTextCeils(val); }
	bool GetTextCeils (void) { return m_pDNumIndicator->GetTextCeils(); }

#ifdef _GDIPLUS
#else
	CDFonts* GetFont() { return m_pDNumIndicator->GetFont(); }
	void SetFont(CDFonts *pfont) { m_pDNumIndicator->SetFont(pfont); }
#endif

	// виртуальные функции, переопределяемые в классе
	virtual void OnLButtonClk(UINT nFlags, CPoint point);
    virtual void SaveParameters(SimpleXML *pSimpleXML);
    virtual void LoadParameters(SimpleXML *pSimpleXML);
};
//------------------------------------------------------------------------------