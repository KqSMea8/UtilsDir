//------------------------------------------------------------------------------
// StaticText.h : файл заголовка
//------------------------------------------------------------------------------
// Класс элемента "Статический текст"
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Custom\CustomElement.h"
#include "C:\ZETTools\Interface2012\Draw\DStaticText.h"
//------------------------------------------------------------------------------
class CStaticText : public CCustomElement
{
public:
	CStaticText(CCustomContainer *owner, CRect rect);
	virtual ~CStaticText();

private:
	CDStaticText *m_pDText;

protected:

public:
	// функции параметров отображения
	CString GetText() { return m_pDText->GetText(); }
	void SetText(CString val) { m_pDText->SetText(val); }

#ifdef _GDIPLUS
	CZFont* GetFont() { return m_pDText->GetFont(); }
	void SetFont(CZFont *pfont) { m_pDText->SetFont(pfont); }

	StringAlignment GetAlignmentHorizontal() {
		return m_pDText->GetAlignmentHorizontal(); }
	void SetAlignmentHorizontal(StringAlignment val) {
		m_pDText->SetAlignmentHorizontal(val); }

	StringAlignment GetAlignmentVertical() {
		return m_pDText->GetAlignmentVertical(); }
	void SetAlignmentVertical(StringAlignment val) {
		m_pDText->SetAlignmentVertical(val); }
#else
	CDFonts* GetFont() { return m_pDText->GetFont(); }
	void SetFont(CDFonts *pfont) { m_pDText->SetFont(pfont); }

	StringAlignmentOGL GetAlignmentHorizontal() {
		return m_pDText->GetAlignmentHorizontal(); }
	void SetAlignmentHorizontal(StringAlignmentOGL val) {
		m_pDText->SetAlignmentHorizontal(val); }

	StringAlignmentOGL GetAlignmentVertical() {
		return m_pDText->GetAlignmentVertical(); }
	void SetAlignmentVertical(StringAlignmentOGL val) {
		m_pDText->SetAlignmentVertical(val); }

	void SetTextSize (float val) {m_pDText->SetTextSize (val); }
	float GetTextSize (void) {return m_pDText->GetTextSize(); }

	void SetTextColor (COLORREF val) {m_pDText->SetTextColor(val); }
	COLORREF GetTextColor () {m_pDText->GetTextColor(); }

	virtual void Clear (void) { m_pDText->Clear(); }
#endif
	//TypeThreeDots GetThreeDots() { return m_pDText->GetThreeDots(); }
	//void SetThreeDots(TypeThreeDots val) { m_pDText->SetThreeDots(val); }

	// виртуальные функции, переопределяемые в классе
    virtual void SaveParameters(SimpleXML *pSimpleXML);
    virtual void LoadParameters(SimpleXML *pSimpleXML);
};
//------------------------------------------------------------------------------