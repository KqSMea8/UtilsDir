//------------------------------------------------------------------------------
// DSimpleSlider.h : файл заголовка
//------------------------------------------------------------------------------
// Класс отображения слайдера.
// Анимация при клике в любой точке.
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Draw\CustomDraw.h"
#ifdef _GDIPLUS
#else
#include "C:\ZETTools\Interface2012\Custom\STL\Detail.h"
#include "C:\ZETTools\Interface2012\Custom\STL\STLRead.h"
#define QNT_DETAILS 4
#endif
//------------------------------------------------------------------------------
struct ParamSlider : public ParamCustomDraw
{// Структура параметров отображения служебных кнопок
	// В родительской структуре - общие параметры, здесь - частные параметры
	TypePosition PositionType;	// тип положения (гориз. или вертик.)
	float PositionValue;			// значение курсора
	float CurrentPosition;			//позиция курсора в пикселях
	int deltaX;
	int deltaY;
	CString FileNameField;		// полное имя файла с картинкой поля
	CString FileNameCursor;		// полное имя файла с картинкой курсора
	double dTimeAnimation;		// длительность анимации в сек
	float fValueCur;			// нормированное текущее положение курсора
    double Rotate;
#ifdef _GDIPLUS
#else
	GLfloat mat_ambient[4]; 	//	{r, g, b, a}
	GLfloat mat_diffuse[4];		//	{r, g, b, a}
	GLfloat mat_specular[4];	//	{r, g, b, a}
	GLfloat mat_emission[4];	//	{r, g, b, a}
	GLfloat mat_shininess;		//	коэффициент блика
#endif
	ParamSlider()
	{
		Size = sizeof(ParamSlider);
		PositionType = tpHorizontal;
		PositionValue = 0;
		CurrentPosition =0;
		deltaX = 3;
		deltaY = 1;
		FileNameField = L"";
		FileNameCursor = L"";
		dTimeAnimation = 0.5;	// 500 мсек
		fValueCur = 0.f;
        Rotate = 0;
#ifdef _GDIPLUS
#else
        mat_ambient[0] = 0.1;
        mat_ambient[1] = 0.1;
        mat_ambient[2] = 0.1;
        mat_ambient[3] = 1.0;

        mat_diffuse[0] = 1.0;
        mat_diffuse[1] = 1.0;
        mat_diffuse[2] = 1.0;
        mat_diffuse[3] = 1.0;

        mat_specular[0] = 1.0;
        mat_specular[1] = 1.0;
        mat_specular[2] = 1.0;
        mat_specular[3] = 1.0;

        mat_emission[0] = 0.2;
        mat_emission[1] = 0.2;
        mat_emission[2] = 0.2;
        mat_emission[3] = 0.0;

        mat_shininess = 100.24f;
#endif
	}
};
//------------------------------------------------------------------------------
class CDSlider : public CCustomDraw
{
public:
#ifdef _GDIPLUS
	CDSimpleSlider(CRect rect, PVOID ppGraphics,
		bool *pFlagIsChange, double *pTimeAnimation);
#else
	CDSlider(CRect rect,	bool *pFlagIsChange, double *pTimeAnimation);
#endif
	virtual ~CDSlider();

private:
	ParamSlider m_param;	// структура всех параметров отображения
#ifdef _GDIPLUS
	Image *m_pImage_Field;		// картинка поля слайдера
	Image *m_pImage_Cursor;		// просто указатель на картинку текущего курсора
	Image *m_pImage_Cursor_s;	// картинка курсора слайдера, стандарт
	Image *m_pImage_Cursor_m;	// картинка курсора слайдера, с мышью
	Image *m_pImage_Cursor_p;	// картинка курсора слайдера, "взятый" мышью
#else
	CDetail Slider[QNT_DETAILS];
    GLuint Texture, Texture1;
    int PicSize[2], PicSize1[2];
#endif
	double dTimeStart;			// время начала анимации
	CRect m_rect0;				// курсор в положении 0
	CRect m_rectC;				// курсор в текущем положении
	CRect m_rect1;				// курсор в положении 1
	int m_LengthValue;			// длина свободного хода курсора
	int m_SizeCursor;	
	double m_nStart;
	bool m_bEvent;
	bool m_bFlag;

	void DefineRectAll();
	void DefineRectCur();

protected:

public:
	// функции параметров отображения
	TypePosition GetPositionType() { return m_param.PositionType;}
	//	void SetPositionType(TypePosition val);	только для чтения

	float GetPositionValue() { return m_param.PositionValue;}
	void SetPositionValue(float val);

	int GetDeltaX() { return m_param.deltaX;}
	void SetDeltaX(int val);

	int GetDeltaY() { return m_param.deltaY;}
	void SetDeltaY(int val);

	CString GetFileNameField() { return m_param.FileNameField;}
	void SetFileNameField(CString str, double angle);

	CString GetFileNameCursor() { return m_param.FileNameCursor;}
	void SetFileNameCursor(CString str, double angle);

	double GetTimeAnimation() { return m_param.dTimeAnimation;}
	void SetTimeAnimation(double val);

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

	// функции класса
	bool GetNeedEvent();
#ifdef _GDIPLUS
#else
	void SetColor(GLfloat *ma, GLfloat *md,	
		GLfloat *me, 
		GLfloat *msp, GLfloat msh);
#endif
};
//------------------------------------------------------------------------------