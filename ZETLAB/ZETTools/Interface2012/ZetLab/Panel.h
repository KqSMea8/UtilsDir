//------------------------------------------------------------------------------
// Panel.h : файл заголовка
//------------------------------------------------------------------------------
// Класс элемента Panel
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Custom\CustomContainer.h"
#include "C:\ZETTools\Interface2012\Draw\DPanel.h"
//------------------------------------------------------------------------------
class CPanel : public CCustomContainer
{
public:
	CPanel(CCustomContainer *owner, CRect rect);
	virtual ~CPanel();

private:
	CDPanel *m_pDPanel;

protected:

public:
	// функции параметров отображения
	int GetBorderHeight() { return m_pDPanel->GetBorderHeight(); }
	void SetBorderHeight(int val) { m_pDPanel->SetBorderHeight(val); }

	int GetBorderRadius() { return m_pDPanel->GetBorderRadius(); }
	void SetBorderRadius(int val) { m_pDPanel->SetBorderRadius(val); }

	PanelFillType GetPanelFillType() { return m_pDPanel->GetPanelFillType(); }
	void SetBorderFillType(PanelFillType val) { m_pDPanel->SetBorderFillType(val); }

	COLORREF GetBorderSolidColor() { return m_pDPanel->GetBorderSolidColor(); }
	void SetBorderSolidColor(COLORREF val) { m_pDPanel->SetBorderSolidColor(val); }

	COLORREF GetBorderGradientColorStart() { return m_pDPanel->GetBorderGradientColorStart(); }
	void SetBorderGradientColorStart(COLORREF val) { m_pDPanel->SetBorderGradientColorStart(val); }

	COLORREF GetBorderGradientColorEnd() { return m_pDPanel->GetBorderGradientColorEnd(); }
	void SetBorderGradientColorEnd(COLORREF val) { m_pDPanel->SetBorderGradientColorEnd(val); }

#ifdef _GDIPLUS
	LinearGradientMode GetBorderGradientMode() { return m_pDPanel->GetBorderGradientMode(); }
	void SetBorderGradientMode(LinearGradientMode val) { m_pDPanel->SetBorderGradientMode(val); }
#endif	
	PanelFillType GetFieldFillType() { return m_pDPanel->GetFieldFillType(); }
	void SetFieldFillType(PanelFillType val) { m_pDPanel->SetFieldFillType(val); }

	int GetFieldRadius() { return m_pDPanel->GetFieldRadius(); }
	void SetFieldRadius(int val) { m_pDPanel->SetFieldRadius(val); }

	COLORREF GetFieldSolidColor() { return m_pDPanel->GetFieldSolidColor(); }
	void SetFieldSolidColor(COLORREF val) { m_pDPanel->SetFieldSolidColor(val); }

	CString GetFieldFileNameTexture() { return m_pDPanel->GetFieldFileNameTexture(); }
	void SetFieldFileNameTexture(CString val) { m_pDPanel->SetFieldFileNameTexture(val); }

	// виртуальные функции, переопределяемые в классе
	virtual void OnDraw();
	virtual void Clear();

    virtual void SaveParameters(SimpleXML *pSimpleXML);
    virtual void LoadParameters(SimpleXML *pSimpleXML);
};
//------------------------------------------------------------------------------