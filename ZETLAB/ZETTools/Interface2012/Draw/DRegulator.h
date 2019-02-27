//------------------------------------------------------------------------------
// DRegulator.h : файл заголовка
//------------------------------------------------------------------------------
// Класс отображения простого слайдера.
// Анимация при клике в противоположном углу.
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Draw\CustomDraw.h"
#ifdef _GDIPLUS
#else
#define QNT_DETAILS 1
#endif
//------------------------------------------------------------------------------
struct ParamRegulator : public ParamCustomDraw
{// Структура параметров отображения служебных кнопок
	// В родительской структуре - общие параметры, здесь - частные параметры
	double dTimeAnimation;		// длительность анимации в сек
	bool m_bRegulatorType;
	float BorderWidth;
	bool variant;
	bool drawcaption;
	bool draw_scale;
	int quan_scale;
	bool center;
	float turn_coeff;
	float start_angle;
	bool MouseIsEnter;
	bool blocking;
	int Number_Texture; // номер текстуры в ZetIntSource
	GLuint Texture1;
	CString FileName;
#ifdef _GDIPLUS
#else
	GLfloat mat_ambient[4]; 	//	{r, g, b, a}
	GLfloat mat_diffuse[4];		//	{r, g, b, a}
	GLfloat mat_specular[4];	//	{r, g, b, a}
	GLfloat mat_emission[4];	//	{r, g, b, a}
	GLfloat mat_shininess;		//	коэффициент блика


	GLfloat mat_ambient_b[4]; 	//	{r, g, b, a}
	GLfloat mat_diffuse_b[4];		//	{r, g, b, a}
	GLfloat mat_specular_b[4];	//	{r, g, b, a}
	GLfloat mat_emission_b[4];	//	{r, g, b, a}
	GLfloat mat_shininess_b;		//	коэффициент блика
#endif
	ParamRegulator()
	{
		Texture1 = 0;
		MouseIsEnter = false;
		Size = sizeof(ParamRegulator);
		dTimeAnimation = 0.5;	// 500 мсек
		m_bRegulatorType = true;
		drawcaption = true;
		BorderWidth = 0.f;
		variant = false;
		draw_scale = false;
		center = false;
		blocking = false;
		start_angle = 0.f;
		turn_coeff = 0.f;
		quan_scale = 0;
		Number_Texture = 19;
		FileName = L"C:\\ZETTools\\Interface2012\\Textures\\Seamless\\Metal\\metellkrug.bmp";
#ifdef _GDIPLUS
#else
		mat_ambient[0] = 0.35;
		mat_ambient[1] = 0.35;
		mat_ambient[2] = 0.35;
		mat_ambient[3] = 0.0;

		mat_diffuse[0] = 1.0;
		mat_diffuse[1] = 1.0;
		mat_diffuse[2] = 1.0;
		mat_diffuse[3] = 0.0;

		mat_specular[0] = 1.0;
		mat_specular[1] = 1.0;
		mat_specular[2] = 1.0;
		mat_specular[3] = 0.0;

		mat_emission[0] = 0.05;
		mat_emission[1] = 0.05;
		mat_emission[2] = 0.05;
		mat_emission[3] = 0.0;

		mat_shininess = 15.24f;
#endif
	}
};
//------------------------------------------------------------------------------
class CDRegulator : public CCustomDraw
{
public:
#ifdef _GDIPLUS
	CDRegulator(CRect rect, PVOID ppGraphics,
		bool *pFlagIsChange, double *pTimeAnimation);
#else
	CDRegulator(CRect rect,	bool *pFlagIsChange, double *pTimeAnimation, bool variant, bool draw_scale, float start_angle);
	CDFonts* GetFont() { return font1; }
	void SetFont(CDFonts *pfont);
#endif
	virtual ~CDRegulator();
	virtual void Clear (void);
private:
	ParamRegulator m_param;	// структура всех параметров отображения
#ifdef _GDIPLUS
#else
	CDFonts *font1;
	CDetail *m_Regulator;
	float m_Angle_Cur, m_Angle_Prev, m_Angle_Rot, m_Angle_Rot_sum;
	float m_center_x, m_center_y;
	float sum;
	long lastpointx;
	GLuint Texture;
	int PicSize[2];
#endif
	double dTimeStart;			// время начала анимации	
	int m_nStart_x, m_nStart_y, m_nStart, m_lastx;
	bool m_bEvent;
	bool m_bFlag;
	GLUquadric *qobj;
	CRect rect_draw, rect1;
protected:

public:
	// функции параметров отображения

	CString GetCaption() { return m_param.Name; }
	void SetCaption(CString val) { m_param.Name = val; }

	double GetTimeAnimation() { return m_param.dTimeAnimation;}
	void SetTimeAnimation(double val);

	bool GetRegulatorType() { return m_param.m_bRegulatorType;}
	void SetRegulatorType(bool val);

	float GetBorderWidth() { return m_param.BorderWidth;}
	void SetBorderWidth(float val);

	void DrawCaption (bool var) { m_param.drawcaption = var; }

	void SetTurnCoeff (float turn_coeff);

    void SetImageFileName(CString str);

	void SetScale (int quan);
	int GetScale (void);

	void SetRegulatorValue (float angle);
	float GetRegulatorValue (void);

	void SetCenter (bool center) { m_param.center = center;}

	void SetBlocking (bool val) { m_param.blocking = val; }
	bool GetBlocking (void) { return m_param.blocking; }

	// виртуальные функции, переопределяемые в классе
	virtual void OnDraw();
    virtual void SaveParameters(SimpleXML *pSimpleXML);
    virtual void LoadParameters(SimpleXML *pSimpleXML);
	virtual void DoProcess();
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();
	virtual void OnLButtonClk(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnMouseWheel(UINT nFlags, CPoint point, short delta);
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point);
	// функции класса
	bool GetNeedEvent();
#ifdef _GDIPLUS
#else
	void SetColor(GLfloat *ma, GLfloat *md,	
		GLfloat *me, 
		GLfloat *msp, GLfloat msh);
	float GetAngle();
#endif
};
//------------------------------------------------------------------------------