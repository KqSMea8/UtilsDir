//------------------------------------------------------------------------------
// DButtonHMME.h : файл заголовка (Draw Button Help, Min, Max, End)
//------------------------------------------------------------------------------
// Класс отображения служебных кнопок.
// Анимация кнопок при клике триггерного типа.
//------------------------------------------------------------------------------
#pragma once
#include <Interface2012\Draw\CustomDraw.h>
//------------------------------------------------------------------------------
enum TypeButtonHMME
{
	tbHMME_Help,
	tbHMME_Min,
	tbHMME_Max,
	tbHMME_End
};
//------------------------------------------------------------------------------
struct ParamButtonHMME : public ParamCustomDraw
{// Структура параметров отображения служебных кнопок
// В родительской структуре - общие параметры, здесь - частные параметры
	CString fileName;		// базовое имя файлов с картинками
	double dTimeAnimation;	// длительность анимации в сек
	double dTimeCur;		// текущая продолжительность анимации
	double dTimeStart;		// время начала анимации
	int Number_Texture[4];  // номер текстуры в файле ZetIntSource
	TypeButtonHMME CodeButton;
	CString Texture_1_FileName;
	CString Texture_2_FileName;
	CString Texture_3_FileName;
	CString Texture_4_FileName;
	BOOL MouseIsEnter;
#ifdef _GDIPLUS
#else
	GLfloat mat_ambient[4]; 	//	{r, g, b, a}
	GLfloat mat_diffuse[4];		//	{r, g, b, a}
	GLfloat mat_specular[4];	//	{r, g, b, a}
	GLfloat mat_emission[4];	//	{r, g, b, a}
	GLfloat mat_shininess;		//	коэффициент блика
#endif
	ParamButtonHMME()
		: fileName(L"")
		, dTimeAnimation(0.2)	// 200 мсек
		, dTimeCur(0.)
		, CodeButton(tbHMME_End)
		, dTimeStart(0.)
		, Texture_1_FileName(L"$(ZetToolsDir)Interface2012\\Textures\\ButtonHMME\\ButtonHelp.bmp")
		, Texture_2_FileName(L"$(ZetToolsDir)Interface2012\\Textures\\ButtonHMME\\ButtonMin.bmp")
		, Texture_3_FileName(L"$(ZetToolsDir)Interface2012\\Textures\\ButtonHMME\\ButtonMax.bmp")
		, Texture_4_FileName(L"$(ZetToolsDir)Interface2012\\Textures\\ButtonHMME\\ButtonEnd.bmp")
		, MouseIsEnter(FALSE)
	{
		Size = sizeof(ParamButtonHMME);

		Number_Texture[0] = 1;
		Number_Texture[1] = 2;
		Number_Texture[2] = 3;
		Number_Texture[3] = 4;


#ifdef _GDIPLUS
#else
		mat_ambient[0] = 0.35;
		mat_ambient[1] = 0.35;
		mat_ambient[2] = 0.35;
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
#endif
	}
};
//------------------------------------------------------------------------------
class CDButtonHMME : public CCustomDraw
{
public:
#ifdef _GDIPLUS	
	CDButtonHMME(CRect rect, PVOID ppGraphics,
		bool *pFlagIsChange, double *pTimeAnimation);
#else
	CDButtonHMME(CRect rect, bool *pFlagIsChange, double *pTimeAnimation, TypeButtonHMME codeButton);
#endif
	virtual ~CDButtonHMME();

private:
	ParamButtonHMME m_param;// структура всех параметров отображения
#ifdef _GDIPLUS
	Image *m_pImage_Cur;	// ук. на текущую картинку
	Image *m_pImage_s;		// стандартное отображение
	Image *m_pImage_m;		// отображение с мышью
	Image *m_pImage_p;		// отображение в нажатом состоянии
#else
	CRect rect1;
	GLuint Texture;
	CDetail *m_ButtonHMME;
	int PicSize[2];
	bool m_bFlag;
#endif
protected:

public:
	// функции параметров отображения
	CString GetImageFileName() { return m_param.fileName;}
	void SetImageFileName(CString str);
	double GetTimeAnimation() { return m_param.dTimeAnimation;}
	void SetTimeAnimation(double val);
	void Resize(CRect rect);
	// виртуальные функции, переопределяемые в классе
	virtual void OnDraw();
	virtual void Clear (void);
	virtual void DoProcess();
	virtual void OnLButtonClk(UINT nFlags, CPoint point);
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();
    virtual void SaveParameters(SimpleXML *pSimpleXML);
    virtual void LoadParameters(SimpleXML *pSimpleXML);
};
//------------------------------------------------------------------------------