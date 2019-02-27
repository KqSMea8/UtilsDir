//------------------------------------------------------------------------------
// DIndicatorLevel.h : файл заголовка
//------------------------------------------------------------------------------
// Класс отображения индикатора уровня
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Draw\CustomDraw.h"
//------------------------------------------------------------------------------
struct ParamIndicatorLevel : public ParamCustomDraw
{// Структура параметров отображения служебных кнопок
// В родительской структуре - общие параметры, здесь - частные параметры
	TypePosition PositionType;	// тип положения (гориз. или вертик.)
	long LinLog;				// линейный - 0, логарифмический - 1
	float Amplitude;			// значение от 0 до 1
	float ThresholdDanger;		// потор опасности
	int Distance;				// рассояние м/д индикатором и треугольником
	int BorderWidth;			// ширина границы
	COLORREF BorderColor;		// цвет границы
	COLORREF BackColor;			// цвет фона
	CString FileName;			// полное имя файла с картинкой треугольника
	double dTimeAnimation;		// время анимации

	ParamIndicatorLevel()
	{
		Size = sizeof(ParamIndicatorLevel);
		LinLog = 1;
		Amplitude = 0;
		ThresholdDanger = 0.93f;
		Distance = 5;
		BorderWidth = 2;
		BorderColor = RGB(82, 82, 82);
		BackColor = RGB(103, 103, 103);
		FileName = L"";
		dTimeAnimation = 0.2;	// 200 мсек
	}
};
//------------------------------------------------------------------------------
class CDIndicatorLevel : public CCustomDraw
{
public:
	CDIndicatorLevel(CRect rect, PVOID ppGraphics,
				bool *pFlagIsChange, double *pTimeAnimation);
	virtual ~CDIndicatorLevel();

private:
	ParamIndicatorLevel m_param;// структура всех параметров отображения
	COLORREF m_ColorCur;		// текущий цвет индикатора
	Image *m_pImage;			// указатель на текущую картинку треугольника
	Image *m_pImage_s;			// картинка треугольника, стандарт
	Image *m_pImage_d;			// картинка треугольника желтого
	Image *m_pImage_p;			// картинка треугольника только, что кликнутого
	double m_dTimeStart;		// время начала анимации
	CRect m_rectT;				// прямоугольник треугольника
	CRect m_rect1;				// прямоугольник самого индикатора
	CRect m_rect2;				// прямоугольник цветового поля
	int m_LengthRect2;			// максимальная длина цветового прямоугольника
	bool m_bDanger;				// значение превысило уровень опасности
	int m_SizeRectT;			// размер Rect треугольника
	void DefineRectAll();

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

	CString GetFileNameTriangle() { return m_param.FileName; }
	void SetFileNameTriangle(CString str);

	double GetTimeAnimation() { return m_param.dTimeAnimation; }
	void SetTimeAnimation(double val);

	// виртуальные функции, переопределяемые в классе
	virtual void OnDraw();
	virtual void DoProcess();
	virtual void OnLButtonClk(UINT nFlags, CPoint point);
};
//------------------------------------------------------------------------------