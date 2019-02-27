//------------------------------------------------------------------------------
// DStaticImage.h : файл заголовка
//------------------------------------------------------------------------------
//  ласс отображени€ статической картинки
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Draw\CustomDraw.h"
//------------------------------------------------------------------------------
struct ParamStaticImage : public ParamCustomDraw
{// —труктура параметров отображени€ служебных кнопок
// ¬ родительской структуре - общие параметры, здесь - частные параметры
	CString fileName;		// полное им€ файла с картинкой
	float CoordZ;			// положение картинки по координате Z
	ParamStaticImage()
	{
		Size = sizeof(ParamStaticImage);
		fileName = L"";
		CoordZ = 0.1;
	}
};
//------------------------------------------------------------------------------
class CDStaticImage : public CCustomDraw
{
public:
#ifdef _GDIPLUS
	CDStaticImage(CRect rect, PVOID ppGraphics,
				bool *pFlagIsChange, double *pTimeAnimation);
#else
	CDStaticImage(CRect rect,
		bool *pFlagIsChange, double *pTimeAnimation);
#endif
	virtual ~CDStaticImage();

private:
	ParamStaticImage m_param;// структура всех параметров отображени€
#ifdef _GDIPLUS
	Image *m_pImage;	// 
#else
	GLuint m_pImage;
	int PicSize[2];
#endif
protected:

public:
	// функции параметров отображени€
	CString GetImageFileName() { return m_param.fileName;}
	void SetImageFileName(CString str);

	float GetCoordZ() { return m_param.CoordZ;}
	void SetCoord(float val);

	// виртуальные функции, переопредел€емые в классе
	virtual void OnDraw();
};
//------------------------------------------------------------------------------