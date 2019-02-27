#pragma once

/*
Данный класс является производным от CStatic, наследует все доступные его свойства,
плюс к ним, в классе реализована поддержка следующий функций
 -  SetText(CString str), задание строки выводимого текста
 -  SetColorBack(COLORREF backColor), установка цвета заднего фона
 -  SetColorText(COLORREF color), установка цвета строки
 -  ReconstructFont(CFont* font), выбор шрифта
 -  возможность вывода текста на прозрачном фоне
 преимущества выбора CStatic с применением выбранных функций заключается еще и в том, что
 если его растянуть в соответствии с выбранным размером, например на какой-нибудь ActiveX,
 то перенос слов организуется автоматически.
*/
class CLabel : public CStatic
{
	DECLARE_DYNAMIC(CLabel)

public:
	CLabel();
	virtual ~CLabel();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnMove(int x, int y);

public:
	//установка цвета текста
	void SetColorText(COLORREF color);
	COLORREF m_crText;

	//установка цвета заднего фона
	void SetColorBack(COLORREF backColor);
	COLORREF m_backColor;

	//задание строки выводимого текста
	void SetText(CString str);
	CString text;

	//возможность вывода текста на прозрачном фоне
	void DrawTransparentBitmap(HDC hdc, HDC hdc_, CRect rect, COLORREF cTransparentColor);
	void SetTransparent(bool isTransp);
	bool m_bTransparent;

	//выбор шрифта
	void ReconstructFont(CFont* font);
	CFont* m_font;
	LOGFONT	m_lf;

	void UpdateSurface();
};


