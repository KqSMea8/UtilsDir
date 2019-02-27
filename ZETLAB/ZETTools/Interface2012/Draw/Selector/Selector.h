#pragma once

#include <GdiPlus.h>
using namespace Gdiplus;
#pragma comment(lib,"GdiPlus.lib")

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>



#define LEFT_GAP		6	//Левая граница
#define RIGHT_GAP		5	//Правая граница 

#define DIG_RECT_OFFS_X 7
#define DIG_RECT_OFFS_Y	5

#define RECT_GAP		1 // расстояние между прямоугольниками

#define	MaxDivCount		10			//максимальное количество цифр

class Selector
{
public:

	Selector();
	Selector(int X, int Y, int Width, int Height);
	virtual	~Selector();
	void Init();

	void	DrawObject(Graphics &gr);

private:

	typedef		boost::shared_ptr<Gdiplus::Font> Font_ptr;
	typedef		boost::shared_ptr<LinearGradientBrush> LGB_ptr;
	typedef		boost::shared_ptr<SolidBrush> SB_ptr;
	typedef		boost::shared_ptr<Pen> Pen_ptr;
	typedef		boost::shared_ptr<Bitmap> Bitmap_ptr;
	typedef		boost::shared_ptr<Color> Color_ptr;

	Rect	m_rcBounds;
	Rect	m_digitRect;
	int		m_charWidth;
	int		m_signWidth;
	int		m_dotWidth;
	short	m_Sign;
	USHORT	m_digits[MaxDivCount];		//Массив цифр индикатора
	int		m_textTop;
	RectF	m_textRect;


	Bitmap_ptr		m_bitmap;

	Font_ptr		m_font;
	StringFormat	m_strFmt;

	LGB_ptr			m_digitsBr;
	GraphicsPath	*m_digitGp;

	SB_ptr			m_chLightBrush;

	LGB_ptr			m_backBr1;
	LGB_ptr			m_backBr2;
	Pen_ptr			m_backPen1;

	SolidBrush		*m_rectUnderDigitsBr;
	Pen				*m_lineDigitPen;

	Color			m_pthCentrColor;
	Color			m_pthSurrColor;
	int				m_countSurrColors;

	Color			m_3dFace;

	void InitDrawTools(Graphics &gr);
	void CreateBackBmp(Graphics &gr);
	
	long CalcRectX(const int &rectNumb);
	void DrawDigitRect(Graphics &graph, GraphicsPath *gp, const Rect &digitRect);
	void DrawText(Graphics &gr);
	void DrawRadiusRect(Graphics& graphics, Brush *brush, Pen* pen, float X, float Y, float width, float height, float radius, int cut);
	
	void CreateFont(Graphics &gr);
	void FittingFont(Graphics &gr);

	void FormIndicatorValue(void);
	void UnFormIndicatorValue(void);

	void IncVariable(short var);
	void DecVariable(short var);


	template <typename T>
	void CheckAndSet(const T &setVal, T &currVal);

	template <typename T>
	void CheckAndSet_Reset(const T &setVal, T &currVal);

	template <>
	void CheckAndSet_Reset <Color> (const Color &setVal, Color &currVal);

	short m_currentDigit;
	Color m_selectorColor;			//цвет селектора
	Color m_textColor;				//цвет текста
	Color m_selectedDigitColor;		//цвет вокруг выделенного числа
	Color_ptr m_pBackColor;				//цвет фона
	float m_maxValue;				//максимальное значение
	float m_minValue;				//минимальное значение
	float m_currentValue;			//текущее значение
	int m_countVisibleDigits;		//количество видимых знаков
	int m_countVisibleDivDigits;	//количество дробных знаков

public:

	void SetRect(const int &x, const int &y, const int &w, const int &h);
	void SetSelectorColor(const Color &selectorColor);
	void SetTextColor(const Color &textColor);
	void SetBackColor(const Color &backColor);
	void SetSelectedDigitColor(const Color &selectedDigitColor);
	void SetMaxValue(const float &maxVal);
	void SetMinValue(const float &minVal);
	void SetCurrentValue(const float &currVal);
	void SetCountVisibleDigits(const int &numDig);
	void SetCountVisibleDivDigits(const int &numDig);
	void SetCurrentDigit(const short &currDigit);


	Rect GetRect();
	Color GetSelectorColor();
	Color GetTextColor();
	Color* GetBackColor();
	Color GetSelectedDigitColor();
	float GetMaxValue();
	float GetMinValue();
	float GetCurrentValue();
	int GetCountVisibleDigits();
	int GetCountVisibleDivDigits();
	short GetCurrentDigit();

	void OnMouseWheel(short &zDelta, CPoint &clPoint);	// pt - поинт в клиентских координатах.
	void OnMouseMove(CPoint &point);	
	void OnLButtonDown(CPoint point);
	void OnKeyUp(UINT nChar);

	short GetPosFromPoint(CPoint pt);
};