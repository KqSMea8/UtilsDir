//------------------------------------------------------------------------------
// ComboBox.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Custom\CustomItem.h"
#include "C:\ZETTools\Interface2012\Draw\DComboBox.h"
#include "C:\ZETTools\Interface2012\ZetLab\MainParam.h"
#include "C:\ZETTools\Interface2012\Custom\Dialog_ZETi12.h"
#include "C:\ZETTools\Interface2012\Custom\Custom_ZETi12.h"
//------------------------------------------------------------------------------
class CZComboBox : public CCustomItem
{
public:
	CZComboBox(CCustomContainer *owner, CRect rect, CRect ownerRect);
	virtual ~CZComboBox();

private:
	CDComboBox *m_pDComboBox;

protected:

public:	

	CRect m_rect;
	bool m_pPress;

	int GetBorderWidth() { return m_pDComboBox->GetBorderWidth(); }
	void SetBorderWidth(int val) { m_pDComboBox->SetBorderWidth(val); }
	
	COLORREF GetBorderColor() { return m_pDComboBox->GetBorderColor(); }
	void SetBorderColor(COLORREF val) { m_pDComboBox->SetBorderColor(val); }

	COLORREF GetBorderColorFocus() { return m_pDComboBox->GetBorderColorFocus(); }
	void SetBorderColorFocus(COLORREF val) { m_pDComboBox->SetBorderColorFocus(val); }

	COLORREF GetBackColor() { return m_pDComboBox->GetBackColor(); }
	void SetBackColor(COLORREF val) { m_pDComboBox->SetBackColor(val); }

	COLORREF GetBackColorMouse() { return m_pDComboBox->GetBackColorMouse(); }
	void SetBackColorMouse(COLORREF val) { m_pDComboBox->SetBackColorMouse(val); }

	long GetIndexCur() { return m_pDComboBox->GetIndexCur(); }
	void SetIndexCur(long val) { m_pDComboBox->SetIndexCur(val); }

	CString GetCaption() { return m_pDComboBox->GetCaption(); }
	void SetCaption(CString str) { m_pDComboBox->SetCaption(str); }

	ComboBoxType GetType() { return m_pDComboBox->GetType(); }
	void SetType(ComboBoxType val) {m_pDComboBox->SetType(val);}

	void SetTextColor(int r, int g, int b) {m_pDComboBox->SetTextColor(r,g,b);}

#ifdef _GDIPLUS
	CZFont* GetFont() { return m_pDComboBox->GetFont(); }
	void SetFont(CZFont *pFont) { m_pDComboBox->SetFont(pFont); }
#endif

	// методы класса
	virtual void OnPickItem(long index);
	virtual void Clear();
	virtual void OnLButtonClk(UINT nFlags, CPoint point);
	virtual void OnMouseWheel(UINT nFlags, CPoint point, short delta);

	void SetTextString(CString str) { m_pDComboBox->SetTextString(str); }
	void EnableMenuItem(long index, bool bEnable)
	{ m_pDComboBox->EnableMenuItem(index, bEnable); }

	int GetAmountOfItems() { return m_pDComboBox->GetAmountOfItems(); }
	CString GetCaptionOfItem(int val) { return m_pDComboBox->GetCaptionOfItem(val);}

	void CompareStrings(CString val) { m_pDComboBox->CompareStrings(val); }
};
//------------------------------------------------------------------------------