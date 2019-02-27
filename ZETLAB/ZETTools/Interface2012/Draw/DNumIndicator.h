//------------------------------------------------------------------------------
// DNumIndicator.h : файл заголовка
//------------------------------------------------------------------------------
// Класс отображения индикатора уровня
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Draw\CustomDraw.h"
//------------------------------------------------------------------------------
struct ParamNumIndicator : public ParamCustomDraw
{// Структура параметров отображения служебных кнопок
	// В родительской структуре - общие параметры, здесь - частные параметры
	TypePosition PositionType;	// тип положения (гориз. или вертик.)
	long LinLog;				// линейный - 0, логарифмический - 1
	float Amplitude;			// значение от 0 до 1
	float ThresholdDanger;		// потор опасности
	int Distance;				// рассояние м/д индикатором и треугольником
	int BorderWidth;			// ширина границы
	int Precision;				// колличество отображаемых знаков после запятой
	int Integer_part;            // количество выводимых знаков целой части числа
	COLORREF BorderColor;		// цвет границы
	COLORREF BackColor;			// цвет фона
    COLORREF TextColor;         //цвет текста
	CString FileName;			// полное имя файла с картинкой треугольника
	double dTimeAnimation;		// время анимации
    char FontName[20];
	CString Text;	
	float TextSize;
	CString TextValue;
	long postcomma;            // количество цифр полсе запятой
	bool TypeView;
#ifdef _GDIPLUS
	CZFont *pFont;		// экземпляр класса шрифта
	StringAlignment AlignmentHorizontal;	// выравнивание по горизонтали
	StringAlignment AlignmentVertical;		// выравнивание по вертикали
	//TypeThreeDots ThreeDots;				// многоточие
#else
	StringAlignmentOGL AlignmentHorizontal;	// выравнивание по горизонтали
	StringAlignmentOGL AlignmentVertical;	// выравнивание по вертикали
	CDFonts *pFont;
#endif
	ParamNumIndicator()
	{
		Size = sizeof(ParamNumIndicator);
		LinLog = 1;
		TypeView = false;
		TextSize = 8.f;
		Amplitude = 0;
		ThresholdDanger = 0.93f;
		Distance = 5;
		BorderWidth = 2;
		BorderColor = RGB(82, 82, 82);
		BackColor = RGB(103, 103, 103);
        TextColor = RGB(40,90,40);
        sprintf(FontName, "Arialbd");
		FileName = L"";
		dTimeAnimation = 0.2;	// 200 мсек
		pFont = NULL;
		Text = L"";
		Precision = 7;
		Integer_part = 5;
		PositionType = tpVertical;
		postcomma = 0;
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
class CDNumIndicator : public CCustomDraw
{
public:
#ifdef _GDIPLUS
	CDNumIndicator(CRect rect, PVOID ppGraphics,
		bool *pFlagIsChange, double *pTimeAnimation);
#else
	CDNumIndicator(CRect rect,
		bool *pFlagIsChange, double *pTimeAnimation);
#endif
	virtual ~CDNumIndicator();

private:
	ParamNumIndicator m_param;	// структура всех параметров отображения
	COLORREF m_ColorCur;		// текущий цвет индикатора
	double m_dTimeStart;		// время начала анимации
	CRect m_rectT;				// прямоугольник треугольника
	CRect m_rect1;				// прямоугольник самого индикатора
	CRect m_rect2;				// прямоугольник цветового поля
	CRect m_rect3;
	int m_LengthRect2;			// максимальная длина цветового прямоугольника
	bool m_bDanger;				// значение превысило уровень опасности
	int m_SizeRectT;			// размер Rect треугольника
	void DefineRectAll();
	char vOut [20];
	bool flag;
protected:

public:
	// функции параметров отображения
	TypePosition GetPositionType() { return m_param.PositionType; }

	long GetLinLog() { return m_param.LinLog; }
	void SetLinLog(long val);

	float GetAmplitude() { return m_param.Amplitude; }
	void SetAmplitude(float val);

	float GetThresholdDanger() { return m_param.ThresholdDanger; }
	void SetThresholdDanger(float val);

	int GetDistance() { return m_param.Distance; }
	void SetDistance(int val);

	int GetBorderWidth() { return m_param.BorderWidth; }
	void SetBorderWidth(int val);

	COLORREF GetBorderColor() { return m_param.BorderColor; }
	void SetBorderColor(COLORREF val);

	COLORREF GetBackColor() { return m_param.BackColor; }
	void SetBackColor(COLORREF val);

	double GetTimeAnimation() { return m_param.dTimeAnimation; }
	void SetTimeAnimation(double val);

	long GetSizeString () { return m_param.postcomma; }
	void SetSizeString (long postcomma) { m_param.postcomma = postcomma; }

	CString GetText() { return m_param.Text; }
	void SetText(CString val);

	int GetPrecision() { return m_param.Precision; }
	void SetPrecision(int val);

	void SetIntegerPart (int val);
	int GetIntegerPart (void) {return m_param.Integer_part;}

	void SetTextColor(COLORREF colortext);
	COLORREF GetTextColor (void);

	void SetTextSize (float val);
	float GetTextSize (void);

	void SetTypeView (bool val) {m_param.TypeView = val; }
	bool GetTypeView (void) { return m_param.TypeView; }

	void SetTextValue (CString val);
	CString GetTextVal (void);

	void SetTextCeils (bool val);
	bool GetTextCeils (void);

#ifdef _GDIPLUS
#else
	CDFonts* GetFont() { return m_param.pFont; }
	void SetFont(CDFonts *pfont);
#endif

	// виртуальные функции, переопределяемые в классе
	virtual void OnDraw();
	virtual void DoProcess();
	virtual void OnLButtonClk(UINT nFlags, CPoint point);
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();
    virtual void SaveParameters(SimpleXML *pSimpleXML);
    virtual void LoadParameters(SimpleXML *pSimpleXML);
};
//------------------------------------------------------------------------------