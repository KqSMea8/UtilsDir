//------------------------------------------------------------------------------
// Button.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include "C:\ZETTools\Interface2012\Custom\CustomButton.h"
//#include "C:\ZETTools\Interface2012\Custom\Font.h"
#include "C:\ZETTools\Interface2012\Draw\DButton.h"
//------------------------------------------------------------------------------
class CZButton : public CCustomButton
{
	DECLARE_DYNAMIC(CZButton)
public:
	CZButton(CCustomContainer *owner, CRect rect);
	CZButton(CCustomContainer *owner, CRect rect, CString ImageFile, int NumberTexture);
	virtual ~CZButton();
	CDButton *m_pDButton;

	eButtonType GetButtonType() { return m_pDButton->GetButtonType(); }
	void SetButtonType(eButtonType val) { m_pDButton->SetButtonType(val);}

	eButtonColoring GetButtonColoring() { return m_pDButton->GetButtonColoring(); }
	void SetButtonColoring(eButtonColoring val) { m_pDButton->SetButtonColoring(val); }

	eButtonAnimation GetButtonAnimation() { return m_pDButton->GetButtonAnimation(); }
	void SetButtonAnimation(eButtonAnimation val) { m_pDButton->SetButtonAnimation(val); }
	
	int GetBorderWidth() { return m_pDButton->GetBorderWidth(); }
	void SetBorderWidth(int val) {m_pDButton->SetBorderWidth(val);}

	COLORREF GetBorderColor() { return m_pDButton->GetBorderColor(); }
	void SetBorderColor(COLORREF val) {m_pDButton->SetBorderColor(val);}

	COLORREF GetBorderColorFocus() { return m_pDButton->GetBorderColorFocus(); }
	void SetBorderColorFocus(COLORREF val) {m_pDButton->SetBorderColorFocus(val);}

	CString GetCaption() { return m_pDButton->GetCaption();}
	void SetCaption(CString str){ m_pDButton->SetCaption(str);}
	
	CString GetCaptionChange() { return m_pDButton->GetCaptionChange();}
	void SetCaptionChange(CString str){ m_pDButton->SetCaptionChange(str);}

	void SetTextColor(int r, int g, int b) {m_pDButton->SetTextColor(r,g,b);}

	void SetSizeText (float val) { m_pDButton->SetSizeText(val); }
	float GetSizeText (void) { return m_pDButton->GetSizeText(); }

	void SetActivePos (bool val) { m_pDButton->SetActivePos(val); }

#ifdef _GDIPLUS
	CZFont* GetFont() { return m_pDButton->GetFont(); }
	void SetFont(CZFont *pFont) { m_pDButton->SetFont(pFont); }
#endif

	double GetTimeAnimation() { return m_pDButton->GetTimeAnimation();}
	void SetTimeAnimation(double val){m_pDButton->SetTimeAnimation(val);}

	bool GetAnimation()	 { return m_pDButton->GetAnimation(); }
	void SetAnimation(bool val) { m_pDButton->SetAnimation(val); }

	CString GetImageFileName() { return m_pDButton->GetImageFileName();}
	void SetImageFileName(CString str){m_pDButton->SetImageFileName(str);}

	void ReturnButtonToState (bool val) {m_pDButton->ReturnButtonToState(val);}

	bool GetButtonState (void) {return m_pDButton->GetButtonState();}
	void SetButtonState(bool val) {m_pDButton->SetButtonState(val);}

	bool GetButtonActive() {return m_pDButton->GetButtonActive();}
	void SetButtonActive(bool val) {m_pDButton->SetButtonActive(val);}

	bool GetDBelongsMenu() {return m_pDButton->DGetBelongsMenu();}
	void SetDBelongsMenu(bool val) {m_pDButton->DSetBelongsMenu(val);}

	bool GetIsClicked() {return m_pDButton->GetIsClicked();}
	void SetIsClicked(bool val) {m_pDButton->SetIsClicked(val);}
	
	void LiftButton(float val) {m_pDButton->LiftButton(val);}
	virtual void Clear(void);	

	void SetFixation();

	void SetRectForDraw(CRect val) {m_pDButton->SetRectForDraw(val);}

	void SetColor(GLfloat *ma, GLfloat *md,	
		GLfloat *me, 
		GLfloat *msp, GLfloat msh) {m_pDButton->SetColor(ma, md, me, msp, msh);}

	void SetAligmentHorizontal(StringAlignmentOGL val) { m_pDButton->SetAligmentHorizontal(val);}

};
//------------------------------------------------------------------------------