//------------------------------------------------------------------------------
// DStaticText.h : файл заголовка
//------------------------------------------------------------------------------
// Класс отображения статического текста
//------------------------------------------------------------------------------
#pragma once
#ifdef _GDIPLUS
#include "C:\ZETTools\Interface2012\Custom\Font.h"
#endif
#include "C:\ZETTools\Interface2012\Draw\CustomDraw.h"
//------------------------------------------------------------------------------
//enum TypeThreeDots
//{// тип многоточия в тексте
//	no,
//	left,
//	center,
//	right
//};
//------------------------------------------------------------------------------
struct ParamStaticText : public ParamCustomDraw
{// Структура параметров отображения служебных кнопок
// В родительской структуре - общие параметры, здесь - частные параметры
	CString Text;			// текст
//	CString TextDraw;		// отображаемый текст
#ifdef _GDIPLUS
	CZFont *pFont;		// экземпляр класса шрифта
	StringAlignment AlignmentHorizontal;	// выравнивание по горизонтали
	StringAlignment AlignmentVertical;		// выравнивание по вертикали
	//TypeThreeDots ThreeDots;				// многоточие
#else
	StringAlignmentOGL AlignmentHorizontal;	// выравнивание по горизонтали
	StringAlignmentOGL AlignmentVertical;		// выравнивание по вертикали
	CDFonts *pFont;
    COLORREF TextColor;
	float TextSize;
    char FontName[20];
#endif
	ParamStaticText()
	{
		Size = sizeof(ParamStaticText);
		Text = L"";
		TextColor = RGB( 255, 255, 255);
//		TextDraw = L"";
		pFont = NULL;
		sprintf(FontName, "Arialbd");
		TextSize = 8.f;
		
#ifdef _GDIPLUS
		AlignmentHorizontal = StringAlignmentCenter;
		AlignmentVertical = StringAlignmentCenter;
		//ThreeDots = no;
#else
		AlignmentHorizontal = StrAlignmentCenter;
		AlignmentVertical = StrAlignmentCenter;
#endif
	}
};
//------------------------------------------------------------------------------
class CDStaticText : public CCustomDraw
{
public:
#ifdef _GDIPLUS
	CDStaticText(CRect rect, PVOID ppGraphics,
				bool *pFlagIsChange, double *pTimeAnimation);
#else
	CDStaticText(CRect rect,
		bool *pFlagIsChange, double *pTimeAnimation);
#endif
	virtual ~CDStaticText();

private:
	ParamStaticText m_param;// структура всех параметров отображения

protected:

public:
	// функции параметров отображения
	CString GetText() { return m_param.Text; }
	void SetText(CString val);
#ifdef _GDIPLUS
	CZFont* GetFont() { return m_param.pFont; }
	void SetFont(CZFont *pfont);

	StringAlignment GetAlignmentHorizontal() {
		return m_param.AlignmentHorizontal; }
	void SetAlignmentHorizontal(StringAlignment val);

	StringAlignment GetAlignmentVertical() {
		return m_param.AlignmentVertical; }
	void SetAlignmentVertical(StringAlignment val);
#else
	CDFonts* GetFont() { return m_param.pFont; }
	void SetFont(CDFonts *pfont);

	StringAlignmentOGL GetAlignmentHorizontal() {
		return m_param.AlignmentHorizontal; }
	void SetAlignmentHorizontal(StringAlignmentOGL val);

	StringAlignmentOGL GetAlignmentVertical() {
		return m_param.AlignmentVertical; }
	void SetAlignmentVertical(StringAlignmentOGL val);

	void SetTextSize (float val);
	float GetTextSize (void);

	void SetTextColor (COLORREF val);
	COLORREF GetTextColor (void);
#endif
	//TypeThreeDots GetThreeDots() { return m_param.ThreeDots; }
	//void SetThreeDots(TypeThreeDots val);

	// виртуальные функции, переопределяемые в классе
	virtual void OnDraw();
	virtual void Clear (void);
    virtual void SaveParameters(SimpleXML *pSimpleXML);
    virtual void LoadParameters(SimpleXML *pSimpleXML);
};
//------------------------------------------------------------------------------