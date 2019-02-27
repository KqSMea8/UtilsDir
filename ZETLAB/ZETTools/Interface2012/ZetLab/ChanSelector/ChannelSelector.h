//------------------------------------------------------------------------------
// ChanSelector.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "afxcmn.h"
#include "C:\ZETTools\Interface2012\Custom\CustomItem.h"
#include "C:\ZETTools\Interface2012\Draw\ChanSelector\DChannelSelector.h"
#include "C:\ZETTools\ZETView_basic_class\CDSRV.h"
#include "C:\ZETTools\Interface2012\ZetLab\ChanSelector\ChanPanel.h"
//------------------------------------------------------------------------------
class CChannelSelector : public CCustomItem
{
public:
	CChannelSelector(CCustomContainer *owner, CRect rect);
	virtual ~CChannelSelector();
	CChanPanel *m_pPanel;

private:
	CDChannelSelector *m_pDChanSelector;
	CDSRV *m_psrv;
	int quan;
	int StrHeight;

protected:

public:
	
	double GetTimeAnimation() { return m_pDChanSelector->GetTimeAnimation();}
	void SetTimeAnimation(double val){m_pDChanSelector->SetTimeAnimation(val);}

	bool GetAnimation()	 { return m_pDChanSelector->GetAnimation(); }
	void SetAnimation(bool val) { m_pDChanSelector->SetAnimation(val); }

	CString GetImageFileName() { return m_pDChanSelector->GetImageFileName();}
	void SetImageFileName(CString str){m_pDChanSelector->SetImageFileName(str);}

	int GetBorderWidth() { return m_pDChanSelector->GetBorderWidth(); }
	void SetBorderWidth(int val) { m_pDChanSelector->SetBorderWidth(val); }

	COLORREF GetBorderColor() { return m_pDChanSelector->GetBorderColor(); }
	void SetBorderColor(COLORREF val) { m_pDChanSelector->SetBorderColor(val); }

	COLORREF GetBorderColorFocus() { return m_pDChanSelector->GetBorderColorFocus(); }
	void SetBorderColorFocus(COLORREF val) { m_pDChanSelector->SetBorderColorFocus(val); }

	BOOL GetPositionValue() { return m_pDChanSelector->GetPositionValue();}
	void SetPositionValue(BOOL val);

	//COLORREF GetBackColor() { return m_pDChanSelector->GetBackColor(); }
	//void SetBackColor(COLORREF val) { m_pDChanSelector->SetBackColor(val); }

	//COLORREF GetBackColorMouse() { return m_pDChanSelector->GetBackColorMouse(); }
	//void SetBackColorMouse(COLORREF val) { m_pDChanSelector->SetBackColorMouse(val); }

	CString GetCaption() { return m_pDChanSelector->GetCaption(); }
	void SetCaption(CString str) { m_pDChanSelector->SetCaption(str); }

#ifdef _GDIPLUS
	CZFont* GetFont() { return m_pDChanSelector->GetFont(); }
	void SetFont(CZFont *pFont) { m_pDChanSelector->SetFont(pFont); }
#else
	CDFonts* GetFont() { return m_pDChanSelector->GetFont(); }
	void SetFont(CDFonts *pFont) { m_pDChanSelector->SetFont(pFont); }
#endif
	

	//CRect GetElementRect() {return m_pDChanSelector->GetElementRect();}
	//bool SetAutoTreePanel(CTreePanel *pTreePanel);
	// методы класса
	virtual void OnMouseLeave();
	virtual void OnLButtonClk(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	
	};
//------------------------------------------------------------------------------