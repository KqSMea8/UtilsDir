//------------------------------------------------------------------------------
// ChanSelector.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "afxcmn.h"
#include "C:\ZETTools\Interface2012\Custom\CustomButton.h"
#include "C:\ZETTools\Interface2012\Draw\ChanSelector\DChanPanel.h"
#include "C:\ZETTools\ZETView_basic_class\CDSRV.h"
//------------------------------------------------------------------------------
class CChanPanel : public CCustomButton
{
public:
	CChanPanel(CCustomContainer *owner, CRect rect);
	virtual ~CChanPanel();
	CRect m_rect;
	

private:
	CDChanPanel *m_pDChanPanel;
protected:

public:
	
	double GetTimeAnimation() { return m_pDChanPanel->GetTimeAnimation();}
	void SetTimeAnimation(double val){m_pDChanPanel->SetTimeAnimation(val);}

	bool GetAnimation()	 { return m_pDChanPanel->GetAnimation(); }
	void SetAnimation(bool val) { m_pDChanPanel->SetAnimation(val); }

	CString GetImageFileName() { return m_pDChanPanel->GetImageFileName();}
	void SetImageFileName(CString str){m_pDChanPanel->SetImageFileName(str);}

	int GetBorderWidth() { return m_pDChanPanel->GetBorderWidth(); }
	void SetBorderWidth(int val) { m_pDChanPanel->SetBorderWidth(val); }

	COLORREF GetBorderColor() { return m_pDChanPanel->GetBorderColor(); }
	void SetBorderColor(COLORREF val) { m_pDChanPanel->SetBorderColor(val); }

	COLORREF GetBorderColorFocus() { return m_pDChanPanel->GetBorderColorFocus(); }
	void SetBorderColorFocus(COLORREF val) { m_pDChanPanel->SetBorderColorFocus(val); }

	//COLORREF GetBackColor() { return m_pDChanPanel->GetBackColor(); }
	//void SetBackColor(COLORREF val) { m_pDChanPanel->SetBackColor(val); }

	//COLORREF GetBackColorMouse() { return m_pDChanPanel->GetBackColorMouse(); }
	//void SetBackColorMouse(COLORREF val) { m_pDChanPanel->SetBackColorMouse(val); }

	CString GetCaption() { return m_pDChanPanel->GetCaption(); }
	void SetCaption(CString str) { m_pDChanPanel->SetCaption(str); }

	CRect GetPanelRect() { return m_pDChanPanel->GetPanelRect(); }
	void SetPanelRect(CRect rect) { m_pDChanPanel->SetPanelRect(rect); }

	void ClickButton() {m_pDChanPanel->ClickButton();} 
	void SetCDSRV(CDSRV *m_psrv) {m_pDChanPanel->SetCDSRV(m_psrv);}
#ifdef _GDIPLUS
	CZFont* GetFont() { return m_pDChanPanel->GetFont(); }
	void SetFont(CZFont *pFont) { m_pDChanPanel->SetFont(pFont); }
#else
	CDFonts* GetFont() { return m_pDChanPanel->GetFont(); }
	void SetFont(CDFonts *pFont) { m_pDChanPanel->SetFont(pFont); }
#endif
	

	//CRect GetElementRect() {return m_pDChanPanel->GetElementRect();}
	//bool SetAutoTreePanel(CTreePanel *pTreePanel);
	// методы класса
	//virtual void OnMouseWheel(UINT nFlags, CPoint point, short delta);
	//virtual void OnLButtonClk(UINT nFlags, CPoint point);
	//virtual void OnPickItem(long index);
	
	};
//------------------------------------------------------------------------------