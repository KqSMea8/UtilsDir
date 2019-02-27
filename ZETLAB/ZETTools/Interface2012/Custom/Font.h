//------------------------------------------------------------------------------
// Font.h : ���� ���������
//------------------------------------------------------------------------------
#pragma once
#include <Interface2012\Custom\DeleteObject.h>
//------------------------------------------------------------------------------
struct ParamFont
{
	float Size;					// ��� ������
	CString Name;				// ��� ������
	COLORREF Color;				// ����
	Gdiplus::FontStyle Style;	// ��� �����
	ParamFont()
	{
		Size = 9.f;
		Name = L"Arial";
		Color = RGB(0, 0, 0);
		Style = Gdiplus::FontStyleRegular;		
		//FontStyleRegular    = 0,		�����������
		//FontStyleBold       = 1,		������
		//FontStyleItalic     = 2,		������
		//FontStyleBoldItalic = 3,		������ ������
		//FontStyleUnderline  = 4,		������������
		//FontStyleStrikeout  = 8		� ������ ����������
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