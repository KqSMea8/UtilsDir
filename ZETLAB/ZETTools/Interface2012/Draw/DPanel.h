//------------------------------------------------------------------------------
// DPanel.h : файл заголовка
//------------------------------------------------------------------------------
// Класс отображения элемента Panel
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Draw\CustomDraw.h"
#define DETAILS 9
//------------------------------------------------------------------------------
enum PanelFillType
{
	pftSolid,
	pftGradient,
	pftTexture
};
//------------------------------------------------------------------------------
struct ParamPanel : public ParamCustomDraw
{// Структура параметров отображения служебных кнопок
// В родительской структуре - общие параметры, здесь - частные параметры
	// параметры границы -------------------------------
	int BorderHeight;
	int BorderRadius;
	PanelFillType BorderFillType;
	COLORREF BorderSolidColor;
	COLORREF BorderGradientColorStart;
	COLORREF BorderGradientColorEnd;
	int Number_Texture;
	CString FileName;
#ifdef _GDIPLUS
	LinearGradientMode BorderGradientMode;
#else
    GLfloat mat_ambient[4]; 	//	{r, g, b, a}
    GLfloat mat_diffuse[4];		//	{r, g, b, a}
    GLfloat mat_specular[4];	//	{r, g, b, a}
    GLfloat mat_emission[4];	//	{r, g, b, a}
    GLfloat mat_shininess;		//	коэффициент блика
#endif
	// параметры поля ----------------------------------
	PanelFillType FieldFillType;
	int FieldRadius;
	COLORREF FieldSolidColor;
	CRect FieldRect;
	float CoordZ;
	ParamPanel()
	{
		Size = sizeof(ParamPanel);
		BorderHeight = 4;
		BorderRadius = 4;
		BorderFillType = pftGradient;
		BorderSolidColor = RGB(90, 90, 90);
		BorderGradientColorStart = RGB(180, 180, 180);
		BorderGradientColorEnd = RGB(90, 90, 90);
		FieldRect = CRect(0, 0, 0, 0);
		CoordZ = 0.1;
		Number_Texture = 0;
		FileName = L"C:\\ZETTools\\Interface2012\\Textures\\Patterns\\metal_mart14.bmp";
#ifdef _GDIPLUS
		BorderGradientMode = LinearGradientModeForwardDiagonal;
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
        mat_emission[3] = 0.0;

        mat_shininess = 100.24f;
#endif

		FieldFillType = pftSolid;
		FieldRadius = 4;
		FieldSolidColor = RGB(60, 60, 60);
	}
};
//------------------------------------------------------------------------------
class CDPanel : public CCustomDraw
{
public:
#ifdef _GDIPLUS
	CDPanel(CRect rect, PVOID ppGraphics,
				bool *pFlagIsChange, double *pTimeAnimation);
#else
	CDPanel(CRect rect, bool *pFlagIsChange, 
		double *pTimeAnimation);
#endif
	virtual ~CDPanel();

private:
	ParamPanel m_param;	// структура всех параметров отображения
#ifdef _GDIPLUS
	Image *m_pImage;
	GraphicsPath m_GraphPath;
#else
	GLuint Texture;
	CDetail *m_Panel[DETAILS];
	int PicSize[2];
	int size, Depth;
#endif
protected:

public:
	// функции параметров отображения
	int GetBorderHeight() { return m_param.BorderHeight; }
	void SetBorderHeight(int val);

	int GetBorderRadius() { return m_param.BorderRadius; }
	void SetBorderRadius(int val);

	PanelFillType GetPanelFillType() { return m_param.BorderFillType; }
	void SetBorderFillType(PanelFillType val);

	COLORREF GetBorderSolidColor() { return m_param.BorderSolidColor; }
	void SetBorderSolidColor(COLORREF val);

	COLORREF GetBorderGradientColorStart() { return m_param.BorderGradientColorStart; }
	void SetBorderGradientColorStart(COLORREF val);

	COLORREF GetBorderGradientColorEnd() { return m_param.BorderGradientColorEnd; }
	void SetBorderGradientColorEnd(COLORREF val);

#ifdef _GDIPLUS
	LinearGradientMode GetBorderGradientMode() { return m_param.BorderGradientMode; }
	void SetBorderGradientMode(LinearGradientMode val);
#endif

	PanelFillType GetFieldFillType() { return m_param.FieldFillType; }
	void SetFieldFillType(PanelFillType val);

	int GetFieldRadius() { return m_param.FieldRadius; }
	void SetFieldRadius(int val);

	COLORREF GetFieldSolidColor() { return m_param.FieldSolidColor; }
	void SetFieldSolidColor(COLORREF val);

	CString GetFieldFileNameTexture() { return m_param.FileName; }
	void SetFieldFileNameTexture(CString val);

	// виртуальные функции, переопределяемые в классе
	virtual void OnDraw();
	virtual void Clear();

    virtual void SaveParameters(SimpleXML *pSimpleXML);
    virtual void LoadParameters(SimpleXML *pSimpleXML);
};
//------------------------------------------------------------------------------