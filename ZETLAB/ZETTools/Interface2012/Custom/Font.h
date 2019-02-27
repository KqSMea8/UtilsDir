//------------------------------------------------------------------------------
// Font.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include <Interface2012\Custom\DeleteObject.h>
//------------------------------------------------------------------------------
struct ParamFont
{
	float Size;					// его размер
	CString Name;				// имя шрифта
	COLORREF Color;				// цвет
	Gdiplus::FontStyle Style;	// его стиль
	ParamFont()
	{
		Size = 9.f;
		Name = L"Arial";
		Color = RGB(0, 0, 0);
		Style = Gdiplus::FontStyleRegular;		
		//FontStyleRegular    = 0,		стандартный
		//FontStyleBold       = 1,		жирный
		//FontStyleItalic     = 2,		курсив
		//FontStyleBoldItalic = 3,		жирный курсив
		//FontStyleUnderline  = 4,		подчеркнутый
		//FontStyleStrikeout  = 8		с линией посередине
	}
};
//------------------------------------------------------------------------------
class CZFont
{
private:
	Gdiplus::Font *m_pFont;
	ParamFont m_param;
	void Create();
	void Delete();

protected:
public:
	CZFont();
	CZFont(ParamFont paramFont);
	CZFont(CString name, float size,
				Gdiplus::FontStyle style = Gdiplus::FontStyleRegular,
				COLORREF color = RGB(0, 0, 0));
	virtual ~CZFont();

	float GetSize() { return m_param.Size; }
	void SetSize(float newVal);

	CString GetName() { return m_param.Name; }
	void SetName(CString newVal);

	COLORREF GetColor() { return m_param.Color; }
	void SetColor(COLORREF newVal) { m_param.Color = newVal; }

	Gdiplus::FontStyle GetStyle() { return m_param.Style; }
	void SetStyle(Gdiplus::FontStyle newVal);

	Gdiplus::Font* GetFont() { return m_pFont; }
};
//------------------------------------------------------------------------------