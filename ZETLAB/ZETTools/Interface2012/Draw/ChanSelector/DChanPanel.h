//------------------------------------------------------------------------------
// DchanPanel.h : файл заголовка
//------------------------------------------------------------------------------
// Класс отображения ChanSelector.
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Draw\CustomDraw.h"
#include "C:\ZETTools\ZETView_basic_class\CDSRV.h"
#include "math.h"
#include <vector>

#define MAX_COMPUTERS					100
#define MAX_CHANNELS					271
#define MAX_DEVICES						100

//------------------------------------------------------------------------------
struct ParamChanPanel : public ParamCustomDraw
{// В родительской структуре - общие параметры, здесь - частные параметры
	int BorderWidth;			// ширина границы
	COLORREF BorderColor;		// цвет границы без фокуса
	COLORREF BorderColorFocus;	// цвет границы с фокусом
	COLORREF BackColor;			// цвет фона без мыши
	COLORREF BackColorMouse;	// цвет фона при наведении мыши
	COLORREF BackColorEnter;	// цвет фона при нажатой кнопке
	COLORREF CheckColorMouse;
	COLORREF CheckColor;
	CString Caption;			// текст, который выводится на кнопке
	CString CaptionChange;			// текст, который выводится на кнопке при нажатии для фиксированной кнопки
	CString fileName;		// базовое имя файлов с картинками
#ifdef _GDIPLUS
	CZFont *pFont;		// экземпляр класса шрифта
	StringAlignment AlignmentHorizontal;	// выравнивание по горизонтали
	StringAlignment AlignmentVertical;		// выравнивание по вертикали
	LinearGradientMode GradientMode;	// тип градиента
#else
	CDFonts *pFont;
	GLfloat mat_ambient[4]; 	//	{r, g, b, a}
	GLfloat mat_diffuse[4];		//	{r, g, b, a}
	GLfloat mat_specular[4];	//	{r, g, b, a}
	GLfloat mat_emission[4];	//	{r, g, b, a}
	GLfloat mat_shininess;		//	коэффициент блика
#endif
	double dTimeAnimation;	// длительность задержки кнопки
	double dTimeCur;		// текущая продолжительность анимации
	double dTimeStart;		// время начала анимации
	int SizeText;			// размер текстового окна (для CheckBox)
	Point pt[3];
	long CurChannel;
	ParamChanPanel()
	{
		Size = sizeof(ParamChanPanel);
		BorderWidth = 0;		// в конструкторе выставляется 1
		BorderColor = RGB(66, 66, 66);
		BorderColorFocus = RGB(66, 66, 66);	// 164, 210, 244;  150 210 255
		BackColor = RGB(240, 240, 240);
		BackColorMouse = RGB(223, 243, 255);
		BackColorEnter = RGB(240, 240, 240);
		//ButtonType = false;
		//CheckBox = false;
		SizeText = 60;
		Caption = L"";
		CaptionChange = L"";
		pFont = NULL;
		dTimeAnimation = 0.5;
		dTimeCur = 0.;
		dTimeStart = 0.;
#ifdef _GDIPLUS
		AlignmentHorizontal = StringAlignmentCenter;
		AlignmentVertical = StringAlignmentCenter;
		GradientMode = LinearGradientModeForwardDiagonal;
#else
		mat_ambient[0] = 0.1;
		mat_ambient[1] = 0.1;
		mat_ambient[2] = 0.1;
		mat_ambient[3] = 1.0;

		mat_diffuse[0] = 0.5;
		mat_diffuse[1] = 0.5;
		mat_diffuse[2] = 0.5;
		mat_diffuse[3] = 1.0;

		mat_specular[0] = 0.2;
		mat_specular[1] = 0.2;
		mat_specular[2] = 0.2;
		mat_specular[3] = 1.0;

		mat_emission[0] = 0.2;
		mat_emission[1] = 0.2;
		mat_emission[2] = 0.2;
		mat_emission[3] = 1.0;

		mat_shininess = 10.24f;
#endif
		fileName = L"";
		CurChannel = 0;
	}
};
//------------------------------------------------------------------------------
class CDChanPanel : public CCustomDraw
{
public:
#ifdef _GDIPLUS
	CDChanPanel(CRect rect, PVOID ppGraphics,
		bool *pFlagIsChange, double *pTimeAnimation);
#else
	CDChanPanel(CRect rect,
		bool *pFlagIsChange, double *pTimeAnimation);
#endif
	virtual ~CDChanPanel();

private:
	ParamChanPanel m_param;	// структура всех параметров отображения
	COLORREF m_BackColorCur;
	COLORREF m_BorderColorCur;
	CRect m_rect1;			// прямоугольник текстового поля
	bool m_bEvent;			// флаг события
	bool m_bFixation;		// флаг нажатия
	CString m_Text;
	double dTimeStart;			// время начала анимации
	int Width;
#ifdef _GDIPLUS
	CZFont *m_pFont;
	Image *m_pImage_Cur;	// ук. на текущую картинку
	Image *m_pImage_s;		// стандартное отображение
	Image *m_pImage_m;		// отображение с мышью
	Image *m_pImage_p;		// отображение в нажатом состоянии
	Image *m_pImage_pm;		// отображение с мышью для checkbox в нажатом состоянии
#else
	CDFonts *m_pFont;
	GLuint Texture;
	CDetail *m_ChSelector;
	int PicSize[2];
	CRect  rect_panel;
	CDSRV *m_psrv;
	int num_device;
	CString device[MAX_DEVICES];
#endif
protected:

public:
	// функции параметров отображения
	int GetBorderWidth() { return m_param.BorderWidth; }
	void SetBorderWidth(int val);

	COLORREF GetBorderColor() { return m_param.BorderColor; }
	void SetBorderColor(COLORREF val);

	COLORREF GetBorderColorFocus() { return m_param.BorderColorFocus; }
	void SetBorderColorFocus(COLORREF val);

	CString GetCaption() { return m_param.Caption; }
	void SetCaption(CString str);

	int GetSizeText() { return m_param.SizeText; }
	void SetSizeText(int val);

	CString GetCaptionChange() { return m_param.CaptionChange; }
	void SetCaptionChange(CString str);

	double GetTimeAnimation() { return m_param.dTimeAnimation;}
	void SetTimeAnimation(double val);

	bool GetAnimation()	 { return m_param.bAnimation; }
	void SetAnimation(bool val) { m_param.bAnimation = val; }

	CRect GetPanelRect() {return rect_panel;}
	void SetPanelRect(CRect rect);

	long GetCurChannel() {return m_param.CurChannel;}
	void SetCurChannel(long val);
#ifdef _GDIPLUS
	CZFont* GetFont() { return m_pFont; }
	void SetFont(CZFont *pFont) { m_pFont = pFont; }

	StringAlignment GetAlignmentHorizontal() {return m_param.AlignmentHorizontal; }
	void SetAlignmentHorizontal(StringAlignment val);

	StringAlignment GetAlignmentVertical() {return m_param.AlignmentVertical; }
	void SetAlignmentVertical(StringAlignment val);
#else
	CDFonts* GetFont() { return m_pFont; }
	void SetFont(CDFonts *pFont) { m_pFont = pFont; }
#endif
	bool GetFixsation() { return m_bFixation; }

	CString GetImageFileName() { return m_param.fileName;}
	void SetImageFileName(CString str);
	
	void ClickButton();
	void SetCDSRV(CDSRV *srv);

	// виртуальные функции, переопределяемые в классе
	virtual void OnDraw();
	virtual void GetFocus();
	virtual void KillFocus();
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();
	virtual void DoProcess();
	virtual void OnLButtonClk(UINT nFlags, CPoint point);


};
//------------------------------------------------------------------------------