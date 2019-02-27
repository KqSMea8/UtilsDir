//------------------------------------------------------------------------------
// DMainParam.h : файл заголовка
//------------------------------------------------------------------------------
// Класс отображения окна данных.
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Draw\CustomDraw.h"
#define QNT_DETAILS 6
//------------------------------------------------------------------------------
struct ParamMainParam : public ParamCustomDraw
{// Структура параметров отображения
	// В родительской структуре - общие параметры, здесь - частные параметры
	CString Title;			// заголовок программы
	int WidthBorder;		// ширина границы
	int SizeIcon;			// размер квадрата для иконки программыок
	POINT PointIcon;		// левая верхняя точка иконки программы
	COLORREF ColorBorderStart;
	COLORREF ColorBorderEnd;
	CString fileName;		// имя файла с картинкой
	int Number_Texture;
#ifdef _GDIPLUS
	CZFont *pFont;			// шрифт заголовка
	LinearGradientMode GradientMode;	// тип градиента
#else
	CDFonts *pFont;
	GLfloat mat_ambient[4]; 	//	{r, g, b, a}
	GLfloat mat_diffuse[4];		//	{r, g, b, a}
	GLfloat mat_specular[4];	//	{r, g, b, a}
	GLfloat mat_emission[4];	//	{r, g, b, a}
	GLfloat mat_shininess;		//	коэффициент блика
#endif
	ParamMainParam()
		: Title(L"")
	, ColorBorderStart(RGB(210, 210, 210))
	, ColorBorderEnd(RGB(90, 90, 90))
	, fileName(L"")
	, SizeIcon(16)
	, WidthBorder(0)	// после выполнения конструктора будет 4
	, pFont(nullptr)
	, Number_Texture(0)
	{
		Size = sizeof(ParamMainParam);
		PointIcon = { 0 };

#ifdef _GDIPLUS
		GradientMode = LinearGradientModeForwardDiagonal;
#else
		mat_ambient[0] = 0.2;
		mat_ambient[1] = 0.2;
		mat_ambient[2] = 0.2;
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
		mat_emission[3] = 1.0;

		mat_shininess = 10.24f;
#endif
	}
};
//------------------------------------------------------------------------------
class CDMainParam : public CCustomDraw
{
public:
#ifdef _GDIPLUS
	CDMainParam(CRect rect, PVOID ppGraphics,
		bool *pFlagIsChange, double *pTimeAnimation);
#else
	CDMainParam(CRect rect,
		bool *pFlagIsChange, double *pTimeAnimation, int height);
#endif
	virtual ~CDMainParam();
private:
	ParamMainParam m_param;	// структура всех параметров отображения
	CRect m_rect2;
	CRect m_rectForDraw;
	int size, Bolder, Depth;
#ifdef _GDIPLUS
	Gdiplus::Image *m_pImage;
	Gdiplus::GraphicsPath m_GraphPath;
#else
	CDetail *m_Main;
	CDetail *m_pMain[QNT_DETAILS];
	GLuint Texture;
	int PicSize[2];
#endif
protected:

public:
	HICON m_hIcon;
	// функции параметров отображения
	CRect GetRect2() { return m_rect2; }

	CString GetTitle() { return m_param.Title; }
	void SetTitle(CString str);

	CString GetImageFileName() { return m_param.fileName; }
	void SetImageFileName(CString str);

	int GetWidthBorder() { return m_param.WidthBorder; }
	void SetWidthBorder(int val);

	int GetSizeIcon() { return m_param.SizeIcon; }
	void SetSizeIcon(int val);

	POINT GetPointIcon() { return m_param.PointIcon; }

	COLORREF GetColorBorderStart() { return m_param.ColorBorderStart; }
	void SetColorBorderStart(COLORREF val);

	COLORREF GetColorBorderEnd() { return m_param.ColorBorderEnd; }
	void SetColorBorderEnd(COLORREF val);
	// виртуальные функции, переопределяемые в классе
	virtual void OnDraw();
	virtual void Clear (void);
    virtual void SaveParameters(SimpleXML *pSimpleXML);
    virtual void LoadParameters(SimpleXML *pSimpleXML);
#ifdef _GDIPLUS
	LinearGradientMode GetGradientMode() { return m_param.GradientMode; }
	void SetGradientMode(LinearGradientMode val);
	CZFont* GetTitleFont() { return m_param.pFont; }
#else
	CDFonts* GetTitleFont() { return m_param.pFont; }
	void OffDraw();
	void SetColor(GLfloat *ma, GLfloat *md,	
		GLfloat *me, 
		GLfloat *msp, GLfloat msh);
#endif
};
//------------------------------------------------------------------------------
