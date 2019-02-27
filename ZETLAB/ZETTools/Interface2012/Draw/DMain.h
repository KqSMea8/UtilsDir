//------------------------------------------------------------------------------
// DMain.h : файл заголовка
//------------------------------------------------------------------------------
// Класс отображения главного контейнера.
//------------------------------------------------------------------------------
#pragma once
//#include <Interface2012\Custom\Font.h>
//#include "resource.h"
#include <Interface2012\Draw\CustomDraw.h>
#define QNT_DETAILS 9
//------------------------------------------------------------------------------
struct ParamMain : public ParamCustomDraw
{// Структура параметров отображения
// В родительской структуре - общие параметры, здесь - частные параметры
	CString Title;			// заголовок программы
	int WidthBorder;		// ширина границы
	int SizeIcon;			// размер квадрата для иконки программы
	int WidthHMME;			// ширина всех системных кнопок
	POINT PointIcon;		// левая верхняя точка иконки программы
	COLORREF ColorBorderStart;
	COLORREF ColorBorderEnd;
	CString fileName;		// имя файла с картинкой
	bool MainWindow, WindowParam, WindowTree ;		//главное окно
    HWND hWnd;
	bool DrawIcon;
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
    GLfloat light_ambient[4]; 	//	{r, g, b, a}
    GLfloat light_diffuse[4];	//	{r, g, b, a}
    GLfloat light_specular[4];	//	{r, g, b, a}
    GLfloat light_position[4];	//	{x, y, z, w}
#endif
	ParamMain()
		: Title(L"")
		, ColorBorderStart(RGB(210, 210, 210))
		, ColorBorderEnd(RGB(90, 90, 90))
		, fileName("")
		, WidthHMME(0)
		, SizeIcon(16)
		, WidthBorder(0)	// после выполнения конструктора будет 4
		, MainWindow(true)
		, WindowParam(true)
		, WindowTree(false)
		, DrawIcon(false)
		, pFont(nullptr)
		, hWnd(NULL)
		, Number_Texture(0)
	{
		Size = sizeof(ParamMain);
		PointIcon = { 0 };


#ifdef _GDIPLUS
		GradientMode = LinearGradientModeForwardDiagonal;
#else
		mat_ambient[0] = 0.3;
		mat_ambient[1] = 0.3;
		mat_ambient[2] = 0.3;
		mat_ambient[3] = 0.0;

		mat_diffuse[0] = 1.0;
		mat_diffuse[1] = 1.0;
		mat_diffuse[2] = 1.0;
		mat_diffuse[3] = 1.0;

		mat_specular[0] = 1.0;
		mat_specular[1] = 1.0;
		mat_specular[2] = 1.0;
		mat_specular[3] = 1.0;

		mat_emission[0] = 0.1;
		mat_emission[1] = 0.1;
		mat_emission[2] = 0.1;
		mat_emission[3] = 0.0;

		mat_shininess = 10.0f;

        light_ambient[0] = 1.0;
        light_ambient[1] = 1.0;
        light_ambient[2] = 1.0;
        light_ambient[3] = 0.1;

        light_diffuse[0] = 1.0;
        light_diffuse[1] = 1.0;
        light_diffuse[2] = 1.0;
        light_diffuse[3] = 0.1;

        light_specular[0] = 1.0;
        light_specular[1] = 1.0;
        light_specular[2] = 1.0;
        light_specular[3] = 0.1;

        light_position[0] = -100.0f;
        light_position[1] = -100.0f;
        light_position[2] = 20.0f;
        light_position[3] = 0.0f;
		//	{x, y, z, w}
        //	w=0 - направленный источник света, w=1 - позиционный источник света (на него действует матрица вида)
#endif
	}
};
//------------------------------------------------------------------------------
class CDMain : public CCustomDraw
{
public:
#ifdef _GDIPLUS
	CDMain(CRect rect, PVOID ppGraphics,
		bool *pFlagIsChange, double *pTimeAnimation);
#else	
	CDMain(CRect rect,
				bool *pFlagIsChange, double *pTimeAnimation);
#endif
	virtual ~CDMain();
private:
	ParamMain m_param;	// структура всех параметров отображения
	CRect m_rect2;
	CRect rect2[20];
	int Bolder, N;
    int Depth;
	float coord[50][50][3];
#ifdef _GDIPLUS
	Gdiplus::Image *m_pImage;
	Gdiplus::GraphicsPath m_GraphPath;
#else
	CDetail *m_Main[QNT_DETAILS];
	CDetail *m_pMain;
	CDetail *m_Main_change;
	GLuint Texture;
	GLuint Icon;
	int PicSize[2];
	int PicSizeIcon[2];
    HGLRC hrc;
    HDC pDC;
#endif
protected:

public:

	void Resize(CRect rect);
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

	int GetWidthHMME() { return m_param.WidthHMME; }
	void SetWidthHMME(int val);

	POINT GetPointIcon() { return m_param.PointIcon; }

	COLORREF GetColorBorderStart() { return m_param.ColorBorderStart; }
	void SetColorBorderStart(COLORREF val);

	COLORREF GetColorBorderEnd() { return m_param.ColorBorderEnd; }
	void SetColorBorderEnd(COLORREF val);

	bool GetMainWindow() { return m_param.MainWindow; }
	void SetMainWindow(bool val);

	bool GetWindowTree() { return m_param.WindowTree; }
	void SetWindowTree(bool val);

	void LoadIcon (CString str);

    void SethWnd(HWND hWnd);

	void GetCurrentContext();
	void FreeCurrentContext();

	CString StringFromRes(UINT nID);

	// виртуальные функции, переопределяемые в классе
	virtual void OnDraw();
	virtual void Clear();
    virtual void SaveParameters(SimpleXML *pSimpleXML);
    virtual void LoadParameters(SimpleXML *pSimpleXML);
#ifdef _GDIPLUS
	LinearGradientMode GetGradientMode() { return m_param.GradientMode; }
	void SetGradientMode(LinearGradientMode val);
	CZFont* GetTitleFont() { return m_param.pFont; }
#else
	void SetRectMask(CRect rect_mask);
	CDFonts* GetTitleFont() { return m_param.pFont; }
	void SetColor(GLfloat *ma, GLfloat *md,	
		GLfloat *me, 
		GLfloat *msp, GLfloat msh);
    void InitOGL();
    BOOL bSetupPixelFormat();
    void Lightning(bool enable);
#endif
};
//------------------------------------------------------------------------------