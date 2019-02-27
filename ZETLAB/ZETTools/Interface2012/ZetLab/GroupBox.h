//------------------------------------------------------------------------------
// GroupBox.h : файл заголовка
//------------------------------------------------------------------------------
// Класс элемента
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Custom\CustomContainer.h"
#include "C:\ZETTools\Interface2012\Draw\DGroupBox.h"
//------------------------------------------------------------------------------
class CGroupBox : public CCustomContainer
{
public:
	CGroupBox(CCustomContainer *owner, CRect rect);
	virtual ~CGroupBox();

private:
	CDGroupBox *m_pDGroupBox;

protected:

public:
	// функции параметров отображения
	CString GetCaption() { return m_pDGroupBox->GetCaption(); }
	void SetCaption(CString val) { m_pDGroupBox->SetCaption(val); }
#ifdef _GDIPLUS
	CZFont* GetFont() { return m_pDGroupBox->GetFont(); }
	void SetFont(CZFont *pfont) { m_pDGroupBox->SetFont(pfont); }
#else
	CDFonts* GetFont() { return m_pDGroupBox->GetFont(); }
	void SetFont(CDFonts *pfont) { m_pDGroupBox->SetFont(pfont); }
#endif
	int GetRadius() { return m_pDGroupBox->GetRadius(); }
	void SetRadius(int val) { m_pDGroupBox->SetRadius(val); }

	int GetLengthLine0() { return m_pDGroupBox->GetLengthLine0(); }
	void SetLengthLine0(int val) { m_pDGroupBox->SetLengthLine0(val); }

	float GetThickness (void) {return m_pDGroupBox->GetThickness();}
	void SetThickness (float val) {m_pDGroupBox->SetThickness(val);}

	COLORREF GetColor() { return m_pDGroupBox->GetColor(); }
	void SetColor(COLORREF val) { m_pDGroupBox->SetColor(val); }

	COLORREF GetTextColor () { return m_pDGroupBox->GetTextColor(); }
	void SetTextColor (COLORREF val) { m_pDGroupBox->SetTextColor(val); }

	float GetCoordZ() { return m_pDGroupBox->GetCoordZ();}
	void SetCoordZ(float val) { m_pDGroupBox->SetCoordZ(val);}

	void SetTextSize (float val) { m_pDGroupBox->SetTextSize(val); }
	float GetTextSize (void) { return m_pDGroupBox->GetTextSize(); }
	// виртуальные функции, переопределяемые в классе
	virtual void OnDraw();
    virtual void SaveParameters(SimpleXML *pSimpleXML);
    virtual void LoadParameters(SimpleXML *pSimpleXML);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
};
//------------------------------------------------------------------------------