//------------------------------------------------------------------------------
// DNumSelector.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Draw\CustomDraw.h"
//------------------------------------------------------------------------------
struct ParamNumSelector : public ParamCustomDraw
{// Структура параметров отображения служебных кнопок
	// В родительской структуре - общие параметры, здесь - частные параметры
	double dTimeAnimation;		// длительность анимации в сек
	CString Text;
	long CurValue;
	COLORREF BackColorEnter;
	COLORREF BackColorLeave;
	COLORREF BackColor;
	double dTimeCur;		// текущая продолжительность анимации
	double dTimeStart;		// время начала анимации
	long max_limit;
	long min_limit;
#ifdef _GDIPLUS
	CZFont *pFont;		// экземпляр класса шрифта
	StringAlignment AlignmentHorizontal;	// выравнивание по горизонтали
	StringAlignment AlignmentVertical;		// выравнивание по вертикали
	//TypeThreeDots ThreeDots;				// многоточие
#else
	StringAlignmentOGL AlignmentHorizontal;	// выравнивание по горизонтали
	StringAlignmentOGL AlignmentVertical;		// выравнивание по вертикали
	CDFonts *pFont;
	GLfloat mat_ambient[4]; 	//	{r, g, b, a}
	GLfloat mat_diffuse[4];		//	{r, g, b, a}
	GLfloat mat_specular[4];	//	{r, g, b, a}
	GLfloat mat_emission[4];	//	{r, g, b, a}
	GLfloat mat_shininess;		//	коэффициент блика
#endif
	ParamNumSelector()
	{
		Size = sizeof(ParamNumSelector);
		dTimeAnimation = 0.5;	// 500 мсек
		Text = L"";
		CurValue = 0;
		pFont = NULL;
		Text = L"";
		BackColor = RGB(100,100,100);
		dTimeCur = 0.;
		dTimeStart = 0.;
		max_limit = 0;
		min_limit = 0;
		BackColorEnter = RGB(0,0,0);
		BackColorLeave = RGB(0,0,0);
#ifdef _GDIPLUS
		AlignmentHorizontal = StringAlignmentCenter;
		AlignmentVertical = StringAlignmentCenter;
		//ThreeDots = no;
#else
		AlignmentHorizontal = StrAlignmentCenter;
		AlignmentVertical = StrAlignmentCenter;

		mat_ambient[0] = 0.2;
		mat_ambient[1] = 0.2;
		mat_ambient[2] = 0.2;
		mat_ambient[3] = 1.0;

		mat_diffuse[0] = 0.5;
		mat_diffuse[1] = 0.5;
		mat_diffuse[2] = 0.5;
		mat_diffuse[3] = 1.0;

		mat_specular[0] = 0.296648;
		mat_specular[1] = 0.296648;
		mat_specular[2] = 0.296648;
		mat_specular[3] = 1.0;

		mat_emission[0] = 0.2;
		mat_emission[1] = 0.2;
		mat_emission[2] = 0.2;
		mat_emission[3] = 1.0;

		mat_shininess = 10.24f;

		BackColorEnter = RGB(50, 50, 50);
		BackColorLeave = RGB(20, 20, 20);
#endif
	}
};
//------------------------------------------------------------------------------
class CDNumSelector : public CCustomDraw
{
public:
#ifdef _GDIPLUS
	CDNumSelector(CRect rect, PVOID ppGraphics,
		bool *pFlagIsChange, double *pTimeAnimation);
#else
	CDNumSelector(CRect rect,	bool *pFlagIsChange, double *pTimeAnimation);
#endif
	virtual ~CDNumSelector();
	virtual void Clear (void);
	bool flag;
private:
	ParamNumSelector m_param;	// структура всех параметров отображения
#ifdef _GDIPLUS
#else
	CDFonts *font1;
	CDetail *m_SNumSelector;
	float m_Angle_Cur[10], m_delta_Angle[10],m_Angle_Prev[10];
	int m_delta_value[10];
	GLuint Texture;
	int PicSize[2];
	int width;
#endif
	CRect rect_dot;
	int Num[2]; //количество цифр дробной и целой частей
	CRect rect_minus;
	double dTimeStart;			// время начала анимации	
	bool m_bEvent;
	bool m_bFlag, m_bUp, m_bDown;
	CRect rect_tr;
	CRect rect_i[10];
	char vOut [20];
	COLORREF BackColorCur;
	int BolderWidth;
	long PrevValue;
protected:

public:
	// функции параметров отображения

	double GetTimeAnimation() { return m_param.dTimeAnimation;}
	void SetTimeAnimation(double val);

	double GetCurValue() {return m_param.CurValue*pow(double(0.1), Num[1]);}
	void SetCurValue(double val);

	double GetMaxLimit() {return m_param.max_limit*pow(double(0.1), Num[1]);}
	void SetMaxLimit(double val);

	double GetMinLimit() {return m_param.min_limit*pow(double(0.1), Num[1]);}
	void SetMinLimit(double val);

	bool GetSign() {return flag;}
	void SetSign(bool val);

	int GetBolderWidth() { return BolderWidth;}
	void SetBolderWidth(int Width);

	int* GetMask() { return Num;}
	void SetMask(int integer, int fraction);

	void SetAngles(double val);
	// виртуальные функции, переопределяемые в классе
	virtual void OnDraw();
	virtual void DoProcess();
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnLButtonClk(UINT nFlags, CPoint point);
	virtual void OnMouseWheel(UINT nFlags, CPoint point, short delta);
	void Init();
	// функции класса
	void DefineRectClick();
#ifdef _GDIPLUS
#else
	void SetColor(GLfloat *ma, GLfloat *md,	
		GLfloat *me, 
		GLfloat *msp, GLfloat msh);
#endif
};
//------------------------------------------------------------------------------